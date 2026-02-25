// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/Collection/AssetCollectionUnique.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"



const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& UAssetCollection_UniqueReferenced::GetAssetList() const
{
	return AssetList;
}

void UAssetCollection_UniqueReferenced::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetList.Empty();

	for (const FAssetDetail_UniqueEd& Shop : AssetListEd)
	{
		TSoftObjectPtr<URPrimaryDataAsset> Asset = Shop.DataAsset;
		URPrimaryDataAsset* AssetPtr = Asset.LoadSynchronous();
		if (!IsValid(AssetPtr))
		{
			continue;
		}

		AssetList.Add(AssetPtr, Shop);
	}

#endif
}



const TMap<FPrimaryAssetId, FAssetDetail_Unique>& UAssetCollection_Unique::GetAssetList() const
{
	return AssetList;
}

void UAssetCollection_Unique::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetList.Empty();

	for (const FAssetDetail_UniqueEd& Shop : AssetListEd)
	{
		TSoftObjectPtr<URPrimaryDataAsset> Asset = Shop.DataAsset;
		URPrimaryDataAsset* AssetPtr = Asset.LoadSynchronous();
		if (!IsValid(AssetPtr))
		{
			continue;
		}

		FPrimaryAssetId AssetId = AssetPtr->GetPrimaryAssetId();
		AssetList.Add(AssetId, Shop);
	}

#endif
}

