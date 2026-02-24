// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarSubsystem.h"

// Engine Headers

// Project Headers
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "AvatarAsset.h"
#include "LatentDelegates.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Interface/StorageProviderInterface.h"
#include "Util/SubsystemUtil.h"
#include "EntityPrimaryAsset.h"
#include "Storage/EntityStorage.h"



TMap<FPrimaryAssetId, FAvatarData>* UAvatarSubsystem::GetMutableAvatarCollection() const
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return nullptr;
	}
	return &Storage->AvatarCollection;
}

const TMap<FPrimaryAssetId, FAvatarData>* UAvatarSubsystem::GetAvatarCollection() const
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return nullptr;
	}
	return &Storage->AvatarCollection;
}

const FAvatarData* UAvatarSubsystem::GetAvatarData(const FPrimaryAssetId& AvatarId) const
{
	const TMap<FPrimaryAssetId, FAvatarData>* Records = GetMutableAvatarCollection();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return nullptr;
	}

	return Records->Find(AvatarId);
}

FAvatarData* UAvatarSubsystem::GetMutableAvatarData(const FPrimaryAssetId& AvatarId) const
{
	TMap<FPrimaryAssetId, FAvatarData>* Records = GetMutableAvatarCollection();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return nullptr;
	}

	return Records->Find(AvatarId);
}


bool UAvatarSubsystem::AddAvatar(const FPrimaryAssetId& AvatarId)
{
	TMap<FPrimaryAssetId, FAvatarData>* Records = GetMutableAvatarCollection();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return false;
	}

	if (Records->Contains(AvatarId))
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar already exists"));
		return false;
	}

	Records->Add(AvatarId, FAvatarData());
	OnAvatarAdded.Broadcast(AvatarId);
	return true;
}

bool UAvatarSubsystem::RemoveAvatar(const FPrimaryAssetId& AvatarId)
{
	TMap<FPrimaryAssetId, FAvatarData>* Records = GetMutableAvatarCollection();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return false;
	}

	if (!Records->Contains(AvatarId))
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar not found"));
		return false;
	}

	Records->Remove(AvatarId);
	OnAvatarRemoved.Broadcast(AvatarId);
	return true;
}

bool UAvatarSubsystem::UpdateAvatar(const FPrimaryAssetId& AvatarId, TFunctionRef<bool(FAvatarData*)> Callback)
{
	FAvatarData* Record = GetMutableAvatarData(AvatarId);
	if (!Callback(Record))
	{
		LOG_ERROR(LogAvatar, TEXT("Record update callback returned false"));
		return false;
	}

	OnAvatarUpdated.Broadcast(AvatarId);
	return true;
}









void UAvatarSubsystem::LoadAvatarStorage()
{/*
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	IStorageProviderInterface* StorageProvider = SubsystemUtil::GetSubsystemInterface<IStorageProviderInterface>(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogAvatar, TEXT("Storage provider not found"));
		return;
	}

	UAvatarStorage* Storage = StorageProvider->GetStorage<UAvatarStorage>(TEXT("/get/avatars"));
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return;
	}

	StorageProviderInterface = TWeakInterfacePtr<IStorageProviderInterface>(StorageProvider);
	AvatarStorage = Storage;

	LOG_INFO(LogAvatar, TEXT("Storage loaded"));*/
}

void UAvatarSubsystem::SaveAvatarStorage()
{/*
	UAvatarStorage* Storage = AvatarStorage.Get();
	IStorageProviderInterface* StorageProvider = StorageProviderInterface.Get();
	if (StorageProvider || IsValid(Storage))
	{
		StorageProvider->SaveStorage(Storage, TEXT("/get/avatars"));
	}

	AvatarStorage = nullptr;
	StorageProviderInterface.Reset();*/
}

bool UAvatarSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAvatarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FLatentDelegates::OnStorageLoaded.AddUObject(this, &UAvatarSubsystem::LoadAvatarStorage);

	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem initialized"));
}

void UAvatarSubsystem::Deinitialize()
{
	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem deinitialized"));

	SaveAvatarStorage();

	Super::Deinitialize();
}

