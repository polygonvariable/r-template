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


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Rank = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Experience = 0;

	void Sanitize()
	{
		Rank = FMath::Max(1, Rank);
		Level = FMath::Max(1, Level);
		Experience = FMath::Max(0, Experience);
	}

};

