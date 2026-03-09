// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SaveGame/Storage.h"

// Project Headers
#include "Definition/Runtime/CraftData.h"
#include "Definition/Runtime/TradeKey.h"

// Generated Headers
#include "CraftStorage.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS()
class UCraftStorage : public UStorage
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnCraftUpdated);
	FOnCraftUpdated OnCraftUpdated;


	const FCraftData* GetItem(const FTradeKey& TradeKey) const;
	bool AddItem(const FTradeKey& TradeKey, FTimespan BatchProcessingTime);
	void ResetItems();

	int ClaimCraftedItems(const FTradeKey& TradeKey);

protected:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FCraftData> CraftItems;

};

