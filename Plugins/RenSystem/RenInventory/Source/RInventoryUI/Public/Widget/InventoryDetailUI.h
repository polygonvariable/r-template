// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "InventoryDetailUI.generated.h"

// Forward Declarations
class UTextBlock;
class UAscensionDetailUI;
class UInventorySubsystem;
class URPrimaryDataAsset;
struct FInventoryItem;



/**
 *
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDetailUI : public UAssetDetailUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDetailUI
	virtual void RefreshDetails() override;
	// ~ End of UAssetDetailUI

protected:

	FGuid ActiveItemId;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryQuantity = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UAscensionDetailUI> AscensionDetail = nullptr;


	virtual void SetCustomDetails(const FInventoryItem* Item, int Quantity);

	// ~ UAssetDetailUI
	virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDetailUI

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

