// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "FFolder.h"
#include "FComputer.h"
#include "UPeacegateFileSystem.generated.h"

class USystemContext;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFilesystemOperationEvent, EFilesystemEventType, InType, FString, InPath);

UENUM(BlueprintType)
enum class EFilesystemStatusCode : uint8
{
	OK,
	FileOrDirectoryNotFound,
	FileOrDirectoryExists,
	DirectoryNotEmpty,
	PermissionDenied,
	UnknownError
};

/**
 * Encapsulates a filesystem of a Peacenet computer.
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UPeacegateFileSystem : public UObject
{
	GENERATED_BODY()

	
private:
	void BuildChildNavigators(UFolderNavigator* RootNav);

public:
	int UserID = 0;
	
public:
	void BuildFolderNavigator();

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	void Initialize(int InUserID);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn="true"))
	USystemContext* SystemContext;

	UPROPERTY()
	UFolderNavigator* Root;

	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FFilesystemOperationEvent FilesystemOperation;

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool CreateDirectory(const FString InPath, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool DirectoryExists(const FString InPath);
	
	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool FileExists(const FString InPath);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool Delete(const FString InPath, const bool InRecursive, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category= "Filesystem")
	bool GetDirectories(const FString& InPath, TArray<FString>& OutDirectories, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool GetFiles(const FString& InPath, TArray<FString>& OutFiles, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	void WriteText(const FString& InPath, const FString& InText);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	void WriteBinary(const FString& InPath, TArray<uint8> InBinary);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool ReadText(const FString& InPath, FString& OutText, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool ReadBinary(const FString& InPath, TArray<uint8>& OutBinary, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool MoveFile(const FString& Source, const FString& Destination, const bool InOverwrite, EFilesystemStatusCode& OutStatusCode);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool MoveFolder(const FString& Source, const FString& Destination, const bool InOverwrite, EFilesystemStatusCode& OutStatusCode);
	
	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	bool CopyFile(const FString& Source, const FString& Destination, const bool InOverwrite, EFilesystemStatusCode& OutStatusCode);

public:
	UFUNCTION(BlueprintCallable, Category="Filesystem")
	static bool IsValidAsFileName(const FString& InFileName);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	static bool IsValidAsUserName(const FString& InUserName);

private:
	bool GetFile(FFolder Parent, FString FileName, int& Index, FFile& File);

	void RecursiveDelete(FFolder& InFolder);

	FFolder GetFolderByID(int FolderID);
	void SetFolderByID(int FolderID, FFolder Folder);
	int GetNewFolderID();

	bool TraversePath(const TArray<FString>& PathParts, UFolderNavigator*& OutNavigator);
	bool TraversePath(const TArray<FString>& PathParts, const int Count, UFolderNavigator*& OutNavigator);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Filesystem")
	static FString ResolveToAbsolute(const FString Path);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Filesystem")
	static TArray<FString> GetPathParts(FString InPath, FString& ResolvedPath);
};
