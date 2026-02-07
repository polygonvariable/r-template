// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AssetCollection.h"

// Engine Headers

// Project Headers



const UAssetCollectionRule* UAssetCollectionGroup::GetCollectionRule() const
{
	return nullptr;
}
const UAssetCollectionRule* UAssetCollectionGroup::GetCollectionRule(const FAssetRuleContext& Context) const
{
	return nullptr;
}

const UAssetCollectionRule* UAssetCollectionGroup_Single::GetCollectionRule() const
{
	return RuleCollection;
}

const UAssetCollectionRule* UAssetCollectionGroup_Sequence::GetCollectionRule() const
{
	if (RuleCollections.Num() > 0)
	{
		return RuleCollections.Last();
	}
	return nullptr;
}

const UAssetCollectionRule* UAssetCollectionGroup_Sequence::GetCollectionRule(const FAssetRuleContext& Context) const
{
	FAssetRuleContext_Sequence Sequence = static_cast<FAssetRuleContext_Sequence>(Context);
	
	const FGameplayTagContainer& RuleTags = Sequence.RuleTags;

	if (RuleTags.IsEmpty())
	{
		int Index = Sequence.Index;
		if (!RuleCollections.IsValidIndex(Index))
		{
			return nullptr;
		}

		return RuleCollections[Index];
	}

	const TObjectPtr<UAssetCollectionRule>* RulePointer = RuleCollections.FindByPredicate([RuleTags](const UAssetCollectionRule* Rule) { return Rule->RuleTags.HasAllExact(RuleTags); });
	if (!RulePointer)
	{
		return nullptr;
	}

	return RulePointer->Get();
}
