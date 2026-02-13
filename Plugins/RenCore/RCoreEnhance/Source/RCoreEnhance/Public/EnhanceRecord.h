// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EnhanceRecord.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnhanceRecord
{

	GENERATED_BODY()

public:

	FEnhanceRecord() {}
	FEnhanceRecord(int InRank, int InLevel, int InExperience) : Rank(InRank), Level(InLevel), Experience(InExperience) {}


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DeprecatedProperty, DeprecationMessage = "Use FAscensionData instead"))
	int Rank = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DeprecatedProperty, DeprecationMessage = "Use FAscensionData instead"))
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DeprecatedProperty, DeprecationMessage = "Use FAscensionData instead"))
	int Experience = 0;

	void Sanitize()
	{
		Rank = FMath::Max(1, Rank);
		Level = FMath::Max(1, Level);
		Experience = FMath::Max(0, Experience);
	}

	void SanitizeExperience(int ExperiencePerLevel = 1000, int LevelPerRank = 10)
	{
		if (ExperiencePerLevel <= 0 || LevelPerRank <= 0)
		{
			return;
		}

		Experience = FMath::Clamp(Experience, 0, ExperiencePerLevel);

		while (Experience >= ExperiencePerLevel)
		{
			Experience -= ExperiencePerLevel;
			Level++;

			if (Level > LevelPerRank)
			{
				Level = 1;
				Rank++;
			}
		}
	}

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FAscensionData
{

	GENERATED_BODY()

public:

	FAscensionData() {}
	FAscensionData(int InExperience, int InLevel, int InRank) : Experience(InExperience), Level(InLevel), Rank(InRank) {}


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Experience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Rank = 1;

	void Sanitize()
	{
		Rank = FMath::Max(1, Rank);
		Level = FMath::Max(1, Level);
		Experience = FMath::Max(0, Experience);
	}

	void Sanitize(int ExperiencePerLevel, int LevelPerRank)
	{
		Sanitize();

		if (ExperiencePerLevel <= 0 || LevelPerRank <= 0)
		{
			return;
		}

		Experience = FMath::Clamp(Experience, 0, ExperiencePerLevel);

		while (Experience >= ExperiencePerLevel)
		{
			Experience -= ExperiencePerLevel;
			Level++;

			if (Level > LevelPerRank)
			{
				Level = 1;
				Rank++;
			}
		}
	}

};

