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
	RCORE_API virtual void RedirectToWidget(UPARAM(Meta = (AllowAbstract = false)) TSubclassOf<UAssetDashboardUI> WidgetClass);

	// ~ UUserWidget
	RCORE_API virtual void NativeConstruct() override;
	RCORE_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Module Macros
#undef RCORE_API

