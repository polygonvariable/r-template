// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/GrantItemExperience.h"

// Engine Headers

// Project Headers
#include "Asset/AscensionAsset.h"
#include "Asset/InventoryAsset.h"
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/Runtime/InventoryItem.h"
#include "Interface/AscensionProviderInterface.h"
#include "Interface/AssetStructureInterface.h"
#include "Library/AscensionLibrary.h"
#include "Management/Collection/AssetCollectionSimple.h"
#include "Manager/RAssetManager.inl"
#include "Subsystem/InventorySubsystem.h"




void UGrantItemExperience::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	InventorySubsystem = UInventorySubsystem::Get(GetWorld());
	if (!IsValid(AssetManager) || !IsValid(InventorySubsystem))
	{
		Fail(TEXT("AssetManager, InventorySubsystem is invalid"));
		return;
	}

	Step_LoadAssets();
}

void UGrantItemExperience::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UGrantItemExperience::OnCleanup()
{
	InventoryId.Invalidate();
	TargetId.Invalidate();
	MaterialId.Invalidate();

	TargetAssetId = FPrimaryAssetId();
	MaterialAssetId = FPrimaryAssetId();

	MaterialQuantity = 0;

	TargetAsset = nullptr;
	MaterialAsset = nullptr;
	AssetManager = nullptr;
	InventorySubsystem = nullptr;
}

void UGrantItemExperience::Step_LoadAssets()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(TargetAssetId);
	Assets.Add(MaterialAssetId);

	TFuture<FLatentResultAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UGrantItemExperience> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAssets<URPrimaryDataAsset>& Result)
		{
			UGrantItemExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<URPrimaryDataAsset*>& Assets = Result.Get();
			This->TargetAsset = Cast<UInventoryAsset>(Assets[0]);
			This->MaterialAsset = Assets[1];

			This->Step_CheckItemAsset();
		}
	);
}

void UGrantItemExperience::Step_CheckItemAsset()
{
	const FInventoryItem* Item = InventorySubsystem->GetItemById(InventoryId, TargetAssetId, TargetId);
	if (!IsValid(TargetAsset) || !Item)
	{
		Fail(TEXT("Item not found, TargetAsset is invalid"));
		return;
	}

	const IAscensionProviderInterface* AscensionProvider = Cast<IAscensionProviderInterface>(TargetAsset);
	if (!AscensionProvider)
	{
		Fail(TEXT("The item doesn't support ascension (IAscensionProviderInterface is not implemented)"));
		return;
	}

	AscensionData = Item->Ascension;

	ExperiencePerLevel = AscensionProvider->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionProvider->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionProvider->GetMaxLevel();
	MaxRank = AscensionProvider->GetMaxRank();

	const UAssetCollection_Simple* ExperienceItems = Cast<UAssetCollection_Simple>(AscensionProvider->GetExperienceItems(AscensionData));
	if (!IsValid(ExperienceItems))
	{
		Fail(TEXT("The material cannot be used to upgrade the item"));
		return;
	}

	const FGameplayTagContainer& CollectionTags = ExperienceItems->GetCollectionTags();
	bool bFound = ExperienceItems->GetAsset(MaterialAssetId, MaterialQuantity);
	if (!bFound)
	{
		Fail(TEXT("Failed to fetch collection item"));
		return;
	}

	Step_CheckMaterialAsset(CollectionTags);
}

void UGrantItemExperience::Step_CheckMaterialAsset(const FGameplayTagContainer& RuleTags)
{
	if (!IsValid(MaterialAsset) || !IsValid(InventorySubsystem))
	{
		Fail(TEXT("MaterialAsset, InventorySubsystem is invalid"));
		return;
	}

	const IAssetStructureInterface* AssetStructure = Cast<IAssetStructureInterface>(MaterialAsset);
	if (!AssetStructure)
	{
		Fail(TEXT("Material asset doesn't have asset structure"));
		return;
	}

	// Possible items that material can break into
	// in this case the Material item will break into Exp item
	const UAssetCollection_Simple* BreakdownAssets = Cast<UAssetCollection_Simple>(AssetStructure->GetBreakdownAssets(RuleTags));
	if (!IsValid(BreakdownAssets))
	{
		Fail(TEXT("Invalid BreakdownAssets"));
		return;
	}

	TPair<FPrimaryAssetId, int> Collection;
	bool bFound = BreakdownAssets->GetAnyAsset(Collection);
	if (!bFound)
	{
		Fail(TEXT("Failed to get asset pair"));
		return;
	}

	const FPrimaryAssetId& BreakdownAssetId = Collection.Key;
	Step_LoadBreakdownAsset(BreakdownAssetId, Collection.Value);
}

void UGrantItemExperience::Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity)
{
	TFuture<FLatentResultAsset<UExperiencePointAsset>> Future = AssetManager->FetchPrimaryAsset<UExperiencePointAsset>(TaskId, AssetId);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UGrantItemExperience> WeakThis(this);
	Future.Next([WeakThis, Quantity](const FLatentResultAsset<UExperiencePointAsset>& Result)
		{
			UGrantItemExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const UExperiencePointAsset* PointAsset = Result.Get();
			
			This->Points = PointAsset->GetPoints(Quantity);
			This->Step_RemoveItem();
		}
	);
}

void UGrantItemExperience::Step_RemoveItem()
{
	bool bRemoved = InventorySubsystem->RemoveItemById(InventoryId, MaterialAssetId, MaterialId, MaterialQuantity);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	Step_AddExperience();
}

// UE_DISABLE_OPTIMIZATION
void UGrantItemExperience::Step_AddExperience()
{
	const FInventoryItem* Item = InventorySubsystem->GetItemById(InventoryId, TargetAssetId, TargetId);
	if (!Item)
	{
		Fail(TEXT("Item not found"));
		return;
	}

	int Amount = FMath::Max(0, (Points * MaterialQuantity));

	int NewExperience = 0;
	int NewLevel = 0;

	bool bAdded = UAscensionLibrary::AddExperience(AscensionData, Amount, ExperiencePerLevel, LevelPerRank, MaxLevel, MaxRank, NewExperience, NewLevel);
	if (!bAdded)
	{
		Fail(TEXT("Failed to add experience"));
		return;
	}

	bool bSuccess = InventorySubsystem->UpdateItemById(InventoryId, TargetAssetId, TargetId, [NewExperience, NewLevel](FInventoryItem* Item)
		{
			if (Item)
			{
				Item->Ascension.Experience = NewExperience;
				Item->Ascension.Level = NewLevel;
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
// UE_ENABLE_OPTIMIZATION

