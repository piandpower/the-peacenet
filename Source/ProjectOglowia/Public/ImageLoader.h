// Shamelessly copied from https://wiki.unrealengine.com/Asynchronous_Image_Loading_from_Disk
// Only difference is this version loads from Peacegate OS and not from the host PC.

#pragma once

#include "CoreMinimal.h"
#include "PixelFormat.h"
#include "ThreadSafeBool.h"
#include "ImageLoader.generated.h"

class UTexture2D;
class UPeacegateFileSystem;

/**
 * Allows runtime loading of Unreal textures from Peacegate files.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UImageLoader : public UObject
{
	GENERATED_BODY()

public: 
	/** Loads an image file from disk into a texture on a worker thread. This will not block the calling thread. @return An image loader object with an OnLoadCompleted event that users can bind to, to get notified when loading is done. */ 
	UFUNCTION(BlueprintCallable, Category = ImageLoader, meta = (HidePin = "Outer", DefaultToSelf = "Outer")) 
	static UImageLoader* LoadImageFromDiskAsync(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath);

	/** Loads an image file from disk into a texture on a worker thread. This will not block the calling thread. @return A future object which will hold the image texture once loading is done. */ 
	static TFuture<UTexture2D*> LoadImageFromDiskAsync(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath, TFunction<void()> CompletionCallback);

	/** Loads an image file from disk into a texture. This will block the calling thread until completed. @return A texture created from the loaded image file. */ 
	UFUNCTION(BlueprintCallable, Category = ImageLoader, meta = (HidePin = "Outer", DefaultToSelf = "Outer"))
	static UTexture2D* LoadImageFromDisk(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath);

public: 
	/** Declare a broadcast-style delegate type, which is used for the load completed event. Dynamic multicast delegates are the only type of event delegates that Blueprint scripts can bind to. */ 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImageLoadCompleted, UTexture2D*, Texture);

		/** This accessor function allows C++ code to bind to the event. */ 
	FOnImageLoadCompleted& OnLoadCompleted() { return LoadCompleted; }

private: /** Helper function that initiates the loading operation and fires the event when loading is done. */ 
	void LoadImageAsync(UPeacegateFileSystem* InFilesystem, UObject* Outer, const FString& ImagePath);

		 /** Helper function to dynamically create a new texture from raw pixel data. */ 
	static UTexture2D* CreateTexture(UObject* Outer, const TArray<uint8>& PixelData, int32 InSizeX, int32 InSizeY, EPixelFormat PixelFormat = EPixelFormat::PF_B8G8R8A8, FName BaseName = NAME_None);

private: 
	/** Holds the load completed event delegate. Giving Blueprint access to this private variable allows Blueprint scripts to bind to the event. */ 
	UPROPERTY(BlueprintAssignable, Category = ImageLoader, meta = (AllowPrivateAccess = true)) 
	FOnImageLoadCompleted LoadCompleted;

	/** Holds the future value which represents the asynchronous loading operation. */ 
	TFuture<UTexture2D*> Future;

	struct FImageWriter
	{
		FImageWriter(const TSharedPtr<class IImageWrapper>& InWrapper)
		{
			ImageWrapper = InWrapper;
		}

		TSharedPtr<class IImageWrapper> ImageWrapper;
		mutable FThreadSafeBool bInUse;
	};


public:
	/** Generates bitmap data out of a texture object and returns the binary data of the bitmap. */
	UFUNCTION(BlueprintCallable, Category = ImageLoader)
	static TArray<uint8> GetBitmapData(UPARAM(Ref) UTexture2D* InTexture);

	template<typename TPixelType>
	static bool SaveImage(const TArray<TPixelType>& Bitmap, const FIntPoint& BitmapSize, TArray<uint8>& OutBitmapData);
};
