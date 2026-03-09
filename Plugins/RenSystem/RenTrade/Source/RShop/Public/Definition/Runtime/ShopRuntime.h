// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "ShopRuntime.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FShopKey
{

	GENERATED_BODY()

public:

	FShopKey() {}
	FShopKey(FPrimaryAssetId InShopAssetId, FGuid InShopCollectionId, FPrimaryAssetId InTargetAssetId)
	{
		ShopAssetId = InShopAssetId;
		ShopCollectionId = InShopCollectionId;
		TargetAssetId = InTargetAssetId;
	}


	UPROPERTY(SaveGame)
	FPrimaryAssetId ShopAssetId;

	UPROPERTY(SaveGame)
	FGuid ShopCollectionId;

	UPROPERTY(SaveGame)
	FPrimaryAssetId TargetAssetId;


	friend inline bool operator == (const FShopKey& A, const FShopKey& B)
	{
		return
			A.ShopAssetId == B.ShopAssetId &&
			A.ShopCollectionId == B.ShopCollectionId &&
			A.TargetAssetId == B.TargetAssetId;
	}

	friend inline uint32 GetTypeHash(const FShopKey& Item)
	{
		uint32 Hash = GetTypeHash(Item.ShopAssetId);
		Hash = HashCombineFast(Hash, GetTypeHash(Item.ShopCollectionId));
		Hash = HashCombineFast(Hash, GetTypeHash(Item.TargetAssetId));
		return Hash;
	}

};



/**
 *
 */
USTRUCT(BlueprintType)
struct FShopData
{

	GENERATED_BODY()

public:

	FShopData() {}

	UPROPERTY(SaveGame)
	int PurchaseCount;

	void Sanitize()
	{
		PurchaseCount = FMath::Max(0, PurchaseCount);
	}

};

