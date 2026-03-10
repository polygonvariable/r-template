// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Definition/Runtime/ShopData.h"

// Engine Headers

// Project Headers



FShopData::FShopData(int InPurchaseCount)
{
	PurchaseCount = InPurchaseCount;
}

bool FShopData::IsValid() const
{
	return PurchaseCount > 0;
}

void FShopData::Reset()
{
	PurchaseCount = 0;
}

void FShopData::Sanitize()
{
	PurchaseCount = FMath::Max(0, PurchaseCount);
}

