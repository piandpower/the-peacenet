// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FFolder.generated.h"

UENUM(Blueprintable)
enum class EFileType : uint8
{
	RawText,
	Program,
	Picture,
	Audio,
	ShellScript,
	Contact,
	ChatLog,
	EmailLog,
	Database,
	PeacegateTheme,
	SystemFile
};


USTRUCT(BlueprintType)
struct FFile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFileType FileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileContent;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTOGLOWIA_API FFolder
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int FolderID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString FolderName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsReadOnly = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int> SubFolders;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FFile> Files;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ParentID = -1;
};


UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UFileUtilities : public UObject
{
	GENERATED_BODY()

public:
	static FString ResolveRelativePath(const FString InRelativePath, const FString InWorkingDirectory = TEXT("/"))
	{
		return FString();
	}

	UFUNCTION(BlueprintCallable, Category="Filesystem")
	static FString GetNameFromPath(const FString& InPath)
	{
		FString NewPath(InPath);
		int LastSlash = -1;
		if (!NewPath.FindLastChar(TEXT('/'), LastSlash))
			return NewPath;
		NewPath.RemoveAt(0, LastSlash + 1);
		return NewPath;
	}

	UFUNCTION(BlueprintCallable, Category="Filesystem")
	static void FormatFilesystem(UPARAM(Ref) TArray<FFolder>& InFilesystem)
	{
		InFilesystem.Empty();

		FFolder Root;

		Root.FolderID = 0;
		Root.IsReadOnly = true;
		Root.FolderName = TEXT("");
		Root.SubFolders = TArray<int>();
		Root.Files = TArray<FFile>();

		InFilesystem.Add(Root);
	}
};
