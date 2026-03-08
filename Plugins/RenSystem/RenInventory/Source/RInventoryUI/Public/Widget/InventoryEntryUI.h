// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "InventoryEntryUI.generated.h"

// Forward Declarations
class UTextBlock;
class URPrimaryDataAsset;



/**
 *
 * 
 * 
 */
UCLASS(Abstract)
class UInventoryEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryQuantity = nullptr;

	// ~ UInventoryUI
	virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UInventoryUI

};

