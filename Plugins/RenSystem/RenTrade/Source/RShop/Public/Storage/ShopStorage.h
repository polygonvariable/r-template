// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SaveGame/Storage.h"

// Project Headers
#include "Definition/Runtime/ShopRuntime.h"

// Generated Headers
#include "ShopStorage.generated.h"



/**
 *
 */
UCLASS()
class UShopStorage : public UStorage
{

	GENERATED_BODY()

public:

	bool GetItem(const FShopKey& ShopKey, FShopData& OutShopData);
	bool AddItem(const FShopKey& ShopKey);
	//int RemoveAvailableItems(FTradeKey TradeKey);

	void ResetItems();

protected:

	UPROPERTY(SaveGame)
	TMap<FShopKey, FShopData> ShopItems;

};

