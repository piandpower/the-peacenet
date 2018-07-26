// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class PROJECTOGLOWIA_API SPeacegateTerminalWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPeacegateTerminalWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
