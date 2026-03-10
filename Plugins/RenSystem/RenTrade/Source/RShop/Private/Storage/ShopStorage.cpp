// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/ShopStorage.h"

// Engine Headers

// Project Headers



const FShopData* UShopStorage::GetItem(const FTradeKey& TradeKey) const
{
	return ShopItems.Find(TradeKey);
}

bool UShopStorage::AddItem(const FTradeKey& TradeKey)
{
	FShopData* FoundData = ShopItems.Find(TradeKey);
	if (!FoundData)
	{
		ShopItems.Add(TradeKey, FShopData(1));
	}
	else
	{
		FoundData->PurchaseCount++;
	}

	OnShopUpdated.Broadcast();
	return true;
}

void UShopStorage::ResetItems()
{
	ShopItems.Empty();
	OnShopUpdated.Broadcast();
}

