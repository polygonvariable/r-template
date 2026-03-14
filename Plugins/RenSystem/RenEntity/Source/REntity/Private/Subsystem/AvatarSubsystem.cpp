// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarSubsystem.h"

// Engine Headers

// Project Headers
#include "Asset/AvatarAsset.h"
#include "Delegate/LatentDelegate.h"
#include "Interface/IStorageProvider.h"
#include "Library/EntityPrimaryAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Manager/RAssetManager.inl"
#include "SaveGame/Storage.h"
#include "Settings/AvatarSettings.h"
#include "Storage/AvatarStorage.h"
#include "Util/SubsystemUtil.h"




UAvatarStorage* UAvatarSubsystem::GetAvatarCollection(const FName& CollectionId) const
{
	IStorageProvider* Storage = StorageProvider.Get();
	if (!Storage)
	{
		return nullptr;
	}
	return Storage->GetStorage<UAvatarStorage>(CollectionId);
}

IAssetInstanceCollection* UAvatarSubsystem::GetInstanceCollection(const FName& CollectionId) const
{
	return Cast<IAssetInstanceCollection>(GetAvatarCollection(CollectionId));
}

FPrimaryAssetType UAvatarSubsystem::GetSupportedAssetType() const
{
	return UAvatarAsset::GetPrimaryAssetType();
}

FName UAvatarSubsystem::GetDefaultCollectionId() const
{
	return UAvatarSettings::Get()->StorageId;
}



void UAvatarSubsystem::OnPreGameInitialized()
{
	IStorageProvider* Storage = IStorageProvider::Get(GetGameInstance());
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Storage subsystem not found"));
		return;
	}

	const UAvatarSettings* Settings = UAvatarSettings::Get();
	
	FStorageHandle Handle;
	Handle.StorageClass = Settings->StorageClass;
	Handle.StorageId = Settings->StorageId;
	Handle.Url = Settings->StorageUrl;

	Storage->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProvider>(Storage);
}

bool UAvatarSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UAvatarSettings::Get()->SubsystemClass;
}

void UAvatarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem initialized"));

	FLatentDelegate::OnPreGameInitialized.AddUObject(this, &UAvatarSubsystem::OnPreGameInitialized);
}

void UAvatarSubsystem::Deinitialize()
{
	FLatentDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider.Reset();

	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem deinitialized"));
	Super::Deinitialize();
}



UAvatarSubsystem* UAvatarSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UAvatarSubsystem* UAvatarSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UAvatarSubsystem>();
}

