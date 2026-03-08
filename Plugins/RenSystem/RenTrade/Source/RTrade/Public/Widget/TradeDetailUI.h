// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "TradeDetailUI.generated.h"

// Forward Declarations
class UAssetEntry;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UTradeDetailUI : public UAssetDetailUI
{

	GENERATED_BODY()

protected:

	// ~ UAssetDetailUI
	virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDetailUI

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

