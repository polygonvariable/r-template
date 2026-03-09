// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetDetailUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API

// Forward Declarations
class UImage;
class UTextBlock;
class UWidgetSwitcher;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetDetailUI : public UAssetUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
	bool bAutoRefresh = false;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryDescription = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitch = nullptr;


	// ~ UAssetUI
	RCORE_API virtual void SwitchDetail(bool bPrimary);
	// ~ End of UAssetUI

};



// Module Macros
#undef RCORE_API

