// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "ItemInstanceSubsystem.generated.h"

// Forward Declarations
class UItemInstance;
class UInventorySubsystem;



/**
 *
 */
USTRUCT()
struct FItemInstanceId
{

	GENERATED_BODY()

public:

	FItemInstanceId() {}
	FItemInstanceId(const FGuid& InInventoryId, const FPrimaryAssetId& InAssetId) : InventoryId(InInventoryId), AssetId(InAssetId) {}

	FGuid InventoryId;
	FPrimaryAssetId AssetId;

	friend inline bool operator == (const FItemInstanceId& A, const FItemInstanceId& B)
	{
		return A.AssetId == B.AssetId && A.InventoryId == B.InventoryId;
	}

	friend inline uint32 GetTypeHash(const FItemInstanceId& InstanceId)
	{
		return HashCombine(GetTypeHash(InstanceId.InventoryId), GetTypeHash(InstanceId.AssetId));
	}

};



/**
 *
 */
UCLASS()
class UItemInstanceSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	UItemInstance* GetItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);
	bool RemoveItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);

	void CommitAllItems();
	bool CommitItem(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);

protected:

	UPROPERTY()
	TMap<FItemInstanceId, TObjectPtr<UItemInstance>> ItemInstances;

	UPROPERTY()
	UAssetManager* AssetManager = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> WeakInventory = nullptr;


	UItemInstance* CreateItem_Internal(const FGuid& InventoryId, const FPrimaryAssetId& AssetId);
	bool CommitItem_Internal(UItemInstance* Item, UInventorySubsystem* InventorySubsystem);

	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UItemInstanceSubsystem* Get(UWorld* World);
	static UItemInstanceSubsystem* Get(UGameInstance* GameInstance);

};

