// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "InventoryDashboardUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UAssetDetailUI;
class UInventoryAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	virtual void ResetDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> InventoryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> InventoryDetail = nullptr;


	UFUNCTION(BlueprintNativeEvent)
	void SetCustomDetail(const UInventoryAsset* Asset);
	virtual void SetCustomDetail_Implementation(const UInventoryAsset* Asset) {};

	// ~ UAssetDashboardUI
	virtual void RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass) override;
	virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

