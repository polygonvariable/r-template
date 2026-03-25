// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/EquipmentInstance.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "EquipmentStorage.generated.h"

// Module Macros
#define RSYSTEM_API REQUIPMENT_API

// Forward Declarations
struct FGameplayTag;



/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentStorage : public UStorage
{

	GENERATED_BODY()

public:

	// ~ UStorage
	RSYSTEM_API virtual void InitializeDefaults();
	// ~ End of UStorage


	bool EquipmentHasOwner(const FGuid& EquipmentId) const;
	FGuid GetEquipmentOwner(const FGuid& EquipmentId) const;

	const TMap<FGameplayTag, FEquipmentKey>* GetOwnedEquipment(const FGuid& OwnerId) const;

	RSYSTEM_API bool GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FPrimaryAssetId& OutEquipmentAssetId) const;
	RSYSTEM_API bool GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FGuid& OutEquipmentId, FPrimaryAssetId& OutEquipmentAssetId) const;
	RSYSTEM_API bool SetEquipmentAtSlot(const FGuid& OwnerId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& EquipmentSlot, const FGuid& EquipmentId, const FPrimaryAssetId& EquipmentAssetId);
	RSYSTEM_API bool RemoveEquipmentAtSlot(const FGuid& OwnerId, const FGameplayTag& SlotTag);

protected:

	UPROPERTY(SaveGame)
	TMap<FGuid, FEquipmentInstance> EquipmentInstances; // InstanceId -> Equipment Instance

	UPROPERTY(SaveGame)
	TMap<FGuid, FGuid> EquipmentRelation; // EquipmentId -> InstanceId

};


// Module Macros
#undef RSYSTEM_API

