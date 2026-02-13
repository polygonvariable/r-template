// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogUI.h"

// Generated Headers
#include "CatalogDashboardUI.generated.h"

// Forward Declarations
class UButton;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCatalogDashboardUI : public UCatalogUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton = nullptr;


	UFUNCTION(BlueprintNativeEvent)
	RCOREASSETMANAGER_API void GetAllCatalogUI(TArray<UCatalogUI*>& OutComponent) const;
	RCOREASSETMANAGER_API virtual void GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutComponent) const;

	// ~ UCatalogUI
	RCOREASSETMANAGER_API virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogUI

	// ~ UUserWidget
	RCOREASSETMANAGER_API virtual void NativeConstruct() override;
	RCOREASSETMANAGER_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget
    
};

