// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "ShopDashboardUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UShopCollectionUI;
class UShopDetailUI;
class UAssetEntry;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract)
class UShopDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId ShopAssetId;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UShopCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UShopDetailUI> PrimaryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> SecondaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> PurchaseButton = nullptr;


	virtual void SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;

	UFUNCTION()
	void HandlePurchase();

	// ~ UAssetDashboardUI
	virtual void GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const;
	// ~ End of UAssetDashboardUI
	 
	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

