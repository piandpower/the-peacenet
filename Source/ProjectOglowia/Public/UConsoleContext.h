// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "PTerminalWidget.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacegateFileSystem.h"
#include "ETerminalColor.h"
#include "UConsoleContext.generated.h"

class UUserContext;

/**
 * Encapsulates a terminal.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UConsoleContext : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UUserContext* UserContext;

	UPROPERTY()
	FString WorkingDirectory;

	UPROPERTY()
	UPTerminalWidget* Terminal;

public:
	UFUNCTION()
	void Setup(UUserContext* InUserContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Console Context")
	UUserContext* GetUserContext();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Console Context")
	FString GetWorkingDirectory();

	UFUNCTION()
	virtual UPTerminalWidget* GetTerminal();

	UFUNCTION()
	void SetTerminal(UPTerminalWidget* InTerminalWidget);

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void MakeBold();

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void MakeBoldItalic();

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void MakeItalic();

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void ResetFormatting();

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void SetColor(ETerminalColor InColor);

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void InvertColors();

	UFUNCTION(BlueprintCallable, Category = "Console|Formatting")
	virtual void SetAttention();

	UFUNCTION(BlueprintCallable)
		void InjectInput(const FString& Input);
	
	UFUNCTION()
	virtual FString SynchronouslyReadLine();

	UFUNCTION(BlueprintCallable, Category = "Peacegate")
	virtual UConsoleContext* CreateChildContext(USystemContext* InSystemContext, int InUserID);

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
	virtual void ReadLine(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo, FString& OutText);
};
