// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/InventorySortType.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "InventoryQueryType.generated.h"

// Forward Declarations



/**
 *
 * 
 */
UENUM(BlueprintType)
enum class EInventoryQuerySource : uint8
{
	Inventory UMETA(DisplayName = "Inventory"),
	Glossary UMETA(DisplayName = "Glossary"),
};



/**
 *
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryQueryRule
{

	GENERATED_BODY()

public:

	FInventoryQueryRule() {}

	UPROPERTY(EditAnywhere)
	EInventoryQuerySource QuerySource = EInventoryQuerySource::Inventory;

	UPROPERTY(EditAnywhere)
	ESortDirection SortDirection = ESortDirection::Ascending;

	UPROPERTY(EditAnywhere)
	EInventorySortType SortType = EInventorySortType::Alphabetical;

};

