// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "AssetCollection.h" // TODO: Remove this (move EAssetCollectionCondition to separate  header)

// Generated Headers
#include "AvatarAsset.generated.h"

// Forward Declarations
class UAssetGroup;



/*
USTRUCT(BlueprintType)
struct FExchangeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId CostItem;

	UPROPERTY(EditDefaultsOnly)
	int CostAmount = 0;

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId ResultItem;

	UPROPERTY(EditDefaultsOnly)
	int ResultAmount = 0;

};
*/








/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UAvatarAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(VisibleDefaultsOnly, AssetRegistrySearchable)
	FName Type = TEXT("Entity.Avatar");

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Health = 500;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Attack = 100;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Defense = 100;


	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> ExperienceItems = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RankItems = nullptr;


	RCOREENTITY_API virtual const UAssetCollection* GetExperienceItems(int InExperience, int InLevel, int InRank) const;
	RCOREENTITY_API virtual const UAssetCollection* GetRankItems(int InExperience, int InLevel, int InRank) const;

	// ~ UPrimaryDataAsset
	RCOREENTITY_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

};




