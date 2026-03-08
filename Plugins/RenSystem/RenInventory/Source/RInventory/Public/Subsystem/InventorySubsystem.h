// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Interface/AssetTransactionInterface.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UStorage;
class UInventoryStorage;
class IStorageProviderInterface;



/**
 *
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem, public IAssetInterchangeInterface
{

	GENERATED_BODY()

public:

	RSYSTEM_API UInventoryStorage* GetInventory(const FGuid& InventoryId) const;

	// ~ IAssetInterchangeInterface
	virtual IAssetTransactionInterface* GetTransactionSource(const FGuid& SourceId) const override;
	virtual FPrimaryAssetType GetSupportedAssetType() const override;
	virtual FGuid GetDefaultSourceId() const override;
	// ~ End of IAssetInterchangeInterface

protected:

	TWeakInterfacePtr<IStorageProviderInterface> StorageProvider;

	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem


public:

	static RSYSTEM_API UInventorySubsystem* Get(UWorld* World);
	static RSYSTEM_API UInventorySubsystem* Get(UGameInstance* GameInstance);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static RSYSTEM_API FGuid GetStorageId();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static RSYSTEM_API FString GetStorageUrl();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static RSYSTEM_API TSubclassOf<UStorage> GetStorageClass();

};



// Module Macros
#undef RSYSTEM_API

