// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "InventoryDashboardUI.generated.h"

// Forward Declarations
class UInventoryCollectionUI;
class UInventoryDetailUI;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> PrimaryDetail = nullptr;

	UFUNCTION(BlueprintCallable)
	void RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass);


	// ~ UAssetDashboardUI
	virtual void GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

