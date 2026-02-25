// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "TradeDashboardUI.generated.h"

// Module Macros
#define RSYSTEM_API RTRADE_API

// Forward Declarations
class UAssetCollectionUI;
class UTradeCollectionUI;
class UTradeDetailUI;
class UAssetEntry;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI, NotBlueprintable)
class UTradeDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId TradeAssetId;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTradeCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTradeDetailUI> PrimaryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> SecondaryCollection = nullptr;


	// ~ UAssetDashboardUI
	RSYSTEM_API virtual void SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	RSYSTEM_API virtual void GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const override;
	// ~ End of UAssetDashboardUI
	
	// ~ UUserWidget
	RSYSTEM_API virtual void NativePreConstruct() override;
	RSYSTEM_API virtual void NativeConstruct() override;
	RSYSTEM_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



// Module Macros
#undef RSYSTEM_API

