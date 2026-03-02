// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "InventoryInstanceSubsystem.generated.h"

// Forward Declarations
class UInventoryInstance;
class UInventorySubsystem;



/**
 *
 */
USTRUCT()
struct FInventoryInstanceId
{

	GENERATED_BODY()

public:

	FInventoryInstanceId() {}
	FInventoryInstanceId(const FGuid& InInventoryId, const FPrimaryAssetId& InAssetId) : InventoryId(InInventoryId), AssetId(InAssetId) {}

	FGuid InventoryId;
	FPrimaryAssetId AssetId;

	friend inline bool operator == (const FInventoryInstanceId& A, const FInventoryInstanceId& B)
	{
		return A.AssetId == B.AssetId && A.InventoryId == B.InventoryId;
	}

	friend inline uint32 GetTypeHash(const FInventoryInstanceId& InstanceId)
	{
		return HashCombine(GetTypeHash(InstanceId.InventoryId), GetTypeHash(InstanceId.AssetId));
	}

};



/**
 *
 */
UCLASS()
class UInventoryInstanceSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	UInventoryInstance* GetItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);
	bool RemoveItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);

	void CommitAllItems();
	bool CommitItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);

protected:

	UPROPERTY()
	TMap<FInventoryInstanceId, TObjectPtr<UInventoryInstance>> InventoryInstances;

	UPROPERTY()
	UAssetManager* AssetManager = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> WeakInventory = nullptr;


	UInventoryInstance* CreateItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);
	bool CommitItem_Internal(UInventoryInstance* Item, UInventorySubsystem* InventorySubsystem);

	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UInventoryInstanceSubsystem* Get(UWorld* World);
	static UInventoryInstanceSubsystem* Get(UGameInstance* GameInstance);

};

