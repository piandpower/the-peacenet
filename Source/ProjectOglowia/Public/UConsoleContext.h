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
	UPROPERTY(BlueprintReadOnly)
	int UserID;

	UPROPERTY(BlueprintReadOnly)
	FString HomeDirectory;

	UPROPERTY()
	TScriptInterface<class ISystemContext> SystemContext;

	UPROPERTY()
	UPTerminalWidget* Terminal;

	UPROPERTY(BlueprintReadOnly)
	FString WorkingDirectory;

	UPROPERTY(BlueprintReadOnly)
	UPeacegateFileSystem* Filesystem;

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	virtual UConsoleContext* CreateChildContext(TScriptInterface<ISystemContext> InSystemContext, int InUserID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Console Context")
	FString GetHostname();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Console Context")
	FString GetUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Console Context")
	FString GetUserTypeDisplay();

	UFUNCTION(BlueprintCallable, Category = "Console Context")
	void SetWorkingDirectory(const FString& InPath);

	UFUNCTION(BlueprintCallable, Category = "Filesystem")
	FString CombineWithWorkingDirectory(const FString& InPath);

	UFUNCTION(BlueprintCallable, Category = "Bash", meta = (Pure))
	FString GetDisplayWorkingDirectory();
	
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
