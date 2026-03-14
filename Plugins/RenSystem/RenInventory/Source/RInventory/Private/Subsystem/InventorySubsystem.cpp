// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventorySubsystem.h"

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Delegate/LatentDelegate.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/InventorySettings.h"
#include "Storage/InventoryStorage.h"




UInventoryStorage* UInventorySubsystem::GetInventory(const FName& InventoryId) const
{
	IStorageProvider* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}
	return StorageInterface->GetStorage<UInventoryStorage>(InventoryId);
}

IAssetInstanceCollection* UInventorySubsystem::GetInstanceCollection(const FName& SourceId) const
{
	return Cast<IAssetInstanceCollection>(GetInventory(SourceId));
}

FPrimaryAssetType UInventorySubsystem::GetSupportedAssetType() const
{
	return UInventoryAsset::GetPrimaryAssetType();
}

FName UInventorySubsystem::GetDefaultCollectionId() const
{
	return UInventorySettings::Get()->StorageId;
}



void UInventorySubsystem::OnPreGameInitialized()
{
	IStorageProvider* StorageInterface = IStorageProvider::Get(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogInventory, TEXT("Storage subsystem not found"));
		return;
	}

	const UInventorySettings* Settings = UInventorySettings::Get();

	FStorageHandle Handle;
	Handle.StorageClass = Settings->StorageClass;
	Handle.StorageId = Settings->StorageId;
	Handle.Url = Settings->StorageUrl;

	StorageInterface->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UInventorySettings::Get()->SubsystemClass;
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

