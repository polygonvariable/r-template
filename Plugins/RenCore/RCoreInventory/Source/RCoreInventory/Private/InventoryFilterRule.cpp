// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryFilterRule.h"

// Engine Headers

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"

#include "RCoreInventory/Public/InventoryRecord.h"



bool FInventoryFilterRule::Match(const FInventoryItem* Item) const
{
	TArray<bool> Results;

	if (Item)
	{
		Results.Add(FilterId.Matches(FName(*Item->ItemId.ToString())));
		Results.Add(FilterRank.Matches(Item->Ascension.Rank));
		Results.Add(FilterLevel.Matches(Item->Ascension.Level));
		Results.Add(FilterExperience.Matches(Item->Ascension.Experience));
		Results.Add(FilterQuantity.Matches(Item->Quantity));
	}

	return MatchInternal(Results);
}

bool FInventoryFilterRule::Match(const FPrimaryAssetId& AssetId, const FName& AssetType, const FName& AssetRarity) const
{
	TArray<bool> Results;

	auto AddIfEnabled = [&](const FFilterRule& Rule, bool bMatch)
		{
			if (Rule.bEnable) Results.Add(bMatch);
		};

	AddIfEnabled(FilterAsset, FilterAsset.Matches(AssetId));
	AddIfEnabled(FilterType, FilterType.Matches(AssetType));
	AddIfEnabled(FilterRarity, FilterRarity.Matches(AssetRarity));

	// If no filter is enabled, treat as "match everything".
	if (Results.Num() == 0)
	{
		return true;
	}

	return MatchInternal(Results);
}

bool FInventoryFilterRule::Match(const FInventoryItem* Item, const FPrimaryAssetId& AssetId, const FName& AssetType, const FName& AssetRarity) const
{
	TArray<bool> Results;

	if (Item)
	{
		Results.Add(FilterId.Matches(FName(*Item->ItemId.ToString())));
		Results.Add(FilterRank.Matches(Item->Ascension.Rank));
		Results.Add(FilterLevel.Matches(Item->Ascension.Level));
		Results.Add(FilterExperience.Matches(Item->Ascension.Experience));
		Results.Add(FilterQuantity.Matches(Item->Quantity));
	}

	Results.Add(FilterAsset.Matches(AssetId));
	Results.Add(FilterType.Matches(AssetType));
	Results.Add(FilterRarity.Matches(AssetRarity));

	return MatchInternal(Results);
}

bool FInventoryFilterRule::MatchInternal(const TArray<bool>& Results) const
{
	/*if (FilterCombination == EFilterCombination::And)
	{
		for (bool Result : Results)
		{
			if (!Result)
			{
				return false;
			}
		}
		return true;
	}
	else if (FilterCombination == EFilterCombination::Or)
	{
		for (bool Result : Results)
		{
			if (Result)
			{
				return true;
			}
		}
		return false;
	}*/

	return false;
}

