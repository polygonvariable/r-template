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



UCLASS(MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetCollection : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditDefaultsOnly)
	TMap<FPrimaryAssetId, int> AssetIds;

	RCOREASSETMANAGER_API void GetAssetCollection(TMap<FPrimaryAssetId, int>& OutCollection) const;
	RCOREASSETMANAGER_API void GetAnyPair(TPair<FPrimaryAssetId, int>& OutPair) const;

};

UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetGroup : public UObject
{

	GENERATED_BODY()

public:

	RCOREASSETMANAGER_API virtual const UAssetCollection* GetCollectionRule() const;
	RCOREASSETMANAGER_API virtual const UAssetCollection* GetCollectionRule(const FAssetRuleContext& Context) const;

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
class UAssetGroup_Single : public UAssetGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	RCOREASSETMANAGER_API virtual const UAssetCollection* GetCollectionRule() const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollection> RuleCollection = nullptr;

};

UCLASS(MinimalAPI)
class UAssetGroup_List : public UAssetGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	RCOREASSETMANAGER_API virtual const UAssetCollection* GetCollectionRule() const override;
	RCOREASSETMANAGER_API virtual const UAssetCollection* GetCollectionRule(const FAssetRuleContext& Context) const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UAssetCollection>> RuleCollections;

};

