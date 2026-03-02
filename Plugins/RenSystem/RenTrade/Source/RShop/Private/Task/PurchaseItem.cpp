// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/PurchaseItem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Asset/TradeAsset.h"
#include "Definition/AssetRuleDefinition.h"
#include "Interface/AssetTransactionInterface.h"
#include "Interface/ShopProviderInterface.h"
#include "Library/AssetUtil.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollectionUnique.h"
#include "Manager/RAssetManager.inl"




void UPurchaseItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadAsset();
}

void UPurchaseItem::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UPurchaseItem::OnCleanup()
{
	AssetManager = nullptr;
	ShopAsset = nullptr;
	TargetAsset = nullptr;

	ShopAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	CostTags.Reset();

	TargetQuantity = 0;
}

void UPurchaseItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(ShopAssetId);
	Assets.Add(TargetAssetId);

	TFuture<FLatentResultAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UPurchaseItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAssets<URPrimaryDataAsset>& Result)
		{
			UPurchaseItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<URPrimaryDataAsset*>& Assets = Result.Get();

			This->ShopAsset = Cast<UTradeAsset>(Assets[0]);
			This->TargetAsset = Assets[1];

			This->Step_CheckTarget();
		}
	);
}

void UPurchaseItem::Step_CheckTarget()
{
	if (!IsValid(ShopAsset))
	{
		Fail(TEXT("Shop asset is invalid"));
		return;
	}

	const UAssetGroup* TradeGroup = ShopAsset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		Fail(TEXT("Trade group is invalid"));
		return;
	}

	const UAssetCollection* TradeCollection = TradeGroup->GetCollectionRule();
	if (!IsValid(TradeCollection))
	{
		Fail(TEXT("Item collection is invalid"));
		return;
	}

	FAssetDetail TargetDetail;
	if (!TradeCollection->GetAssetDetail(TargetAssetId, TargetDetail))
	{
		Fail(TEXT("Item asset not found"));
		return;
	}

	TargetQuantity = TargetDetail.Quantity;

	Step_CheckCost();
}

void UPurchaseItem::Step_CheckCost()
{
	const IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(TargetAsset);
	if (!ShopProvider)
	{
		Fail(TEXT("Item asset does not implement IShopProviderInterface"));
		return;
	}

	FInstancedStruct Context = FInstancedStruct::Make(FAssetRuleContext(CostTags));

	const UAssetCollection* CostCollection = ShopProvider->GetPurchaseCost(Context);
	if (!IsValid(CostCollection))
	{
		Fail(TEXT("Purchase cost is invalid"));
		return;
	}

	TMap<FPrimaryAssetId, int> CostAssetList;
	CostCollection->GetAssetList(CostAssetList);

	FPrimaryAssetType CostAssetType = CostCollection->GetCollectionType();

	Step_PerformTransaction(CostAssetList, CostAssetType);
}

void UPurchaseItem::Step_PerformTransaction(const TMap<FPrimaryAssetId, int>& CostAssetList, FPrimaryAssetType CostAssetType)
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	IAssetTransactionInterface* TargetTransaction = AssetUtil::GetTransactionInterface(GameInstance, TargetAssetId);
	IAssetTransactionInterface* CostTransaction = AssetUtil::GetTransactionInterface(GameInstance, CostAssetType);

	if (!TargetTransaction || !CostTransaction)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FGuid TargetSlotId = TargetTransaction->GetDefaultSlotId();
	FGuid CostSlotId = CostTransaction->GetDefaultSlotId();

	if (!CostTransaction->RemoveItems(CostSlotId, CostAssetList, 1))
	{
		Fail(TEXT("Failed to remove item"));
		return;
	}
	
	if (!TargetTransaction->AddItem(TargetSlotId, TargetAssetId, TargetQuantity))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Success();
}

