// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail.h"
#include "Management/AssetCollection.h"

// Generated Headers
#include "AssetCollectionUnique.generated.h"

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations
class FObjectPreSaveContext;
class URPrimaryDataAsset;



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Unique Referenced)")
class UAssetCollection_UniqueReferenced : public UAssetCollection
{

	GENERATED_BODY()

public:

	RCORE_API virtual const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& GetAssetList() const;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_UniqueEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<URPrimaryDataAsset*, FAssetDetail_Unique> AssetList;

};



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Unique)")
class UAssetCollection_Unique : public UAssetCollection
{

	GENERATED_BODY()

public:

	RCORE_API virtual const TMap<FPrimaryAssetId, FAssetDetail_Unique>& GetAssetList() const;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_UniqueEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<FPrimaryAssetId, FAssetDetail_Unique> AssetList;

};



// Module Macros
#undef RCORE_API

