// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Item/ItemInstance.h"

// Engine Headers

// Project Headers



bool UItemInstance::InitializeItem(const FGuid& InInventoryId, const FPrimaryAssetId& InAssetId, FInventoryItem InItem)
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

void UItemInstance::ResetItem()
{
	bIsInitialized = false;

	InventoryId.Invalidate();
	Item.Reset();
	AssetId = FPrimaryAssetId();
}

int UItemInstance::GetQuantity() const
{
	return Item.Quantity;
}

void UItemInstance::SetQuantity(int InQuantity)
{
	Item.Quantity = FMath::Max(0, InQuantity);
	OnItemUpdated.Broadcast();
}

FGuid UItemInstance::GetInventoryId() const
{
	return InventoryId;
}

FPrimaryAssetId UItemInstance::GetAssetId() const
{
	return AssetId;
}

FGuid UItemInstance::GetItemId() const
{
	return Item.ItemId;
}
