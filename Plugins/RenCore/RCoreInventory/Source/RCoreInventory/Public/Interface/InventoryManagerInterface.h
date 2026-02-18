// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "InventoryManagerInterface.generated.h"

// Forward Declarations
class UInventoryAsset;



UINTERFACE(MinimalAPI)
class UInventoryManagerInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCOREINVENTORY_API IInventoryManagerInterface
{

	GENERATED_BODY()

public:

	virtual bool ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& InItems, bool bUseOr) const = 0;

};

