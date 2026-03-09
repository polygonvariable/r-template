// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "AssetTransactionInterface.generated.h"

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations



UINTERFACE(MinimalAPI)
class UAssetTransactionInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORE_API IAssetTransactionInterface
{

	GENERATED_BODY()

public:

	virtual bool AddItem(const FPrimaryAssetId& AssetId, int Quantity) = 0;
	virtual bool AddItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) = 0;

	virtual bool RemoveItem(const FPrimaryAssetId& AssetId, int Quantity) = 0;
	virtual bool RemoveItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) = 0;

	virtual bool RemoveAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) = 0;
	virtual bool RemoveItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity) = 0;

	virtual bool ContainItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const = 0;
	virtual bool ContainAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const = 0;


};




UINTERFACE(MinimalAPI)
class UAssetInterchangeInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORE_API IAssetInterchangeInterface
{

	GENERATED_BODY()

public:

	virtual IAssetTransactionInterface* GetTransactionSource(const FGuid& SourceId) const = 0;
	virtual FPrimaryAssetType GetSupportedAssetType() const = 0;
	virtual FGuid GetDefaultSourceId() const = 0;

};


// Module Macros
#undef RCORE_API

