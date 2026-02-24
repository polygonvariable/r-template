// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "ShopEntryUI.generated.h"

// Forward Declarations
class UTextBlock;
class UAssetEntry;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract)
class UShopEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> CostQuantity = nullptr;

	// ~ UInventoryUI
	virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UInventoryUI

};

