// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/Collection/AssetCollectionUnique.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"



const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& UAssetCollection_UniqueReferenced::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_UniqueReferenced::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& Kv : AssetList)
	{
		if (!IsValid(Kv.Key))
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

bool UAssetCollection_UniqueReferenced::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& Kv : AssetList)
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

void UAssetCollection_UniqueReferenced::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& Kv : AssetList)
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

void UAssetCollection_UniqueReferenced::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& Kv : AssetList)
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

void UAssetCollection_UniqueReferenced::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& Kv : AssetList)
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

void UAssetCollection_UniqueReferenced::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail_UniqueEd& Item : AssetListEd)
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











const TMap<FPrimaryAssetId, FAssetDetail_Unique>& UAssetCollection_Unique::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_Unique::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& AssetPair : AssetList)
	{
		const FPrimaryAssetId& AssetId = AssetPair.Key;
		const FAssetDetail& AssetDetail = AssetPair.Value;

		OutAsset = TPair<FPrimaryAssetId, FAssetDetail>(AssetId, AssetDetail);
		return true;
	}
	return false;
}

bool UAssetCollection_Unique::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	const FAssetDetail_Unique* Detail = AssetList.Find(AssetId);
	if (Detail == nullptr)
	{
		return false;
	}
	OutDetail = *Detail;
	return true;
}

void UAssetCollection_Unique::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key, Kv.Value);
	}
}

void UAssetCollection_Unique::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key, Kv.Value.Quantity);
	}
}

void UAssetCollection_Unique::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key);
	}
}

void UAssetCollection_Unique::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail_UniqueEd& Item : AssetListEd)
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
		AssetList.Add(AssetId, Item);
	}

#endif
}

