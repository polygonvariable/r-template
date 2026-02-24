// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "AscensionProviderInterface.generated.h"

// Forward Declarations
class UAssetCollection;

struct FAscensionData;



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UAscensionProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCOREENHANCE_API IAscensionProviderInterface
{

	GENERATED_BODY()

public:

	virtual int GetExperienceInterval(int Level) const = 0;
	virtual int GetLevelInterval(int Rank) const = 0;
	virtual int GetMaxLevel() const = 0;
	virtual int GetMaxRank() const = 0;

	virtual const UAssetCollection* GetExperienceItems(const FAscensionData& Ascension) const = 0;
	virtual const UAssetCollection* GetRankItems(const FAscensionData& Ascension) const = 0;

};
