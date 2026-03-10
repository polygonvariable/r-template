// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "TradeDashboardUI.generated.h"

// Module Macros
#define RSYSTEM_API RTRADEUI_API

// Forward Declarations
class UAssetCollection;
class UAssetCollectionUI;
class UTradeCollectionUI;
class UTradeDetailUI;
class UAssetEntry;
class UTradeAsset;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI, NotBlueprintable)
class UTradeDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	RSYSTEM_API virtual void ResetDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId TradeAssetId;

	UPROPERTY(EditAnywhere)
	FGuid TradeCollectionId;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTradeCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTradeDetailUI> PrimaryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> SecondaryCollection = nullptr;


	RSYSTEM_API virtual void InitializeTradeDetails(const UTradeAsset* Asset);
	RSYSTEM_API virtual const UAssetCollection* GetTradeMaterialCollection(const URPrimaryDataAsset* Asset) const;

	// ~ UAssetDashboardUI
	RSYSTEM_API virtual void InitializeDetail() override;
	RSYSTEM_API virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	RSYSTEM_API virtual void SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDashboardUI
	
	// ~ UUserWidget
	RSYSTEM_API virtual void NativeConstruct() override;
	RSYSTEM_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	FGuid _TradeLoadId;

};



// Module Macros
#undef RSYSTEM_API

