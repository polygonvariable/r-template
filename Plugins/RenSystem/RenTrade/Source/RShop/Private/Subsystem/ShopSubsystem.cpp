// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ShopSubsystem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/ShopAsset.h"
#include "Definition/AssetDetail_Trade.h"
#include "Definition/AssetRuleDefinition.h"
#include "Delegate/LatentDelegate.h"
#include "Interface/ShopProviderInterface.h"
#include "Interface/StorageProviderInterface.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Storage/ShopStorage.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/PurchaseItem.h"



void UShopSubsystem::PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogShop, TEXT("Task subsystem is invalid"));
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	UPurchaseItem* Task = TaskSubsystem->CreateTask<UPurchaseItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogShop, TEXT("Failed to create task"));
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->ShopAssetId = ShopAssetId;
	Task->TradeCollectionId = TradeCollectionId;
	Task->TargetAssetId = TargetAssetId;
	Task->StartTask();
}


UShopStorage* UShopSubsystem::GetShopStorage()
{
	IStorageProviderInterface* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}
	return StorageInterface->GetStorage<UShopStorage>(UShopSubsystem::GetStorageId());
}


void UShopSubsystem::QueryItems(const UShopAsset* Asset, const FGuid& CollectionId, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&)> Callback)
{
	UShopStorage* ShopStorage = GetShopStorage();
	if (!IsValid(Asset) || !IsValid(ShopStorage))
	{
		return;
	}

	const UAssetGroup* TradeGroup = Asset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		return;
	}

	FInstancedStruct TradeContext = FInstancedStruct::Make(FAssetRuleContext(CollectionId));
	const UAssetCollection_Trade* AssetCollection = TradeGroup->GetCollectionRule<UAssetCollection_Trade>(TradeContext);
	if (!IsValid(AssetCollection))
	{
		return;
	}

	FPrimaryAssetId ShopAssetId = Asset->GetPrimaryAssetId();
	const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetList = AssetCollection->GetAssetList();

	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetKv : AssetList)
	{
		const URPrimaryDataAsset* ItemDataAsset = AssetKv.Key;
		FAssetDetail_Trade ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, TradeContext);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FShopKey ShopKey(ShopAssetId, CollectionId, ItemAssetId);
		FShopData ShopData;
		if (ShopStorage->GetItem(ShopKey, ShopData))
		{
			ItemDetail.Quota = FMath::Max(0, ItemDetail.Quota - ShopData.PurchaseCount);
		}

		Callback(ItemAssetId, ItemDetail);
	}
}

const UAssetCollection* UShopSubsystem::GetMaterialCollection(const URPrimaryDataAsset* Asset, const FInstancedStruct& Context) const
{
	const IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(Asset);
	if (!ShopProvider)
	{
		return nullptr;
	}
	return ShopProvider->GetPurchaseCost(Context);
}

const UAssetCollection* UShopSubsystem::GetMaterialCollection(const URPrimaryDataAsset* Asset, const FGuid& CollectionId) const
{
	return GetMaterialCollection(Asset, FInstancedStruct::Make(FAssetRuleContext(CollectionId)));
}



void UShopSubsystem::OnPreGameInitialized()
{
	IStorageProviderInterface* StorageInterface = IStorageProviderInterface::Get(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogShop, TEXT("Storage subsystem not found"));
		return;
	}

	FStorageHandle Handle;
	Handle.StorageClass = UShopSubsystem::GetStorageClass();
	Handle.StorageId = UShopSubsystem::GetStorageId();
	Handle.Url = UShopSubsystem::GetStorageUrl();

	StorageInterface->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProviderInterface>(StorageInterface);
}

bool UShopSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UShopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogShop, TEXT("ShopSubsystem initialized"));

	FLatentDelegate::OnPreGameInitialized.AddUObject(this, &UShopSubsystem::OnPreGameInitialized);
}

void UShopSubsystem::Deinitialize()
{
	StorageProvider.Reset();

	FLatentDelegate::OnPreGameInitialized.RemoveAll(this);

	LOG_WARNING(LogShop, TEXT("ShopSubsystem deinitialized"));
	Super::Deinitialize();
}



UShopSubsystem* UShopSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UShopSubsystem* UShopSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UShopSubsystem>();
}



FGuid UShopSubsystem::GetStorageId()
{
	return FGuid(TEXT("390C55A4-BF6C-4874-9346-C422E916AD64"));
}

FString UShopSubsystem::GetStorageUrl()
{
	return TEXT("/api/get/shop");
}

TSubclassOf<UStorage> UShopSubsystem::GetStorageClass()
{
	return UShopStorage::StaticClass();
}

