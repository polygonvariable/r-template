// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarSubsystem.h"

// Engine Headers

// Project Headers
#include "LatentDelegates.h"
#include "StorageProviderInterface.h"
#include "LogMacro.h"
#include "LogCategory.h"
#include "SubsystemUtils.h"
#include "AvatarAsset.h"

#include "Storage/EntityStorage.h"
#include "RenEntity/Public/EntityPrimaryAsset.h"




TMap<FPrimaryAssetId, FAvatarRecord>* UAvatarSubsystem::GetMutableAvatarRecords() const
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return nullptr;
	}
	return &Storage->Records;
}




const TMap<FPrimaryAssetId, FAvatarRecord>* UAvatarSubsystem::GetAvatarRecords() const
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return nullptr;
	}
	return &Storage->Records;
}

const FAvatarRecord* UAvatarSubsystem::GetAvatarRecord(const FPrimaryAssetId& AvatarId) const
{
	const TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return nullptr;
	}

	return Records->Find(AvatarId);
}

FAvatarRecord* UAvatarSubsystem::GetMutableAvatarRecord(const FPrimaryAssetId& AvatarId) const
{
	TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return nullptr;
	}

	return Records->Find(AvatarId);
}

bool UAvatarSubsystem::AddAvatar(const FPrimaryAssetId& AvatarId)
{
	TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
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

	FAvatarRecord NewRecord;

	Records->Add(AvatarId, NewRecord);

	return true;
}

bool UAvatarSubsystem::RemoveAvatar(const FPrimaryAssetId& AvatarId)
{
	TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
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

	return true;
}

bool UAvatarSubsystem::UpdateAvatar(const FPrimaryAssetId& AvatarId, const FAvatarRecord& AvatarRecord)
{
	FAvatarRecord* Record = GetMutableAvatarRecord(AvatarId);
	if (!Record)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar not found"));
		return false;
	}

	Record->Health = AvatarRecord.Health;
	Record->Enhance = AvatarRecord.Enhance;
	Record->Sanitize();

	return true;
}

bool UAvatarSubsystem::UpdateAvatar(const FPrimaryAssetId& AvatarId, TFunctionRef<bool(FAvatarRecord*)> Callback)
{
	FAvatarRecord* Record = GetMutableAvatarRecord(AvatarId);
	if (!Callback(Record))
	{
		LOG_ERROR(LogAvatar, TEXT("Record update callback returned false"));
		return false;
	}

	return true;
}







void UAvatarSubsystem::LoadAvatarStorage()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<IStorageProviderInterface>(GetGameInstance());
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
	AvatarStorage = TWeakObjectPtr<UAvatarStorage>(Storage);

	LOG_INFO(LogAvatar, TEXT("Storage loaded"));
}

void UAvatarSubsystem::SaveAvatarStorage()
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	IStorageProviderInterface* StorageProvider = StorageProviderInterface.Get();
	if (StorageProvider || IsValid(Storage))
	{
		StorageProvider->SaveStorage(Storage, TEXT("/get/avatars"));
	}

	AvatarStorage.Reset();
	StorageProviderInterface.Reset();
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

