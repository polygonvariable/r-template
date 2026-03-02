// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetDashboardUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API

// Forward Declarations
class UButton;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetDashboardUI : public UAssetUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton = nullptr;


	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass);

	UFUNCTION(BlueprintNativeEvent)
	RCORE_API void GetAllAssetUI(TArray<UAssetUI*>& OutAssetUI) const;
	RCORE_API virtual void GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const;

	// ~ UAssetUI
	RCORE_API virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	RCORE_API virtual void NativeConstruct() override;
	RCORE_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



// Module Macros
#undef RCORE_API

