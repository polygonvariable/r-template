// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/InventoryItemRarity.h"
#include "Definition/InventoryItemType.h"

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
class RSYSTEM_API InventoryPrimaryAsset
{

public:

	static FPrimaryAssetType GetAssetType();
	static FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	static bool IsValid(const FPrimaryAssetId& AssetId);

	static bool GetType(const FAssetData& AssetData, FName& Type);
	static bool GetType(const FAssetData& AssetData, EInventoryItemType& Type);

	static bool GetRarity(const FAssetData& AssetData, FName& Rarity);
	static bool GetRarity(const FAssetData& AssetData, EInventoryItemRarity& Rarity);

	static bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	static bool GetStackable(const FAssetData& AssetData, bool& bStackable);
	static bool GetPersistWhenEmpty(const FAssetData& AssetData, bool& bPersistWhenEmpty);

};



// Module Macros
#undef RSYSTEM_API

