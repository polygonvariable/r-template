// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/ShopData.h"
#include "Definition/Runtime/TradeKey.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "ShopStorage.generated.h"

// Module Macros
#define RSYSTEM_API RSHOP_API

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UShopStorage : public UStorage
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnShopUpdated);
	FOnShopUpdated OnShopUpdated;


	RSYSTEM_API const FShopData* GetItem(const FTradeKey& TradeKey) const;
	RSYSTEM_API bool AddItem(const FTradeKey& TradeKey);
	RSYSTEM_API void ResetItems();

protected:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FShopData> ShopItems;

};



// Module Macros
#undef RSYSTEM_API

