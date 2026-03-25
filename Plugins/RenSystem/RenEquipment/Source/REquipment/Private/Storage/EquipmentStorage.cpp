// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/EquipmentStorage.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"



void UEquipmentStorage::InitializeDefaults()
{
	EquipmentInstances = UEquipmentSettings::Get()->DefaultEquipment;
}


bool UEquipmentStorage::EquipmentHasOwner(const FGuid& EquipmentId) const
{
	return EquipmentRelation.Contains(EquipmentId);
}

FGuid UEquipmentStorage::GetEquipmentOwner(const FGuid& EquipmentId) const
{
	return EquipmentRelation.FindRef(EquipmentId);
}

const TMap<FGameplayTag, FEquipmentKey>* UEquipmentStorage::GetOwnedEquipment(const FGuid& OwnerId) const
{
	const FEquipmentInstance* Instance = EquipmentInstances.Find(OwnerId);
	if (!Instance)
	{
		return nullptr;
	}
	return &Instance->EquipmentSlot;
}

bool UEquipmentStorage::GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FPrimaryAssetId& OutEquipmentAssetId) const
{
	FGuid OutEquipmentId;
	return GetEquipmentAtSlot(InOwnerId, InEquipmentSlot, OutEquipmentId, OutEquipmentAssetId);
}

bool UEquipmentStorage::GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FGuid& OutEquipmentId, FPrimaryAssetId& OutEquipmentAssetId) const
{
	OutEquipmentId = FGuid();
	OutEquipmentAssetId = FPrimaryAssetId();

	const FEquipmentInstance* Instance = EquipmentInstances.Find(InOwnerId);
	if (!Instance)
	{
		return false;
	}
	
	const TMap<FGameplayTag, FEquipmentKey>& Slots = Instance->EquipmentSlot;
	const FEquipmentKey* FoundId = Slots.Find(InEquipmentSlot);
	if (!FoundId)
	{
		return false;
	}

	OutEquipmentId = FoundId->AssetInstanceId;
	OutEquipmentAssetId = FoundId->AssetId;
	return true;
}

bool UEquipmentStorage::SetEquipmentAtSlot(const FGuid& OwnerId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& EquipmentSlot, const FGuid& EquipmentId, const FPrimaryAssetId& EquipmentAssetId)
{
	if (!OwnerId.IsValid() || !OwnerAssetId.IsValid() || !EquipmentId.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Invalid parameters"));
		return false;
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();

	if (!Settings->EquipmentSlots.HasTagExact(EquipmentSlot))
	{
		LOG_ERROR(LogTemp, TEXT("Invalid equipment slot"));
		return false;
	}

	if (EquipmentAssetId.PrimaryAssetType != Settings->EquipmentType || OwnerAssetId.PrimaryAssetType != Settings->OwnerType)
	{
		LOG_ERROR(LogTemp, TEXT("Invalid equipment or owner type"));
		return false;
	}

	const FGuid* Found_OwnerId = EquipmentRelation.Find(EquipmentId);
	if (Found_OwnerId)
	{
		if (OwnerId != *Found_OwnerId)
		{
			LOG_ERROR(LogTemp, TEXT("Equipment is used by another instance"));
			return false;
		}
	}

	FEquipmentInstance& EquipmentInstance = EquipmentInstances.FindOrAdd(OwnerId);
	FPrimaryAssetId& InstanceAssetId = EquipmentInstance.OwnerAssetId;
	if (!InstanceAssetId.IsValid())
	{
		InstanceAssetId = OwnerAssetId;
	}

	FEquipmentKey ExistingKey(EquipmentAssetId, EquipmentId);
	TMap<FGameplayTag, FEquipmentKey>& Slots = EquipmentInstance.EquipmentSlot;

	FGameplayTag ToRemove;
	for (const TPair<FGameplayTag, FEquipmentKey>& Slot : Slots)
	{
		if (Slot.Value == ExistingKey)
		{
			ToRemove = Slot.Key;
			break;
		}
	}

	if (ToRemove.IsValid())
	{
		Slots.Remove(ToRemove);
	}

	Slots.Add(EquipmentSlot, FEquipmentKey(EquipmentAssetId, EquipmentId));
	EquipmentRelation.Add(EquipmentId, OwnerId);

	OnStorageUpdated.Broadcast();
	return true;
}

bool UEquipmentStorage::RemoveEquipmentAtSlot(const FGuid& OwnerId, const FGameplayTag& SlotTag)
{
	FEquipmentInstance* EquipmentInstance = EquipmentInstances.Find(OwnerId);
	if (!EquipmentInstance)
	{
		return false;
	}

	EquipmentInstance->EquipmentSlot.Remove(SlotTag);
	EquipmentRelation.Remove(OwnerId);

	OnStorageUpdated.Broadcast();
	return true;
}
