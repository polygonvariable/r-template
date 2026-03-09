// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"

// Generated Headers
#include "InventoryItem.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{

	GENERATED_BODY()

public:

	FInventoryItem() {}
	FInventoryItem(const FGuid& InItemId, int InQuantity) : ItemId(InItemId), Quantity(InQuantity) {}


	UPROPERTY(SaveGame)
	FGuid ItemId;

	UPROPERTY(SaveGame)
	int Quantity = 0;

	UPROPERTY(SaveGame)
	FAscensionData Ascension;


	bool IsValid() const
	{
		return ItemId.IsValid() && Quantity > 0 && Ascension.IsValid();
	}

	void Reset()
	{
		ItemId.Invalidate();
		Quantity = 0;
		Ascension.Reset();
	}

	void Sanitize()
	{
		Quantity = FMath::Max(0, Quantity);
		Ascension.Sanitize();
	}

	friend inline bool operator == (const FInventoryItem& A, const FInventoryItem& B)
	{
		return A.ItemId == B.ItemId && A.Quantity == B.Quantity;
	}

	friend inline uint32 GetTypeHash(const FInventoryItem& Record)
	{
		return HashCombine(GetTypeHash(Record.ItemId), GetTypeHash(Record.Quantity));
	}

	FString ToString() const
	{
		FString Detail = TEXT("Item Id: ") + ItemId.ToString();
		Detail += TEXT("\nQuantity: ") + FString::FromInt(Quantity);
		Detail += TEXT("\nAscension: ") + Ascension.ToString();
		return Detail;
	}

};

