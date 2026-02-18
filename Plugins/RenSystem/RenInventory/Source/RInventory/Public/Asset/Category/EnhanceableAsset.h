// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Interface/AscensionProviderInterface.h"
#include "ExchangeRule.h"

#include "Asset/Category/CraftableAsset.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Forward Declarations
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
	RINVENTORY_API virtual int GetExperienceInterval(int Level) const override;
	RINVENTORY_API virtual int GetLevelInterval(int Rank) const override;
	RINVENTORY_API virtual int GetMaxLevel() const override;
	RINVENTORY_API virtual int GetMaxRank() const override;
	RINVENTORY_API virtual const UAssetCollection* GetExperienceItems(const FAscensionData& Ascension) const override;
	RINVENTORY_API virtual const UAssetCollection* GetRankItems(const FAscensionData& Ascension) const override;
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

