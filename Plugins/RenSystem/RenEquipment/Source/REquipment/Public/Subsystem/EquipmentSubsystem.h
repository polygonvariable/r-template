// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "EquipmentSubsystem.generated.h"

// Module Macros
#define RSYSTEM_API REQUIPMENT_API

// Forward Declarations
class IStorageProvider;
class UEquipmentStorage;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UEquipmentSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSyncEquipment, const FGuid& /* Owner Id */);
	FOnSyncEquipment OnSyncEquipment;

	RSYSTEM_API void SyncEquipment(const FGuid& OwnerId) const;
	RSYSTEM_API UEquipmentStorage* GetEquipment() const;

protected:

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static RSYSTEM_API UEquipmentSubsystem* Get(UWorld* World);
	static RSYSTEM_API UEquipmentSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef RSYSTEM_API

