// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Interface/IAssetInstance.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UStorage;
class UInventoryStorage;
class IStorageProvider;



/**
 *
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem, public IAssetInstanceCollectionProvider
{

	GENERATED_BODY()

public:

	RSYSTEM_API UInventoryStorage* GetInventory(const FName& InventoryId) const;

	// ~ IAssetInstanceCollectionProvider
	RSYSTEM_API virtual IAssetInstanceCollection* GetInstanceCollection(const FName& SourceId) const override;
	RSYSTEM_API virtual FPrimaryAssetType GetSupportedAssetType() const override;
	RSYSTEM_API virtual FName GetDefaultCollectionId() const override;
	// ~ End of IAssetInstanceCollectionProvider

protected:

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static RSYSTEM_API UInventorySubsystem* Get(UWorld* World);
	static RSYSTEM_API UInventorySubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef RSYSTEM_API

