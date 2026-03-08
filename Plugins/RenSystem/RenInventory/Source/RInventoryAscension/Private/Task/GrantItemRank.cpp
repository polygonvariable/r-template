// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/GrantItemRank.h"

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/Runtime/InventoryItem.h"
#include "Interface/AscensionProviderInterface.h"
#include "Library/AscensionLibrary.h"
#include "Management/Collection/AssetCollectionSimple.h"
#include "Manager/RAssetManager.inl"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"




void UGrantItemRank::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetWorld());
	if (!IsValid(AssetManager) || !IsValid(InventorySubsystem))
	{
		Fail(TEXT("AssetManager, InventorySubsystem is invalid"));
		return;
	}

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(UInventorySubsystem::GetStorageId());
	if (!IsValid(InventoryStorage))
	{
		Fail(TEXT("InventoryStorage is invalid"));
		return;
	}

	Inventory = InventoryStorage;

	Step_LoadAsset();
}

void UGrantItemRank::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UGrantItemRank::OnCleanup()
{
	InventoryId.Invalidate();
	TargetId.Invalidate();

	TargetAssetId = FPrimaryAssetId();

	TargetAsset = nullptr;
	AssetManager = nullptr;
	Inventory = nullptr;

	AscensionData.Reset();
}

void UGrantItemRank::Step_LoadAsset()
{
	TFuture<FLatentLoadedAsset<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAsset<URPrimaryDataAsset>(TargetAssetId);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UGrantItemRank> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAsset<URPrimaryDataAsset>& Result)
		{
			UGrantItemRank* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			This->TargetAsset = Cast<UInventoryAsset>(Result.Get());
			This->Step_CheckTarget();
		}
	);
}

void UGrantItemRank::Step_CheckTarget()
{
	const IAscensionProviderInterface* AscensionProvider = Cast<IAscensionProviderInterface>(TargetAsset);
	if (!AscensionProvider)
	{
		Fail(TEXT("The item doesn't support ascension (IAscensionProviderInterface is not implemented)"));
		return;
	}

	const FInventoryItem* Item = Inventory->GetItemById(TargetAssetId, TargetId);
	if (!IsValid(TargetAsset) || !Item)
	{
		Fail(TEXT("Item not found, TargetAsset is invalid"));
		return;
	}

	AscensionData = Item->Ascension;

	ExperiencePerLevel = AscensionProvider->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionProvider->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionProvider->GetMaxLevel();
	MaxRank = AscensionProvider->GetMaxRank();

	bool bRankUpRequired = UAscensionLibrary::IsRankUpRequired(AscensionData, LevelPerRank, MaxLevel, MaxRank);
	if (!bRankUpRequired)
	{
		Fail(TEXT("Item cannot rank up"));
		return;
	}

	const UAssetCollection* RankItems = AscensionProvider->GetRankItems(AscensionData);
	if (!IsValid(RankItems))
	{
		Fail(TEXT("The material cannot be used to upgrade the item"));
		return;
	}

	TMap<FPrimaryAssetId, int> AssetList;
	RankItems->GetAssetList(AssetList);

	bool bRemoved = Inventory->RemoveItems(AssetList, 1);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	bool bSuccess = Inventory->UpdateItemById(TargetAssetId, TargetId,
		[](FInventoryItem* Item)
		{
			if (Item)
			{
				Item->Ascension.Rank++;
				Item->Sanitize();
			}
		}
	);

	if (!bSuccess)
	{
		Fail(TEXT("Failed to update"));
		return;
	}

	Succeed();
}

