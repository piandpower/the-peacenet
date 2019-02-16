/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


#pragma once

#include "CoreMinimal.h"
#include "FFolder.generated.h"

USTRUCT(BlueprintType)
struct FFile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FileName;

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
