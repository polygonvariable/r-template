// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/TradeAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollectionUnique.h"
#include "Manager/RAssetManager.inl"
#include "Widget/TradeEntry.h"



void UTradeCollectionUI::SetTradeAssetId(FPrimaryAssetId AssetId)
{
	TradeAssetId = AssetId;
}

void UTradeCollectionUI::DisplayEntries()
{
	CancelAssetLoad();
	StartAssetLoad();
}

const UAssetCollection* UTradeCollectionUI::GetAssetCollection(const URPrimaryDataAsset* Asset) const
{
	return nullptr;
}

void UTradeCollectionUI::StartAssetLoad()
{
	if (!IsValid(AssetManager))
	{
		return;
	}

	TradeLoadId = FGuid::NewGuid();

	TFuture<FLatentResultAsset<UTradeAsset>> Future = AssetManager->FetchPrimaryAsset<UTradeAsset>(TradeLoadId, TradeAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogTrade, TEXT("Failed to fetch ShopAsset"));
		return;
	}

	TWeakObjectPtr<UTradeCollectionUI> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAsset<UTradeAsset>& Result)
		{
			UTradeCollectionUI* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				return;
			}
			This->OnAssetLoaded(Result.Asset);
		}
	);
}

void UTradeCollectionUI::CancelAssetLoad()
{
	if (IsValid(AssetManager))
	{
		AssetManager->CancelFetch(TradeLoadId);
	}
}

void UTradeCollectionUI::OnAssetLoaded(UTradeAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	const UAssetGroup* AssetGroup = Asset->TradeItems;
	if (!IsValid(AssetGroup))
	{
		return;
	}

	const UAssetCollection_UniqueReferenced* AssetCollection = Cast<UAssetCollection_UniqueReferenced>(AssetGroup->GetCollectionRule());
	if (!IsValid(AssetCollection))
	{
		return;
	}

	const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& AssetList = AssetCollection->GetAssetList();

	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& AssetKv : AssetList)
	{
		const URPrimaryDataAsset* ItemDataAsset = AssetKv.Key;
		const FAssetDetail_Unique& TradeItem = AssetKv.Value;

		const UAssetCollection_Unique* CostCollection = Cast<UAssetCollection_Unique>(GetAssetCollection(ItemDataAsset));
		if (!IsValid(CostCollection))
		{
			continue;
		}

		const TMap<FPrimaryAssetId, FAssetDetail_Unique>& CostItems = CostCollection->GetAssetList();
		for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& CostKv : CostItems)
		{
			UTradeEntry* Entry = GetEntryFromPool<UTradeEntry>();
			if (IsValid(Entry))
			{
				const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

				Entry->TradeItem = TradeItem;
				Entry->CostAssetId = CostKv.Key;
				Entry->CostItem = CostKv.Value;
				AddEntry(ItemAssetId, Entry);
			}
		}
	}
}

void UTradeCollectionUI::NativeConstruct()
{
	AssetManager = Cast<URAssetManager>(URAssetManager::GetIfInitialized());

	Super::NativeConstruct();
}

void UTradeCollectionUI::NativeDestruct()
{
	CancelAssetLoad();
	AssetManager = nullptr;

	Super::NativeDestruct();
}

