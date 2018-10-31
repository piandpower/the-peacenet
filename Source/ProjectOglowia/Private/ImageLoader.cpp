// Shamelessly copied from https://wiki.unrealengine.com/Asynchronous_Image_Loading_from_Disk
// Only difference is this version loads from Peacegate OS and not from the host PC.

#include "ImageLoader.h"

#include "ImageLoader.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "RenderUtils.h"
#include "PixelFormat.h"
#include "HighResScreenshot.h"
#include "ModuleManager.h"
#include "UPeacegateFileSystem.h"
#include "Engine/Texture2D.h"

// Module loading is not allowed outside of the main thread, so we load the ImageWrapper module ahead of time. 
static IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

UImageLoader* UImageLoader::LoadImageFromDiskAsync(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath)
{
	// This simply creates a new ImageLoader object and starts an asynchronous load. 
	UImageLoader* Loader = NewObject<UImageLoader>();
	Loader->LoadImageAsync(InFilesystem, Outer, ImagePath);
	return Loader;
}

void UImageLoader::LoadImageAsync(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath) 
{ 
	// The asynchronous loading operation is represented by a Future, which will contain the result value once the operation is done. 
	// We store the Future in this object, so we can retrieve the result value in the completion callback below. 
	Future = LoadImageFromDiskAsync(InFilesystem, Outer, ImagePath, [this]() 
	{ 
		// This is the same Future object that we assigned above, but later in time. 
		// At this point, loading is done and the Future contains a value. 
		if (Future.IsValid()) 
		{ 
			// Notify listeners about the loaded texture on the game thread. 
			AsyncTask(ENamedThreads::GameThread, [this]() 
			{ 
				LoadCompleted.Broadcast(Future.Get());
			}); 
		} 
	}); 
}

TFuture<UTexture2D*> UImageLoader::LoadImageFromDiskAsync(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath, TFunction<void()> CompletionCallback)
{ 
	// Run the image loading function asynchronously through a lambda expression, capturing the ImagePath string by value. 
	// Run it on the thread pool, so we can load multiple images simultaneously without interrupting other tasks. 
	return Async<UTexture2D*>(EAsyncExecution::ThreadPool, [=]() 
	{ 
		return LoadImageFromDisk(InFilesystem, Outer, ImagePath);
	}, CompletionCallback);
}

UTexture2D* UImageLoader::LoadImageFromDisk(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath) 
{ 
	// Load the compressed byte data from the file 
	TArray<uint8> FileData;

	EFilesystemStatusCode StatusCode;

	// Check if the file exists first 
	if (!InFilesystem->ReadBinary(ImagePath, FileData, StatusCode)) 
	{ 
		//UIL_LOG(Error, TEXT("File not found or couldn't be read: %s"), *ImagePath); 
		return nullptr;
	}

	// Detect the image type using the ImageWrapper module 
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(FileData.GetData(), FileData.Num());
	
	if (ImageFormat == EImageFormat::Invalid) 
	{ 
		//UIL_LOG(Error, TEXT("Unrecognized image file format: %s"), *ImagePath); 
		return nullptr;
	}

	// Create an image wrapper for the detected image format 
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	
	if (!ImageWrapper.IsValid()) 
	{ 
		//UIL_LOG(Error, TEXT("Failed to create image wrapper for file: %s"), *ImagePath);
		return nullptr;
	}

	// Decompress the image data 
	const TArray<uint8>* RawData = nullptr;
	
	ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num());
	ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData);
	
	if (RawData == nullptr) 
	{ 
		//UIL_LOG(Error, TEXT("Failed to decompress image file: %s"), *ImagePath); 
		return nullptr;
	}

	// Create the texture and upload the uncompressed image data 
	FString TextureBaseName = TEXT("Texture_") + FGuid::NewGuid().ToString();
	return CreateTexture(Outer, *RawData, ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), EPixelFormat::PF_B8G8R8A8, FName(*TextureBaseName));
}

UTexture2D* UImageLoader::CreateTexture(UObject* Outer, const TArray<uint8>& PixelData, int32 InSizeX, int32 InSizeY, EPixelFormat InFormat, FName BaseName) 
{ 
	// Shamelessly copied from UTexture2D::CreateTransient with a few modifications 
	if (InSizeX <= 0 || InSizeY <= 0 || (InSizeX % GPixelFormats[InFormat].BlockSizeX) != 0 || (InSizeY % GPixelFormats[InFormat].BlockSizeY) != 0)
	{ 
		//UIL_LOG(Warning, TEXT("Invalid parameters specified for UImageLoader::CreateTexture()")); 
		return nullptr;
	}

	// Most important difference with UTexture2D::CreateTransient: we provide the new texture with a name and an owner 
	FName TextureName = MakeUniqueObjectName(Outer, UTexture2D::StaticClass(), BaseName);
	UTexture2D* NewTexture = NewObject<UTexture2D>(Outer, TextureName, RF_Transient);

	NewTexture->PlatformData = new FTexturePlatformData();
	NewTexture->PlatformData->SizeX = InSizeX;
	NewTexture->PlatformData->SizeY = InSizeY;
	NewTexture->PlatformData->PixelFormat = InFormat;

	// Allocate first mipmap and upload the pixel data 
	int32 NumBlocksX = InSizeX / GPixelFormats[InFormat].BlockSizeX;
	int32 NumBlocksY = InSizeY / GPixelFormats[InFormat].BlockSizeY;
	FTexture2DMipMap* Mip = new(NewTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = InSizeX;
	Mip->SizeY = InSizeY;
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	
	void* TextureData = Mip->BulkData.Realloc(NumBlocksX * NumBlocksY * GPixelFormats[InFormat].BlockBytes);
	FMemory::Memcpy(TextureData, PixelData.GetData(), PixelData.Num());
	Mip->BulkData.Unlock();

	NewTexture->UpdateResource();
	return NewTexture;
}

TArray<uint8> UImageLoader::GetBitmapData(UTexture2D * InTexture)
{
	// Retrieve the texture's current compression and mipmap settings, we need to nuke 'em temporarily.
	TextureCompressionSettings prevCompression = InTexture->CompressionSettings;
	TextureMipGenSettings prevMipSettings = InTexture->MipGenSettings;
	InTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	InTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;


	// Update texture resources.
	InTexture->UpdateResource();

	// Get the first mipmap of the texture
	FTexture2DMipMap* Mip = &InTexture->PlatformData->Mips[0];

	// Array of colors, for pixel data
	TArray<FColor> Pixels;
	
	// The texture's width and height
	int w = Mip->SizeX;
	int h = Mip->SizeY;

	// Zero out the array.
	Pixels.InsertZeroed(0, w * h);

	// Get the raw data of the texture
	FByteBulkData* RawData = &Mip->BulkData;

	// Retrieve the color data from the texture
	FColor* FormattedData = static_cast<FColor*>(RawData->Lock(LOCK_READ_ONLY));

	// Copy each pixel from the texture to the bitmap.
	for (int i = 0; i < Pixels.Num(); i++)
	{
		Pixels[i] = FormattedData[i];
	}

	// Unlock the texture
	RawData->Unlock();

	// Destination size
	FIntPoint DestSize(w, h);

	TArray<uint8> Ret;
	if (UImageLoader::SaveImage(Pixels, DestSize, Ret))
		return Ret;

	// Revert the texture settings
	InTexture->CompressionSettings = prevCompression;
	InTexture->MipGenSettings = prevMipSettings;

	return TArray<uint8>();
}

// -------------------
// Re-implementation of FHighResScreenshotConfig::SaveImage(), which saves to an in-memory byte buffer instead.
// -------------------


template<typename> struct FPixelTypeTraits {};

template<> struct FPixelTypeTraits<FColor>
{
	static const ERGBFormat SourceChannelLayout = ERGBFormat::BGRA;

	static FORCEINLINE bool IsWritingHDRImage(const bool)
	{
		return false;
	}
};

template<> struct FPixelTypeTraits<FFloat16Color>
{
	static const ERGBFormat SourceChannelLayout = ERGBFormat::RGBA;

	static FORCEINLINE bool IsWritingHDRImage(const bool bCaptureHDR)
	{
		static const auto CVarDumpFramesAsHDR = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.BufferVisualizationDumpFramesAsHDR"));
		return bCaptureHDR || CVarDumpFramesAsHDR->GetValueOnAnyThread();
	}
};

template<> struct FPixelTypeTraits<FLinearColor>
{
	static const ERGBFormat SourceChannelLayout = ERGBFormat::RGBA;

	static FORCEINLINE bool IsWritingHDRImage(const bool bCaptureHDR)
	{
		static const auto CVarDumpFramesAsHDR = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.BufferVisualizationDumpFramesAsHDR"));
		return bCaptureHDR || CVarDumpFramesAsHDR->GetValueOnAnyThread();
	}
};


template<typename TPixelType>
bool UImageLoader::SaveImage(const TArray<TPixelType>& Bitmap, const FIntPoint& BitmapSize, TArray<uint8>& OutBitmapData)
{
	typedef FPixelTypeTraits<TPixelType> Traits;

	static_assert(ARE_TYPES_EQUAL(TPixelType, FFloat16Color) || ARE_TYPES_EQUAL(TPixelType, FColor) || ARE_TYPES_EQUAL(TPixelType, FLinearColor), "Source format must be either FColor, FLinearColor or FFloat16Color");
	const int32 x = BitmapSize.X;
	const int32 y = BitmapSize.Y;

	bool bSuccess = false;

	if (Bitmap.Num() == x * y)
	{
		const size_t BitsPerPixel = (sizeof(TPixelType) / 4) * 8;

		FImageWriter ImageWriter(ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG));
		
		if (ImageWriter.ImageWrapper.IsValid() &&
			ImageWriter.ImageWrapper->SetRaw((void*)&Bitmap[0], sizeof(TPixelType)* x * y, x, y, Traits::SourceChannelLayout, BitsPerPixel))
		{
			EImageCompressionQuality LocalCompressionQuality = EImageCompressionQuality::Default;

			// Compress and write image
			OutBitmapData = ImageWriter.ImageWrapper->GetCompressed((int32)LocalCompressionQuality);
			
			bSuccess = true;
		}

		ImageWriter.bInUse = false;
	}
	else
	{
		//UIL_LOG(Error, TEXT("Failed to write bitmap data."));
	}

	return bSuccess;

}
