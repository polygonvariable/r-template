// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/Collection/AssetCollectionTrade.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"



const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& UAssetCollection_TradeReferenced::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_TradeReferenced::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail_Trade& OutDetail) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const URPrimaryDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		if (AssetId == Asset->GetPrimaryAssetId())
		{
			OutDetail = Kv.Value;
			return true;
		}
	}
	return false;
}

bool UAssetCollection_TradeReferenced::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		URPrimaryDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Kv.Key->GetPrimaryAssetId();
		FAssetDetail AssetDetail = Kv.Value;

		OutAsset = TPair<FPrimaryAssetId, FAssetDetail>(AssetId, AssetDetail);
		return true;
	}
	return false;
}

bool UAssetCollection_TradeReferenced::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const URPrimaryDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		if (AssetId == Asset->GetPrimaryAssetId())
		{
			OutDetail = Kv.Value;
			return true;
		}
	}
	return false;
}

void UAssetCollection_TradeReferenced::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const URPrimaryDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
		OutAssets.Add(AssetId, Kv.Value);
	}
}

void UAssetCollection_TradeReferenced::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const URPrimaryDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
		OutAssets.Add(AssetId, Kv.Value.Quantity);
	}
}

void UAssetCollection_TradeReferenced::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const URPrimaryDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
		OutAssets.Add(AssetId);
	}
}

void UAssetCollection_TradeReferenced::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);


#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail_TradeEd& Item : AssetListEd)
	{
		TSoftObjectPtr<URPrimaryDataAsset> Asset = Item.DataAsset;
		URPrimaryDataAsset* AssetPtr = Asset.LoadSynchronous();
		if (!IsValid(AssetPtr))
		{
			LOG_ERROR(LogAsset, TEXT("Failed to load asset"));
			continue;
		}

		FPrimaryAssetId AssetId = AssetPtr->GetPrimaryAssetId();
		if (AssetType.IsValid() && AssetType != AssetId.PrimaryAssetType)
		{
			LOG_ERROR(LogAsset, TEXT("Non-matching asset types found in collection"));
			continue;
		}

		AssetType = AssetId.PrimaryAssetType;
		AssetList.Add(AssetPtr, Item);
	}

#endif
}

