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
