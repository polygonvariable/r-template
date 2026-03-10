// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/CraftData.h"
#include "Definition/Runtime/TradeKey.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "CraftStorage.generated.h"

// Module Macros
#define RSYSTEM_API RCRAFT_API

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UCraftStorage : public UStorage
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnCraftUpdated);
	FOnCraftUpdated OnCraftUpdated;


	RSYSTEM_API const FCraftData* GetItem(const FTradeKey& TradeKey) const;
	RSYSTEM_API bool AddItem(const FTradeKey& TradeKey, FTimespan BatchProcessingTime);
	RSYSTEM_API void ResetItems();

	RSYSTEM_API int ClaimCraftedItems(const FTradeKey& TradeKey);

protected:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FCraftData> CraftItems;

};



// Module Macros
#undef RSYSTEM_API

