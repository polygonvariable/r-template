// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/PurchaseItem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Asset/ShopAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Interface/AssetTransactionInterface.h"
#include "Interface/ShopProviderInterface.h"
#include "Library/AssetUtil.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Manager/RAssetManager.inl"
#include "Storage/ShopStorage.h"
#include "Subsystem/ShopSubsystem.h"



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

	TradeCollectionId.Invalidate();

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

	TFuture<FLatentLoadedAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UPurchaseItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<URPrimaryDataAsset>& Result)
		{
			UPurchaseItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<URPrimaryDataAsset*>& Assets = Result.Get();

			This->ShopAsset = Cast<UShopAsset>(Assets[0]);
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

	FInstancedStruct TradeContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection_Trade* TradeCollection = TradeGroup->GetCollectionRule<UAssetCollection_Trade>(TradeContext);
	if (!IsValid(TradeCollection))
	{
		Fail(TEXT("Item collection is invalid"));
		return;
	}

	FAssetDetail_Trade TargetDetail;
	if (!TradeCollection->GetAssetDetail(TargetAssetId, TargetDetail))
	{
		Fail(TEXT("Item asset not found"));
		return;
	}

	TargetQuantity = TargetDetail.Quantity;
	TargetQuota = TargetDetail.Quota;

	Step_CheckMaterial();
}

void UPurchaseItem::Step_CheckMaterial()
{
	const IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(TargetAsset);
	if (!ShopProvider)
	{
		Fail(TEXT("Item asset does not implement IShopProviderInterface"));
		return;
	}

	FInstancedStruct MaterialContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* MaterialCollection = ShopProvider->GetPurchaseCost(MaterialContext);
	if (!IsValid(MaterialCollection))
	{
		Fail(TEXT("Purchase material is invalid"));
		return;
	}

	TMap<FPrimaryAssetId, int> MaterialAssetList;
	MaterialCollection->GetAssetList(MaterialAssetList);

	FPrimaryAssetType MaterialAssetType = MaterialCollection->GetCollectionType();

	Step_CheckQuota(MaterialAssetList, MaterialAssetType);
}

void UPurchaseItem::Step_CheckQuota(const TMap<FPrimaryAssetId, int>& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	if (TargetQuota <= 0)
	{
		Step_PerformTransaction(MaterialAssetList, MaterialAssetType);
		return;
	}

	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	UShopSubsystem* ShopSubsystem = UShopSubsystem::Get(GameInstance);
	if (!IsValid(ShopSubsystem))
	{
		Fail(TEXT("Failed to get ShopSubsystem"));
		return;
	}

	UShopStorage* ShopStorage = ShopSubsystem->GetShopStorage();
	if (!IsValid(ShopStorage))
	{
		Fail(TEXT("Failed to get ShopStorage"));
		return;
	}

	FShopKey ShopKey(ShopAssetId, TradeCollectionId, TargetAssetId);
	FShopData ShopData;
	if (ShopStorage->GetItem(ShopKey, ShopData))
	{
		if (ShopData.PurchaseCount >= TargetQuota)
		{
			Fail(TEXT("Item quota exceeded"));
			return;
		}
	}

	if (!ShopStorage->AddItem(ShopKey))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Step_PerformTransaction(MaterialAssetList, MaterialAssetType);
}

void UPurchaseItem::Step_PerformTransaction(const TMap<FPrimaryAssetId, int>& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
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

