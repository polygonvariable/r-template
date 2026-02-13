// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "InstancedStruct.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"

// Generated Headers
#include "InventoryRecord.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{

	GENERATED_BODY()

public:

	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid ItemId;

	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAscensionData Ascension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInstancedStruct Metadata;

	bool IsValid() const
	{
		return ItemId.IsValid() && Quantity > 0;
	}

	friend inline bool operator == (const FInventoryItem& A, const FInventoryItem& B)
	{
		return A.ItemId == B.ItemId && A.Quantity == B.Quantity;
	}

	friend inline uint32 GetTypeHash(const FInventoryItem& Record)
	{
		return HashCombine(GetTypeHash(Record.ItemId.ToString()), GetTypeHash(Record.Quantity));
	}

};

