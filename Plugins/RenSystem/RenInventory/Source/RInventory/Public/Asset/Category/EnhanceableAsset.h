// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "EnhanceProviderInterface.h"
#include "ExchangeRule.h"

#include "Asset/Category/CraftableAsset.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Forward Declarations



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UEnhanceableAsset : public UCraftableAsset, public IEnhanceProviderInterface
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FExchangeRule EnhanceRules;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FExchangeRule> RankingRules;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FCurveTableRowHandle ExperienceCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FCurveTableRowHandle LevelCurve;


	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollectionGroup> ExperienceItems = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollectionGroup> RankItems = nullptr;


	int GetCurveValue(const FCurveTableRowHandle& Curve, int Default, int Time) const;

public:

	// ~ IEnhanceProviderInterface
	RINVENTORY_API virtual int GetExperienceInterval(int Level) const override;
	RINVENTORY_API virtual int GetLevelInterval(int Rank) const override;
	RINVENTORY_API virtual int GetMaxLevel() const override;
	RINVENTORY_API virtual int GetMaxRank() const override;
	RINVENTORY_API virtual const FExchangeRule& GetEnhanceRules() const override;
	RINVENTORY_API virtual const TArray<FExchangeRule>& GetRankingRules() const override;
	RINVENTORY_API virtual const UAssetCollectionRule_Dictionary* GetExperienceItems(int InExperience, int InLevel, int InRank) const override;
	RINVENTORY_API virtual const UAssetCollectionRule_Dictionary* GetRankItems(int InExperience, int InLevel, int InRank) const override;
	// ~ End of IEnhanceProviderInterface

};

