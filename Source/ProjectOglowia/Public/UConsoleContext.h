// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "PTerminalWidget.h"
#include "UPeacegateFileSystem.h"
#include "UConsoleContext.generated.h"
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTOGLOWIA_API UConsoleContext : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UPTerminalWidget* Terminal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString WorkingDirectory = TEXT("/home");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	UPeacegateFileSystem* Filesystem;

	UFUNCTION(BlueprintCallable, Category="Filesystem")
	FString CombineWithWorkingDirectory(const FString& InPath)
	{
		if (InPath.StartsWith("/"))
			return Filesystem->ResolveToAbsolute(InPath);
		return Filesystem->ResolveToAbsolute(WorkingDirectory + TEXT("/") + InPath);
	}

	UFUNCTION(BlueprintCallable, Category="Bash", meta=(Pure))
	FString GetDisplayWorkingDirectory()
	{
		if (WorkingDirectory.StartsWith("/home"))
		{
			FString NewWorkingDirectory(WorkingDirectory);
			NewWorkingDirectory.RemoveFromStart("/home");
			return TEXT("~") + NewWorkingDirectory;
		}
		return WorkingDirectory;
	}

	UFUNCTION(BlueprintCallable, Category = "Console")
		virtual void WriteLine(const FString& InText) { Terminal->WriteLine(InText); }

	UFUNCTION(BlueprintCallable, Category = "Console")
	virtual void Write(const FString& InText) { Terminal->Write(InText); }

	UFUNCTION(BlueprintCallable, Category = "Console")
	virtual void OverwriteLine(const FString& InText) { Terminal->OverwriteLine(InText); }

	UFUNCTION(BlueprintCallable, Category="Console")
		virtual void Clear() { Terminal->Clear(); }

	UFUNCTION(BlueprintCallable, Category="Console", meta=(Latent, LatentInfo="LatentInfo", HidePin="WorldContextObject", DefaultToSelf="WorldContextObject"))
	virtual void ReadLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, FString& OutText) { Terminal->ReadLine(WorldContextObject, LatentInfo, OutText); }
};
