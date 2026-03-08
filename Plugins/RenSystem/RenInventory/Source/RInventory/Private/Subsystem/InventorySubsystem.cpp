// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventorySubsystem.h"

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Delegate/LatentDelegate.h"
#include "Interface/StorageProviderInterface.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/InventoryStorage.h"




UInventoryStorage* UInventorySubsystem::GetInventory(const FGuid& InventoryId) const
{
	IStorageProviderInterface* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}
	return StorageInterface->GetStorage<UInventoryStorage>(InventoryId);
}

IAssetTransactionInterface* UInventorySubsystem::GetTransactionSource(const FGuid& SourceId) const
{
	UInventoryStorage* Storage = GetInventory(SourceId);
	IAssetTransactionInterface* Source = Cast<IAssetTransactionInterface>(Storage);

	return Source;
}

FPrimaryAssetType UInventorySubsystem::GetSupportedAssetType() const
{
	return UInventoryAsset::GetPrimaryAssetType();
}

FGuid UInventorySubsystem::GetDefaultSourceId() const
{
	return UInventorySubsystem::GetStorageId();
}



void UInventorySubsystem::OnPreGameInitialized()
{
	IStorageProviderInterface* StorageInterface = IStorageProviderInterface::Get(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogInventory, TEXT("Storage subsystem not found"));
		return;
	}

	FStorageHandle Handle;
	Handle.StorageClass = UInventorySubsystem::GetStorageClass();
	Handle.StorageId = UInventorySubsystem::GetStorageId();
	Handle.Url = UInventorySubsystem::GetStorageUrl();

	StorageInterface->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProviderInterface>(StorageInterface);
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventory, TEXT("InventorySubsystem initialized"));

	FLatentDelegate::OnPreGameInitialized.AddUObject(this, &UInventorySubsystem::OnPreGameInitialized);
}

void UInventorySubsystem::Deinitialize()
{
	StorageProvider.Reset();

	FLatentDelegate::OnPreGameInitialized.RemoveAll(this);

	LOG_WARNING(LogInventory, TEXT("InventorySubsystem deinitialized"));
	Super::Deinitialize();
}



UInventorySubsystem* UInventorySubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UInventorySubsystem* UInventorySubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventorySubsystem>();
}

FGuid UInventorySubsystem::GetStorageId()
{
	return FGuid(TEXT("24D61E46-5B41-49A4-BDFB-B1E39BF54665"));
}

FString UInventorySubsystem::GetStorageUrl()
{
	return TEXT("/api/get/inventory");
}

TSubclassOf<UStorage> UInventorySubsystem::GetStorageClass()
{
	return UInventoryStorage::StaticClass();
}

