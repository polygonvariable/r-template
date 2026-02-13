// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/CatalogEntryUI.h"

// Generated Headers
#include "AvatarEntryUI.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract)
class UAvatarEntryUI : public UCatalogEntryUI
{

	GENERATED_BODY()

protected:

	// ~ UCatalogEntryUI
	virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const override;
	virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset) override;
	// ~ End of UCatalogEntryUI

};

