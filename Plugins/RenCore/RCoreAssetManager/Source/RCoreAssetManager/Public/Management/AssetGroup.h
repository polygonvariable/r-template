// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "AssetGroup.generated.h"

// Module Macros
#define RCORE_API RCOREASSETMANAGER_API

// Forward Declarations
class UAssetCollection;

struct FInstancedStruct;



/**
 *
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetGroup : public UObject
{

	GENERATED_BODY()

public:

	RCORE_API virtual const UAssetCollection* GetCollectionRule() const;
	RCORE_API virtual const UAssetCollection* GetCollectionRule(const FInstancedStruct& Context) const;

	template<typename T>
	const T* GetCollectionRule() const
	{
		return Cast<const T>(GetCollectionRule());
	}

	template<typename T>
	const T* GetCollectionRule(const FInstancedStruct& Context) const
	{
		return Cast<const T>(GetCollectionRule(Context));
	}

};



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Group (Single)")
class UAssetGroup_Single : public UAssetGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	RCORE_API virtual const UAssetCollection* GetCollectionRule() const override;
	RCORE_API virtual const UAssetCollection* GetCollectionRule(const FInstancedStruct& Context) const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollection> RuleCollection = nullptr;

};



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Group (List)")
class UAssetGroup_List : public UAssetGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	RCORE_API virtual const UAssetCollection* GetCollectionRule() const override;
	RCORE_API virtual const UAssetCollection* GetCollectionRule(const FInstancedStruct& Context) const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UAssetCollection>> RuleCollections;

};



// Module Macros
#undef RCORE_API

