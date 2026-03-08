// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetailTrade.h"
#include "Management/AssetCollection.h"

// Generated Headers
#include "AssetCollectionTrade.generated.h"

// Module Macros
#define RSYSTEM_API RTRADE_API

// Forward Declarations
class FObjectPreSaveContext;



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Trade Referenced)")
class UAssetCollection_TradeReferenced : public UAssetCollection
{
	GENERATED_BODY()

public:

	RSYSTEM_API virtual const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& GetAssetList() const;
	RSYSTEM_API virtual bool GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail_Trade& OutDetail) const;

	// ~ UAssetCollection
	RSYSTEM_API virtual bool GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const override;
	RSYSTEM_API virtual bool GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const override;
	RSYSTEM_API virtual void GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const override;
	RSYSTEM_API virtual void GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const override;
	RSYSTEM_API virtual void GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const override;
	// ~ End of UAssetCollection

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_TradeEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<URPrimaryDataAsset*, FAssetDetail_Trade> AssetList;

};



// Module Macros
#undef RSYSTEM_API

