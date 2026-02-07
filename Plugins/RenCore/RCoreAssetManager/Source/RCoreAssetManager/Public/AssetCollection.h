// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Asset/CommonDefinition.h"
#include "Asset/AssetRuleDefinition.h"

// Generated Headers
#include "AssetCollection.generated.h"

// Forward Declarations


UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetCollectionRule : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer RuleTags;

};

UCLASS(MinimalAPI)
class UAssetCollectionRule_Dictionary : public UAssetCollectionRule
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	ESelectionCondition SelectionCondition = ESelectionCondition::Or;

	UPROPERTY(EditDefaultsOnly, Meta = (ForceInlineRow))
	TMap<FPrimaryAssetId, int> AssetIds;

};









UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetCollectionGroup : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer GroupTags;

	RCOREASSETMANAGER_API virtual const UAssetCollectionRule* GetCollectionRule() const;
	RCOREASSETMANAGER_API virtual const UAssetCollectionRule* GetCollectionRule(const FAssetRuleContext& Context) const;

	template<typename T>
	const T* GetCollectionRule(const FAssetRuleContext& Context) const
	{
		return Cast<const T>(GetCollectionRule(Context));
	}

	template<typename T>
	const T* GetCollectionRule() const
	{
		return Cast<const T>(GetCollectionRule());
	}

};

UCLASS(MinimalAPI)
class UAssetCollectionGroup_Single : public UAssetCollectionGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	RCOREASSETMANAGER_API virtual const UAssetCollectionRule* GetCollectionRule() const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollectionRule> RuleCollection = nullptr;

};

UCLASS(MinimalAPI)
class UAssetCollectionGroup_Sequence : public UAssetCollectionGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	RCOREASSETMANAGER_API virtual const UAssetCollectionRule* GetCollectionRule() const override;
	RCOREASSETMANAGER_API virtual const UAssetCollectionRule* GetCollectionRule(const FAssetRuleContext& Context) const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UAssetCollectionRule>> RuleCollections;

};

