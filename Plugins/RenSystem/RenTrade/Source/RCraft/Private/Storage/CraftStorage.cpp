// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/CraftStorage.h"

// Engine Headers

// Project Headers



const FCraftData* UCraftStorage::GetItem(const FTradeKey& TradeKey) const
{
	const FCraftData* FoundData = CraftItems.Find(TradeKey);
	return FoundData;
}

bool UCraftStorage::AddItem(const FTradeKey& TradeKey, FTimespan BatchProcessingTime)
{
	FCraftData* FoundData = CraftItems.Find(TradeKey);
	if (FoundData)
	{
		FoundData->PendingQuantity++;
	}
	else
	{
		CraftItems.Add(TradeKey, FCraftData(1, FDateTime::Now(), BatchProcessingTime));
	}

	OnCraftUpdated.Broadcast();
	return true;
}

void UCraftStorage::ResetItems()
{
	CraftItems.Empty();
	OnCraftUpdated.Broadcast();
}

int UCraftStorage::ClaimCraftedItems(const FTradeKey& TradeKey)
{
	FCraftData* FoundData = CraftItems.Find(TradeKey);
	if (!FoundData)
	{
		return 0;
	}

	int TotalQuantity = FoundData->PendingQuantity;
	FDateTime BatchTime = FoundData->BatchStartTimestamp;
	FTimespan BatchDuration = FoundData->BatchProcessingTime;

	FDateTime CurrentTime = FDateTime::Now();

	FTimespan Elapsed = CurrentTime - BatchTime;

	int Completed = 0;

	if (BatchDuration.GetTicks() > 0)
	{
		Completed = Elapsed.GetTicks() / BatchDuration.GetTicks();
	}

	int AvailableQuantity = FMath::Clamp(Completed, 0, TotalQuantity);
	if (AvailableQuantity > 0)
	{
		FoundData->BatchStartTimestamp = BatchTime + (BatchDuration * AvailableQuantity);
	}

	FoundData->PendingQuantity -= AvailableQuantity;
	FoundData->Sanitize();

	if (FoundData->PendingQuantity <= 0)
	{
		CraftItems.Remove(TradeKey);
	}

	if (AvailableQuantity > 0)
	{
		OnCraftUpdated.Broadcast();
	}

	return AvailableQuantity;
}
