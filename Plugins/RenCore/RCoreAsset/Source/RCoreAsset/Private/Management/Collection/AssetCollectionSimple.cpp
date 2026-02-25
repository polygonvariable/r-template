// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/Collection/AssetCollectionSimple.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"



const TMap<FPrimaryAssetId, int>& UAssetCollection_Simple::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_Simple::GetAsset(const FPrimaryAssetId& AssetId, int& OutQuantity) const
{
	const int* Quantity = AssetList.Find(AssetId);
	if (Quantity != nullptr)
	{
		OutQuantity = *Quantity;
		return true;
	}
	return false;
}

bool UAssetCollection_Simple::GetAnyAsset(TPair<FPrimaryAssetId, int>& OutPair) const
{
	for (const TPair<FPrimaryAssetId, int>& Pair : AssetList)
	{
		OutPair = Pair;
		return true;
	}
	return false;
}

void UAssetCollection_Simple::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetList.Empty();

	for (const FAssetDetail_SimpleEd& AssetDetail : AssetListEd)
	{
		TSoftObjectPtr<URPrimaryDataAsset> Asset = AssetDetail.DataAsset;
		URPrimaryDataAsset* AssetPtr = Asset.LoadSynchronous();
		if (!IsValid(AssetPtr))
		{
			continue;
		}

		FPrimaryAssetId AssetId = AssetPtr->GetPrimaryAssetId();
		AssetList.Add(AssetId, AssetDetail.Quantity);
	}

#endif
}

