// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarSubsystem.h"

// Engine Headers

// Project Headers
#include "Asset/AvatarAsset.h"
#include "Delegate/GameLifecycleDelegates.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/AvatarSettings.h"
#include "Storage/AvatarStorage.h"




UAvatarStorage* UAvatarSubsystem::GetAvatarCollection() const
{
	IStorageProvider* Storage = StorageProvider.Get();
	if (!Storage)
	{
		return nullptr;
	}

	FName StorageId = UAvatarSettings::Get()->StorageId;
	UStorage* AvatarStorage = Storage->GetStorage(StorageId);

	return Cast<UAvatarStorage>(AvatarStorage);
}

IAssetInstanceCollection* UAvatarSubsystem::GetInstanceCollection(const FName& CollectionId) const
{
	return Cast<IAssetInstanceCollection>(GetAvatarCollection());
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
	IStorageProvider* StorageInterface = IStorageProvider::Get(GetGameInstance());
	if (StorageInterface)
	{
		StorageInterface->LoadStorageFromSettings(UAvatarSettings::Get());
		StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
	}
}

bool UAvatarSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UAvatarSettings::Get()->SubsystemClass;
}

void UAvatarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem initialized"));

	FGameLifecycleDelegates::OnPreGameInitialized.AddUObject(this, &UAvatarSubsystem::OnPreGameInitialized);
}

void UAvatarSubsystem::Deinitialize()
{
	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);
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

