// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FFolder.h"
#include "UPeacegateFileSystem.generated.h"

UENUM(BlueprintType)
enum class EFilesystemEventType : uint8
{
	CreateDirectory,
	WriteFile,
	DeleteFile,
	DeleteDirectory,
	MoveDirectory,
	MoveFile,
	CopyDirectory,
	CopyFile
};

UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UFolderNavigator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, UFolderNavigator*> SubFolders;
	
	UPROPERTY()
	int FolderIndex = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFilesystemModifiedEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFilesystemOperationEvent, EFilesystemEventType, InType, FString, InPath);

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacegateFileSystem : public UObject
{
	GENERATED_BODY()

	
private:
	void BuildChildNavigators(UFolderNavigator* RootNav);

public:
	UPROPERTY()
	UFolderNavigator* Root;

	UFUNCTION(BlueprintCallable, Category="Filesystem")
	FString ResolveToAbsolute(const FString Path);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FFilesystemModifiedEvent FilesystemModified;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FFilesystemOperationEvent FilesystemOperation;

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn = "true"))
	TArray<FFolder> FolderTree;

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	void BuildFolderNavigator();

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	void CreateDirectory(const FString InPath);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool DirectoryExists(const FString InPath);
	
	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool FileExists(const FString InPath);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	void Delete(const FString InPath);

	UFUNCTION(BlueprintCallable, Category= "Filesystem")
	TArray<FString> GetDirectories(const FString& InPath);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	TArray<FString> GetFiles(const FString& InPath);

public:
	UFUNCTION(BlueprintCallable, Category="Filesystem")
	static bool IsValidAsFileName(const FString& InFileName);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	static bool IsValidAsUserName(const FString& InUserName);

};
