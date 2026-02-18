// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers

// Forward Declarations

// Module Macros
#ifdef RSYSTEM_API
#undef RSYSTEM_API
#endif
#define RSYSTEM_API RINVENTORY_API



/**
 *
 */
class RSYSTEM_API InventoryFilterProperty
{

public:

	static const FName AssetId;
	static const FName AssetType;
	static const FName AssetRarity;

	static const FName ItemId;
	static const FName ItemQuantity;
	static const FName ItemExperience;
	static const FName ItemLevel;
	static const FName ItemRank;

};



// Module Macros
#undef RSYSTEM_API

