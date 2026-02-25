// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "TradeCollectionUI.generated.h"

// Module Macros
#define RSYSTEM_API RTRADE_API

// Forward Declarations
class UTradeAsset;
class URAssetManager;
class UAssetCollection;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI, NotBlueprintable)
class UTradeCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	void SetTradeAssetId(FPrimaryAssetId AssetId);

	// ~ UAssetCollectionUI
	RSYSTEM_API virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY()
	URAssetManager* AssetManager = nullptr;

	FGuid TradeLoadId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId TradeAssetId;

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	virtual const UAssetCollection* GetAssetCollection(const URPrimaryDataAsset* Asset) const;

	void StartAssetLoad();
	void CancelAssetLoad();
	void OnAssetLoaded(UTradeAsset* Asset);

	// ~ UUserWidget
	RSYSTEM_API virtual void NativeConstruct() override;
	RSYSTEM_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



// Module Macros
#undef RSYSTEM_API

