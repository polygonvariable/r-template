// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Interface/AscensionProviderInterface.h"

// Generated Headers
#include "AvatarAsset.generated.h"

// Module Macros
#define RCORE_API RCOREENTITY_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;

struct FAscensionData;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UAvatarAsset : public URPrimaryDataAsset, public IAscensionProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Health = 500;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Attack = 100;

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Defense = 100;


	// ~ IAscensionProviderInterface
	RCORE_API virtual int GetExperienceInterval(int Level) const override;
	RCORE_API virtual int GetLevelInterval(int Rank) const override;
	RCORE_API virtual int GetMaxLevel() const override;
	RCORE_API virtual int GetMaxRank() const override;
	RCORE_API virtual const UAssetCollection* GetExperienceItems(const FAscensionData& Ascension) const override;
	RCORE_API virtual const UAssetCollection* GetRankItems(const FAscensionData& Ascension) const override;
	// ~ End of IAscensionProviderInterface

	// ~ UPrimaryDataAsset
	RCORE_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

protected:

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> ExperienceItems = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RankItems = nullptr;

public:

	RCORE_API static FPrimaryAssetType GetPrimaryAssetType();

};



// Module Macros
#undef RCORE_API

