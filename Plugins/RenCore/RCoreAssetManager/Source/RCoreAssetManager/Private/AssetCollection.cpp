// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AssetCollection.h"

// Engine Headers

// Project Headers



const UAssetCollection* UAssetGroup::GetCollectionRule() const
{
	return nullptr;
}

const UAssetCollection* UAssetGroup::GetCollectionRule(const FAssetRuleContext& Context) const
{
	return nullptr;
}

const UAssetCollection* UAssetGroup_Single::GetCollectionRule() const
{
	return RuleCollection;
}

const UAssetCollection* UAssetGroup_List::GetCollectionRule() const
{
	if (RuleCollections.Num() > 0)
	{
		return RuleCollections.Last();
	}
	return nullptr;
}

const UAssetCollection* UAssetGroup_List::GetCollectionRule(const FAssetRuleContext& Context) const
{
	const FGameplayTagContainer& RuleTags = Context.Tags;

	if (RuleTags.IsEmpty())
	{
		FAssetRuleContext_List Sequence = static_cast<FAssetRuleContext_List>(Context);

		int Index = Sequence.Index;
		if (!RuleCollections.IsValidIndex(Index))
		{
			return nullptr;
		}

		return RuleCollections[Index];
	}

	const TObjectPtr<UAssetCollection>* RulePointer = RuleCollections.FindByPredicate([RuleTags](const UAssetCollection* Rule) { return Rule->Tags.HasAllExact(RuleTags); });
	if (!RulePointer)
	{
		return nullptr;
	}

	return RulePointer->Get();
}

void UAssetCollection::GetAssetCollection(TMap<FPrimaryAssetId, int>& OutCollection) const
{
	OutCollection = AssetIds;
}

void UAssetCollection::GetAnyPair(TPair<FPrimaryAssetId, int>& OutPair) const
{
	for (const TPair<FPrimaryAssetId, int>& Pair : AssetIds)
	{
		OutPair = Pair;
		break;
	}
}
