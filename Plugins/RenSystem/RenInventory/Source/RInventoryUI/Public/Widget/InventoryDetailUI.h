// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogDetailUI.h"

// Generated Headers
#include "InventoryDetailUI.generated.h"

// Forward Declarations
class UTextBlock;
class UAscensionDetailUI;
class UInventorySubsystem;

struct FInventoryItem;



/**
 *
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDetailUI : public UCatalogDetailUI
{

	GENERATED_BODY()

public:

	void SetContainerId(FGuid Id);

	// ~ UCatalogDetailUI
	virtual void RefreshDetails() override;
	// ~ End of UCatalogDetailUI

protected:

	FGuid ActiveItemId;

	UPROPERTY(EditAnywhere)
	FGuid ContainerId;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryQuantity = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UAscensionDetailUI> AscensionDetail = nullptr;
	

	// ~ UCatalogDetailUI
	virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const override;
	virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogDetailUI

	virtual void SetCustomDetails(const FInventoryItem* Item, int Quantity);

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

