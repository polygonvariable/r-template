// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/InventoryItem.h"

// Generated Headers
#include "InventoryStack.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryStack
{

	GENERATED_BODY()

public:

	FInventoryStack() {}
	FInventoryStack(FGuid InStackId, bool bInStackable, bool bInPersistWhenEmpty) : StackId(InStackId), bStackable(bInStackable), bPersistWhenEmpty(bInPersistWhenEmpty) {}


	UPROPERTY(SaveGame)
	TArray<FInventoryItem> ItemList;

	UPROPERTY(SaveGame)
	FGuid StackId;

	UPROPERTY(SaveGame)
	bool bStackable = false;

	UPROPERTY(SaveGame)
	bool bPersistWhenEmpty = false;


	friend inline bool operator == (const FInventoryStack& A, const FInventoryStack& B)
	{
		return A.StackId == B.StackId;
	}

	friend inline uint32 GetTypeHash(const FInventoryStack& A)
	{
		return GetTypeHash(A.StackId.ToString());
	}

};

