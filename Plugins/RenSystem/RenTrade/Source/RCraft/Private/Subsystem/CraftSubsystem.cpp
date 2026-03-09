// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/CraftSubsystem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail_Trade.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/TradeKey.h"
#include "Delegate/LatentDelegate.h"
#include "Interface/CraftProviderInterface.h"
#include "Interface/StorageProviderInterface.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Storage/CraftStorage.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/Task_CraftItem.h"
#include "Task/Task_ClaimCraftItem.h"



void UCraftSubsystem::ClaimCraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		return;
	}

	UTask_ClaimCraftItem* Task = TaskSubsystem->CreateTask<UTask_ClaimCraftItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->CraftAssetId = CraftAssetId;
	Task->TargetAssetId = TargetAssetId;
	Task->TradeCollectionId = TradeCollectionId;
	Task->StartTask();
}

void UCraftSubsystem::CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		return;
	}

	UTask_CraftItem* Task = TaskSubsystem->CreateTask<UTask_CraftItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->CraftAssetId = CraftAssetId;
	Task->TargetAssetId = TargetAssetId;
	Task->TradeCollectionId = TradeCollectionId;
	Task->StartTask();
}

UCraftStorage* UCraftSubsystem::GetCraftStorage()
{
	IStorageProviderInterface* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}
	return StorageInterface->GetStorage<UCraftStorage>(UCraftSubsystem::GetStorageId());
}

void UCraftSubsystem::QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)> Callback)
{
	UCraftStorage* CraftStorage = GetCraftStorage();
	if (!IsValid(Asset) || !IsValid(CraftStorage))
	{
		return;
	}

	const UAssetGroup* TradeGroup = Asset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		return;
	}

	FInstancedStruct Context = FInstancedStruct::Make(FAssetRuleContext(CollectionId));
	const UAssetCollection_Trade* AssetCollection = TradeGroup->GetCollectionRule<UAssetCollection_Trade>(Context);
	if (!IsValid(AssetCollection))
	{
		return;
	}

	FPrimaryAssetId CraftAssetId = Asset->GetPrimaryAssetId();
	const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetList = AssetCollection->GetAssetList();

	if (QuerySource == ECraftQuerySource::Glossary)
	{
		HandleGlossaryItems(AssetList, CraftAssetId, CollectionId, Context, CraftStorage, MoveTemp(Callback));
	}
	else
	{
		HandleStorageItems(AssetList, CraftAssetId, CollectionId, Context, CraftStorage, MoveTemp(Callback));
	}
}



const UAssetCollection* UCraftSubsystem::GetMaterialCollection(const URPrimaryDataAsset* Asset, const FInstancedStruct& Context) const
{
	const ICraftProviderInterface* Provider = Cast<ICraftProviderInterface>(Asset);
	if (!Provider)
	{
		return nullptr;
	}
	return Provider->GetCraftingMaterial(Context);
}

const UAssetCollection* UCraftSubsystem::GetMaterialCollection(const URPrimaryDataAsset* Asset, const FGuid& CollectionId) const
{
	return GetMaterialCollection(Asset, FInstancedStruct::Make(FAssetRuleContext(CollectionId)));
}




void UCraftSubsystem::HandleGlossaryItems(const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback)
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetKv : AssetList)
	{
		const URPrimaryDataAsset* ItemDataAsset = AssetKv.Key;
		FAssetDetail_Trade ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, Context);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FTradeKey TradeKey(CraftAssetId, CollectionId, ItemAssetId);
		const FCraftData* CraftData = CraftStorage->GetItem(TradeKey);
		if (CraftData)
		{
			ItemDetail.Quota = FMath::Max(0, ItemDetail.Quota - CraftData->PendingQuantity);
		}

		Callback(ItemAssetId, ItemDetail, CraftData);
	}
}

void UCraftSubsystem::HandleStorageItems(const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback)
{
	for (const TPair<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetKv : AssetList)
	{
		const URPrimaryDataAsset* ItemDataAsset = AssetKv.Key;
		FAssetDetail_Trade ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, Context);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FTradeKey TradeKey(CraftAssetId, CollectionId, ItemAssetId);
		const FCraftData* CraftData = CraftStorage->GetItem(TradeKey);
		if (!CraftData)
		{
			continue;
		}

		ItemDetail.Quota = CraftData->PendingQuantity;

		Callback(ItemAssetId, ItemDetail, CraftData);
	}
}


void UCraftSubsystem::OnPreGameInitialized()
{
	IStorageProviderInterface* StorageInterface = IStorageProviderInterface::Get(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogShop, TEXT("Storage subsystem not found"));
		return;
	}

	FStorageHandle Handle;
	Handle.StorageClass = UCraftSubsystem::GetStorageClass();
	Handle.StorageId = UCraftSubsystem::GetStorageId();
	Handle.Url = UCraftSubsystem::GetStorageUrl();

	StorageInterface->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProviderInterface>(StorageInterface);
}



bool UCraftSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UCraftSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("CraftSubsystem initialized"));

	FLatentDelegate::OnPreGameInitialized.AddUObject(this, &UCraftSubsystem::OnPreGameInitialized);
}

void UCraftSubsystem::Deinitialize()
{
	StorageProvider.Reset();
	FLatentDelegate::OnPreGameInitialized.RemoveAll(this);

	LOG_WARNING(LogTemp, TEXT("CraftSubsystem deinitialized"));
	Super::Deinitialize();
}



UCraftSubsystem* UCraftSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UCraftSubsystem* UCraftSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UCraftSubsystem>();
}



FGuid UCraftSubsystem::GetStorageId()
{
	return FGuid(TEXT("953B0E29-3DE3-494B-8580-0D868D22D360"));
}

FString UCraftSubsystem::GetStorageUrl()
{
	return TEXT("/api/get/craft");
}

TSubclassOf<UStorage> UCraftSubsystem::GetStorageClass()
{
	return UCraftStorage::StaticClass();
}

