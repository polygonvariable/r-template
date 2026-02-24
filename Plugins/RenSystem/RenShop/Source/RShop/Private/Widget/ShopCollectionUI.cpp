// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/ShopAsset.h"
#include "Interface/ShopProviderInterface.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Manager/RAssetManager.inl"
#include "Widget/ShopEntry.h"


void UShopCollectionUI::SetShopAssetId(FPrimaryAssetId AssetId)
{
	ShopAssetId = AssetId;
}

void UShopCollectionUI::DisplayEntries()
{
	CancelShopLoad();
	StartShopLoad();
}

void UShopCollectionUI::StartShopLoad()
{
	if (!IsValid(AssetManager))
	{
		return;
	}

	ShopLoadId = FGuid::NewGuid();

	TFuture<FLatentResultAsset<UShopAsset>> Future = AssetManager->FetchPrimaryAsset<UShopAsset>(ShopLoadId, ShopAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogItemShop, TEXT("Failed to fetch ShopAsset"));
		return;
	}

	TWeakObjectPtr<UShopCollectionUI> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAsset<UShopAsset>& Result)
		{
			UShopCollectionUI* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				return;
			}
			This->OnShopLoaded(Result.Asset);
		}
	);
}

void UShopCollectionUI::CancelShopLoad()
{
	if (IsValid(AssetManager))
	{
		AssetManager->CancelFetch(ShopLoadId);
	}
}

void UShopCollectionUI::OnShopLoaded(UShopAsset* ShopAsset)
{
	if (!IsValid(ShopAsset))
	{
		return;
	}

	const UAssetGroup* AssetGroup = ShopAsset->ShopItems;
	if (!IsValid(AssetGroup))
	{
		return;
	}

	const UAssetCollection_UniqueReferenced* AssetCollection = Cast<UAssetCollection_UniqueReferenced>(AssetGroup->GetCollectionRule());
	if (!IsValid(AssetCollection))
	{
		return;
	}

	const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& AssetList = AssetCollection->AssetList;

	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Unique>& Asset : AssetList)
	{
		const URPrimaryDataAsset* ItemDataAsset = Asset.Key;
		const FAssetDetail_Unique& ShopItem = Asset.Value;

		const IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(ItemDataAsset);
		if (!ShopProvider)
		{
			continue;
		}

		const UAssetCollection_Unique* PurchaseCost = Cast<UAssetCollection_Unique>(ShopProvider->GetPurchaseCost());
		if (!IsValid(PurchaseCost))
		{
			continue;
		}

		const TMap<FPrimaryAssetId, FAssetDetail_Unique>& CostItems = PurchaseCost->AssetList;
		for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Cost : CostItems)
		{
			UShopEntry* Entry = GetEntryFromPool<UShopEntry>();
			if (IsValid(Entry))
			{
				const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

				Entry->ShopItem = ShopItem;
				Entry->CostAssetId = Cost.Key;
				Entry->CostItem = Cost.Value;
				AddEntry(ItemAssetId, Entry);
			}
		}
	}
}

void UShopCollectionUI::NativeConstruct()
{
	AssetManager = Cast<URAssetManager>(URAssetManager::GetIfInitialized());

	Super::NativeConstruct();
}

void UShopCollectionUI::NativeDestruct()
{
	CancelShopLoad();
	AssetManager = nullptr;

	Super::NativeDestruct();
}

