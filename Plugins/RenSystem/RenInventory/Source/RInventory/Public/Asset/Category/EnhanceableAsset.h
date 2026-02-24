// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/Category/CraftableAsset.h"
#include "Interface/AscensionProviderInterface.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Module Macros
#define RSYSTEM_API RINVENTORY_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;

struct FAscensionData;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UEnhanceableAsset : public UCraftableAsset, public IAscensionProviderInterface
{

	GENERATED_BODY()

public:

	// ~ IAscensionProviderInterface
	RSYSTEM_API virtual int GetExperienceInterval(int Level) const override;
	RSYSTEM_API virtual int GetLevelInterval(int Rank) const override;
	RSYSTEM_API virtual int GetMaxLevel() const override;
	RSYSTEM_API virtual int GetMaxRank() const override;
	RSYSTEM_API virtual const UAssetCollection* GetExperienceItems(const FAscensionData& Ascension) const override;
	RSYSTEM_API virtual const UAssetCollection* GetRankItems(const FAscensionData& Ascension) const override;
	// ~ End of IAscensionProviderInterface

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

};



// Module Macros
#undef RSYSTEM_API

