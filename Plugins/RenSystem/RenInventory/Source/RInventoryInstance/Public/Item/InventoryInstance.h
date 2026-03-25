// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/InventoryInstance.h"

// Generated Headers
#include "InventoryInstance.generated.h"

// Forward Declarations
class UInventorySubsystem;



/**
 *
 */
UCLASS()
class UInventoryInstance : public UObject
{

	GENERATED_BODY()

public:

    bool InitializeItem(const FGuid& InInventoryId, const FPrimaryAssetId& InAssetId, FInventoryInstance InItem);
    void ResetItem();

    int GetQuantity() const;
    void SetQuantity(int InQuantity);

    FGuid GetInventoryId() const;
	FPrimaryAssetId GetAssetId() const;
	FGuid GetItemId() const;

    UPROPERTY()
    TObjectPtr<UInventoryInstance> NextNode = nullptr;


    DECLARE_MULTICAST_DELEGATE(FOnItemUpdated);
    FOnItemUpdated OnItemUpdated;

protected:

    bool bIsInitialized = false;

    FGuid InventoryId;
    FPrimaryAssetId AssetId;
    FInventoryInstance Item;

};

