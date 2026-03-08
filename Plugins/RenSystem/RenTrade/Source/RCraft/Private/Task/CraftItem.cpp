// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/CraftItem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Interface/AssetTransactionInterface.h"
#include "Interface/CraftProviderInterface.h"
#include "Library/AssetUtil.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Manager/RAssetManager.inl"




void UCraftItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadAsset();
}

void UCraftItem::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UCraftItem::OnCleanup()
{
	AssetManager = nullptr;
	TradeAsset = nullptr;
	TargetAsset = nullptr;

	CraftAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	TargetQuantity = 0;
}

void UCraftItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(CraftAssetId);
	Assets.Add(TargetAssetId);

	TFuture<FLatentLoadedAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UCraftItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<URPrimaryDataAsset>& Result)
		{
			UCraftItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<URPrimaryDataAsset*>& Assets = Result.Get();

			This->TradeAsset = Cast<UTradeAsset>(Assets[0]);
			This->TargetAsset = Assets[1];

			This->Step_CheckTarget();
		}
	);
}

void UCraftItem::Step_CheckTarget()
{
	if (!IsValid(TradeAsset))
	{
		Fail(TEXT("TradeAsset is invalid"));
		return;
	}

	const UAssetGroup* TradeGroup = TradeAsset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		Fail(TEXT("Shop items is invalid"));
		return;
	}

	FInstancedStruct TradeContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* TradeCollection = TradeGroup->GetCollectionRule(TradeContext);
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

	Step_CheckMaterial();
}

void UCraftItem::Step_CheckMaterial()
{
	const ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(TargetAsset);
	if (!CraftProvider)
	{
		Fail(TEXT("Item asset does not implement ICraftProviderInterface"));
		return;
	}

	FInstancedStruct MaterialContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* MaterialCollection = CraftProvider->GetCraftingMaterial(MaterialContext);
	if (!IsValid(MaterialCollection))
	{
		Fail(TEXT("Crafting material is invalid"));
		return;
	}

	TMap<FPrimaryAssetId, int> MaterialAssetList;
	MaterialCollection->GetAssetList(MaterialAssetList);

	FPrimaryAssetType MaterialAssetType = MaterialCollection->GetCollectionType();

	Step_PerformTransaction(MaterialAssetList, MaterialAssetType);
}

void UCraftItem::Step_PerformTransaction(const TMap<FPrimaryAssetId, int>& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	IAssetInterchangeInterface* TargetInterchange = AssetUtil::GetAssetInterchange(GameInstance, TargetAssetId);
	IAssetInterchangeInterface* MaterialInterchange = AssetUtil::GetAssetInterchange(GameInstance, MaterialAssetType);

	if (!TargetInterchange || !MaterialInterchange)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FGuid TargetId = TargetInterchange->GetDefaultSourceId();
	FGuid MaterialId = MaterialInterchange->GetDefaultSourceId();

	IAssetTransactionInterface* TargetTransaction = TargetInterchange->GetTransactionSource(TargetId);
	IAssetTransactionInterface* MaterialTransaction = MaterialInterchange->GetTransactionSource(MaterialId);

	if (!TargetTransaction || !MaterialTransaction)
	{
		Fail(TEXT("Failed to get transaction source"));
		return;
	}

	if (!MaterialTransaction->RemoveItems(MaterialAssetList, 1))
	{
		Fail(TEXT("Failed to remove item"));
		return;
	}
	
	if (!TargetTransaction->AddItem(TargetAssetId, TargetQuantity))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Success();
}

