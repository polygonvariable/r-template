// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/AvatarStorage.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Definition/AssetFilterProperty.h"
#include "Definition/AvatarQueryType.h"
#include "Definition/AvatarSortType.h"
#include "Definition/FilterContext.h"
#include "Definition/Runtime/AvatarInstance.h"
#include "Filter/FilterCriterion.h"
#include "Library/CharacterPrimaryAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/AvatarSettings.h"



void UAvatarStorage::InitializeDefaults()
{
	AvatarCollection = UAvatarSettings::Get()->DefaultAvatars;
}

bool UAvatarStorage::AddItem(const FPrimaryAssetId& AssetId, int Quantity)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		LOG_ERROR(LogAvatar, TEXT("AssetManager is invalid"));
		return false;
	}

	if (!AddInstance_Internal(AssetManager, AssetId))
	{
		LOG_ERROR(LogAvatar, TEXT("Failed to add instance"));
		return false;
	}

	OnRefreshed.Broadcast();
	return true;
}

bool UAvatarStorage::AddItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		LOG_ERROR(LogAvatar, TEXT("AssetManager is invalid"));
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		AddInstance_Internal(AssetManager, Kv.Key);
	}

	OnRefreshed.Broadcast();
	return true;
}

bool UAvatarStorage::RemoveItem(const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!RemoveInstance_Internal(AssetId))
	{
		return false;
	}

	OnRefreshed.Broadcast();
	return true;
}

bool UAvatarStorage::RemoveItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		RemoveInstance_Internal(Kv.Key);
	}

	OnRefreshed.Broadcast();
	return true;
}

bool UAvatarStorage::RemoveAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity)
{
    return bool();
}

bool UAvatarStorage::RemoveItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity)
{
    return bool();
}

bool UAvatarStorage::ContainItems(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const
{
    return bool();
}

bool UAvatarStorage::ContainAnyItems(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const
{
    return bool();
}

bool UAvatarStorage::UpdateItem(const FPrimaryAssetId& AssetId, TFunctionRef<void(FAvatarInstance*)> Callback)
{
	FAvatarInstance* Instance = AvatarCollection.Find(AssetId);
	if (!Instance)
	{
		return false;
	}

	Callback(Instance);

	OnRefreshed.Broadcast();
	return true;
}



const FAvatarInstance* UAvatarStorage::GetInstance(const FPrimaryAssetId& AssetId) const
{
	return AvatarCollection.Find(AssetId);
}

const FAvatarInstance* UAvatarStorage::GetInstanceById(const FGuid& ItemId) const
{
	for (const TPair<FPrimaryAssetId, FAvatarInstance>& Kv : AvatarCollection)
	{
		if (Kv.Value.AvatarId == ItemId)
		{
			return &Kv.Value;
		}
	}
	return nullptr;
}


void UAvatarStorage::QueryItems(const UFilterCriterion* FilterCriterion, const FAvatarQueryRule& QueryRule, TFunctionRef<void(const FAvatarSortEntry&)> Callback)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return;
	}

	TArray<FAvatarSortEntry> SortedItems;

	if (QueryRule.QuerySource == EAssetQuerySource::Asset)
	{
		QueryAssetItems(AssetManager, FilterCriterion, SortedItems);
	}
	else
	{
		QueryInstanceItems(AssetManager, FilterCriterion, SortedItems);
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FAvatarSortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}

void UAvatarStorage::HandleItemSorting(TArray<FAvatarSortEntry>& SortedItems, const FAvatarQueryRule& QueryRule) const
{
	const ESortDirection& SortDirection = QueryRule.SortDirection;
	const EAvatarSortType& SortType = QueryRule.SortType;

	switch (SortType)
	{
	case EAvatarSortType::None:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	case EAvatarSortType::Alphabetical:
		SortedItems.Sort([SortDirection](const FAvatarSortEntry& A, const FAvatarSortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.ItemName.ToString() < B.ItemName.ToString() :
					A.ItemName.ToString() > B.ItemName.ToString();
			}
		);
		break;
	case EAvatarSortType::Level:
		SortedItems.Sort([SortDirection](const FAvatarSortEntry& A, const FAvatarSortEntry& B)
			{
				if (!A.Item || !B.Item) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Item->Ascension.Level < B.Item->Ascension.Level :
					A.Item->Ascension.Level > B.Item->Ascension.Level;
			}
		);
		break;
	case EAvatarSortType::Rank:
		SortedItems.Sort([SortDirection](const FAvatarSortEntry& A, const FAvatarSortEntry& B)
			{
				if (!A.Item || !B.Item) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Item->Ascension.Rank < B.Item->Ascension.Rank :
					A.Item->Ascension.Rank > B.Item->Ascension.Rank;
			}
		);
		break;
	default:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	}
}

void UAvatarStorage::QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const
{
	TArray<FPrimaryAssetId> AssetIds;
	if (!AssetManager->GetPrimaryAssetIdList(FCharacterPrimaryAsset::GetAssetType(), AssetIds))
	{
		return;
	}

	OutSortedItems.Reserve(AssetIds.Num());

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		if (!FCharacterPrimaryAsset::GetDisplayName(AssetData, DisplayName))
		{
			continue;
		}

		if (IsValid(FilterCriterion))
		{
			FFilterContext Context;
			Context.SetValue(FAssetFilterProperty::AssetId, AssetId);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		OutSortedItems.Emplace(AssetId, DisplayName);
	}
}

void UAvatarStorage::QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const
{
	for (const TPair<FPrimaryAssetId, FAvatarInstance>& Kv : AvatarCollection)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;

		FAssetData AssetData;
		AssetManager->GetPrimaryAssetData(AssetId, AssetData);

		FText DisplayName = FText::GetEmpty();
		if (!FCharacterPrimaryAsset::GetDisplayName(AssetData, DisplayName))
		{
			continue;
		}

		const FAvatarInstance& Item = Kv.Value;
		if (IsValid(FilterCriterion))
		{
			FFilterContext Context;
			Context.SetValue(FAssetFilterProperty::AssetId, AssetId);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		OutSortedItems.Emplace(AssetId, DisplayName, &Item);
	}
}


bool UAvatarStorage::AddInstance_Internal(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId)
{
	if (!FCharacterPrimaryAsset::IsValid(AssetId) || AvatarCollection.Contains(AssetId))
	{
		return false;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		return false;
	}

	int Health = 0;
	if (!FCharacterPrimaryAsset::GetHealth(AssetData, Health))
	{
		return false;
	}

	FGuid ItemId = FGuid::NewGuid();
	AvatarCollection.Add(AssetId, FAvatarInstance(ItemId, Health));

	OnAdded.Broadcast(AssetId, ItemId);
	return true;
}

bool UAvatarStorage::RemoveInstance_Internal(const FPrimaryAssetId& AssetId)
{
	FAvatarInstance RemovedInstance;
	if (!AvatarCollection.RemoveAndCopyValue(AssetId, RemovedInstance))
	{
		return false;
	}

	OnRemoved.Broadcast(AssetId, RemovedInstance.AvatarId);
	return true;
}
