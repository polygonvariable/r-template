// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Item/InventoryInstance.h"

// Engine Headers

// Project Headers



bool UInventoryInstance::InitializeItem(const FGuid& InInventoryId, const FPrimaryAssetId& InAssetId, FInventoryItem InItem)
{
	if (bIsInitialized)
	{
		return false;
	}

	bIsInitialized = true;

	InventoryId = InInventoryId;
	AssetId = InAssetId;
	Item = InItem;

	return true;
}

void UInventoryInstance::ResetItem()
{
	bIsInitialized = false;

	InventoryId.Invalidate();
	Item.Reset();
	AssetId = FPrimaryAssetId();
}

int UInventoryInstance::GetQuantity() const
{
	return Item.Quantity;
}

void UInventoryInstance::SetQuantity(int InQuantity)
{
	Item.Quantity = FMath::Max(0, InQuantity);
	OnItemUpdated.Broadcast();
}

FGuid UInventoryInstance::GetInventoryId() const
{
	return InventoryId;
}

FPrimaryAssetId UInventoryInstance::GetAssetId() const
{
	return AssetId;
}

FGuid UInventoryInstance::GetItemId() const
{
	return Item.ItemId;
}
