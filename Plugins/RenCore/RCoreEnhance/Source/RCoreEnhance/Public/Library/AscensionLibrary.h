// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Kismet/BlueprintFunctionLibrary.h"

// Project Headers

// Generated Headers
#include "AscensionLibrary.generated.h"

// Forward Declarations
struct FAscensionData;



/**
 *
 */
UCLASS(MinimalAPI)
class UAscensionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

    RCOREENHANCE_API static bool AddExperience(const FAscensionData& CurrentData, int Amount, int ExperiencePerLevel, int LevelPerRank, int MaxLevel, int MaxRank, int& OutExperience, int& OutLevel);
    RCOREENHANCE_API static bool CanGainExperience(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank);
    RCOREENHANCE_API static bool IsRankUpRequired(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank);

private:

    static int GetMaxLevelForRank(int Rank, int LevelPerRank, int MaxLevel);

};