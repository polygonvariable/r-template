// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/CraftItem.h"

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Asset/TradeAsset.h"
#include "Interface/AssetTransactionInterface.h"
#include "Interface/CraftProviderInterface.h"
#include "Library/AssetUtil.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollectionUnique.h"
#include "Manager/RAssetManager.inl"




void UCraftItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadShopAsset();
}

void UCraftItem::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UCraftItem::OnCleanup()
{
	AssetManager = nullptr;
	ShopAsset = nullptr;
	ItemAsset = nullptr;

	CraftAssetId = FPrimaryAssetId();
	ItemAssetId = FPrimaryAssetId();
	CostAssetId = FPrimaryAssetId();

	ItemId.Invalidate();
	ItemQuantity = 0;
	CostId.Invalidate();
	CostQuantity = 0;
}

void UCraftItem::Step_LoadShopAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(CraftAssetId);
	Assets.Add(ItemAssetId);

	TFuture<FLatentResultAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UCraftItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAssets<URPrimaryDataAsset>& Result)
		{
			UCraftItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<URPrimaryDataAsset*>& Assets = Result.Get();

			This->ShopAsset = Cast<UTradeAsset>(Assets[0]);
			This->ItemAsset = Assets[1];

			This->Step_CheckItem();
		}
	);
}

void UCraftItem::Step_CheckItem()
{
	if (!IsValid(ShopAsset))
	{
		Fail(TEXT("Shop asset is invalid"));
		return;
	}

	const UAssetGroup* ShopItems = ShopAsset->TradeItems;
	if (!IsValid(ShopItems))
	{
		Fail(TEXT("Shop items is invalid"));
		return;
	}

	const UAssetCollection_UniqueReferenced* ItemCollection = ShopItems->GetCollectionRule<UAssetCollection_UniqueReferenced>();
	if (!IsValid(ItemCollection))
	{
		Fail(TEXT("Item collection is invalid"));
		return;
	}

	const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& ItemList = ItemCollection->GetAssetList();

	const FAssetDetail_Unique* ItemDetail = ItemList.Find(ItemAsset);
	if (!ItemDetail)
	{
		Fail(TEXT("Item asset not found"));
		return;
	}

	ItemId = ItemDetail->ItemId;
	ItemQuantity = ItemDetail->Quantity;

	Step_CheckCost();
}

void UCraftItem::Step_CheckCost()
{
	const ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(ItemAsset);
	if (!CraftProvider)
	{
		Fail(TEXT("Item asset does not implement ICraftProviderInterface"));
		return;
	}

	const UAssetCollection_Unique* CraftCost = Cast<UAssetCollection_Unique>(CraftProvider->GetCraftingItems());
	if (!IsValid(CraftCost))
	{
		Fail(TEXT("Craft cost is invalid"));
		return;
	}

	const TMap<FPrimaryAssetId, FAssetDetail_Unique>& CostItems = CraftCost->GetAssetList();
	const FAssetDetail_Unique* CostDetail = CostItems.Find(CostAssetId);
	if (!CostDetail)
	{
		Fail(TEXT("Item asset not found"));
		return;
	}

	CostId = CostDetail->ItemId;
	CostQuantity = CostDetail->Quantity;

	Step_CraftItem();
}

void UCraftItem::Step_CraftItem()
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	IAssetTransactionInterface* SourceTransaction = AssetUtil::GetTransactionInterface(GameInstance, ItemAssetId.PrimaryAssetType);
	IAssetTransactionInterface* TargetTransaction = AssetUtil::GetTransactionInterface(GameInstance, CostAssetId.PrimaryAssetType);

	if (!SourceTransaction || !TargetTransaction)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FGuid SourceSlotId = SourceTransaction->GetDefaultSlotId();
	FGuid TargetSlotId = TargetTransaction->GetDefaultSlotId();

	if (!SourceTransaction->RemoveItem(SourceSlotId, CostAssetId, CostQuantity))
	{
		Fail(TEXT("Failed to remove item"));
		return;
	}
	
	if (!TargetTransaction->AddItem(TargetSlotId, ItemAssetId, ItemQuantity))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Success();
}

