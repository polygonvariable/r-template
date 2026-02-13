// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogFilterUI.h"

// Generated Headers
#include "InventoryFilterUI.generated.h"

// Forward Declarations



/**
 *
 *
 *
 */
UCLASS(Abstract)
class UInventoryFilterUI : public UCatalogFilterUI
{

	GENERATED_BODY()

protected:

	// ~ UCatalogUI
	virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const override;
	virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogUI

};

