// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail.h"
#include "Management/AssetCollection.h"

// Generated Headers
#include "AssetCollectionSimple.generated.h"

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations
class FObjectPreSaveContext;



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Simple)")
class UAssetCollection_Simple : public UAssetCollection
{

	GENERATED_BODY()

public:

	RCORE_API virtual const TMap<FPrimaryAssetId, int>& GetAssetList() const;
	RCORE_API virtual bool GetAsset(const FPrimaryAssetId& AssetId, int& OutQuantity) const;
	RCORE_API virtual bool GetAnyAsset(TPair<FPrimaryAssetId, int>& OutPair) const;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_SimpleEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<FPrimaryAssetId, int> AssetList;

};



// Module Macros
#undef RCORE_API

