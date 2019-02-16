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
#include "Engine/GameInstance.h"
#include "UPeacenetSettings.h"
#include "UPeacenetGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSettingsAppliedEvent, UPeacenetSettings*, InSettings);

class UPeacenetGameTypeAsset;

/**
 * A game instance specifically for Peacenet.
 */
UCLASS(BlueprintType)
class PROJECTOGLOWIA_API UPeacenetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UPeacenetGameTypeAsset*> GameTypes;

	// The settings for the current instance of The Peacenet.
	UPROPERTY()
	UPeacenetSettings* Settings;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FSettingsAppliedEvent SettingsApplied;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	UPeacenetSettings* GetSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Peacenet")
	TArray<UPeacenetGameTypeAsset*> const& GetGameTypes() const;

	UFUNCTION(BlueprintCallable, Category = "Peacenet")
	void CreateWorld(FString InCharacterName, UPeacenetGameTypeAsset* InGameType);

public: // UGameInstance overrides.
	virtual void Init() override;
	virtual void Shutdown() override;
};
