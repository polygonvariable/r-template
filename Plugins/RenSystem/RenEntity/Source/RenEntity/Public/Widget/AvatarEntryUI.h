// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "AvatarEntryUI.generated.h"

// Forward Declarations
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract)
class UAvatarEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	// ~ UAssetEntryUI
	virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const override;
	virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetEntryUI

};

