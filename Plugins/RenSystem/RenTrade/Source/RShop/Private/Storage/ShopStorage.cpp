// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/ShopStorage.h"

// Engine Headers

// Project Headers



bool UShopStorage::GetItem(const FShopKey& ShopKey, FShopData& OutShopData)
{
	FShopData* FoundData = ShopItems.Find(ShopKey);
	if (!FoundData)
	{
		return false;
	}

	OutShopData = *FoundData;
	return true;
}

bool UShopStorage::AddItem(const FShopKey& ShopKey)
{
	FShopData* FoundData = ShopItems.Find(ShopKey);
	if (!FoundData)
	{
		FShopData ShopData;
		ShopData.Quota = 1;
		ShopData.BatchTime = FDateTime::Now();

		ShopItems.Add(ShopKey, ShopData);

		return true;
	}

	FoundData->Quota++;
	return true;
}

//int UShopStorage::RemoveAvailableItems(FShopKey ShopKey)
//{
//	FShopData* FoundData = ShopItems.Find(ShopKey);
//	if (!FoundData)
//	{
//		return 0;
//	}
//
//	int TotalQuantity = FoundData->Quantity;
//	FDateTime BatchTime = FoundData->BatchTime;
//	FTimespan BatchDuration = FoundData->BatchDuration;
//
//	FDateTime CurrentTime = FDateTime::Now();
//
//	FTimespan Elapsed = CurrentTime - BatchTime;
//
//	int Completed = 0;
//
//	if (BatchDuration.GetTicks() > 0)
//	{
//		Completed = Elapsed.GetTicks() / BatchDuration.GetTicks();
//	}
//
//	int AvailableQuantity = FMath::Clamp(Completed, 0, TotalQuantity);
//	if (AvailableQuantity > 0)
//	{
//		FoundData->BatchTime = BatchTime + (BatchDuration * AvailableQuantity);
//	}
//
//	FoundData->Quantity -= AvailableQuantity;
//	FoundData->Sanitize();
//
//	if (FoundData->Quantity <= 0)
//	{
//		ShopItems.Remove(ShopKey);
//	}
//
//	return AvailableQuantity;
//}





void UShopStorage::ResetItems()
{
	ShopItems.Empty();
}

