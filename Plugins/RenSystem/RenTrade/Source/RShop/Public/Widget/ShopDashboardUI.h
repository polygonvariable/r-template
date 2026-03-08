// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/TradeDashboardUI.h"

// Generated Headers
#include "ShopDashboardUI.generated.h"

// Forward Declarations
class UButton;
class UAssetCollection;
class URPrimaryDataAsset;
class UShopSubsystem;



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UShopDashboardUI : public UTradeDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<UShopSubsystem> ShopSubsystem;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> PurchaseButton = nullptr;


	UFUNCTION()
	void HandlePurchase();

	// ~ UTradeDashboardUI
	virtual const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset) const override;
	// ~ End of UTradeDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

