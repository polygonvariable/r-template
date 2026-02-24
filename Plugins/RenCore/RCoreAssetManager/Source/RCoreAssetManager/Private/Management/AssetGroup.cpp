// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/AssetGroup.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Definition/AssetRuleDefinition.h"
#include "Management/AssetCollection.h"



const UAssetCollection* UAssetGroup::GetCollectionRule() const
{
	return nullptr;
}

const UAssetCollection* UAssetGroup::GetCollectionRule(const FInstancedStruct& Context) const
{
	return nullptr;
}



const UAssetCollection* UAssetGroup_Single::GetCollectionRule() const
{
	return RuleCollection;
}

const UAssetCollection* UAssetGroup_Single::GetCollectionRule(const FInstancedStruct& Context) const
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

const UAssetCollection* UAssetGroup_List::GetCollectionRule(const FInstancedStruct& Context) const
{
	const FAssetRuleContext* BaseContext = Context.GetPtr<FAssetRuleContext>();
	if (!BaseContext)
	{
		return nullptr;
	}

	const FGameplayTagContainer& RuleTags = BaseContext->Tags;
	if (RuleTags.IsEmpty())
	{
		const FAssetRuleContext_List* ListContext = Context.GetPtr<FAssetRuleContext_List>();
		if (ListContext)
		{
			int Index = ListContext->Index;
			if (!RuleCollections.IsValidIndex(Index))
			{
				return nullptr;
			}

			return RuleCollections[Index];
		}
	}

	const TObjectPtr<UAssetCollection>* RulePointer = RuleCollections.FindByPredicate(
		[RuleTags](const UAssetCollection* Rule)
		{
			return Rule->GetCollectionTags().HasAllExact(RuleTags);
		}
	);

	if (!RulePointer)
	{
		return nullptr;
	}

	return RulePointer->Get();
}

