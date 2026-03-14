// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"

// Generated Headers
#include "AvatarData.generated.h"



/**
 * 
 * 
 * 
 */
USTRUCT(BlueprintType)
struct FAvatarData
{

	GENERATED_BODY()

public:

	FAvatarData() {};
	FAvatarData(const FGuid& InAvatarId, int InHealth) : AvatarId(InAvatarId), Health(InHealth) {};

	UPROPERTY()
	FGuid AvatarId;

	UPROPERTY()
	int Health = 0;

	UPROPERTY()
	FAscensionData Ascension;

	void Sanitize()
	{
		Health = FMath::Max(0, Health);
		Ascension.Sanitize();
	}

	FString ToString() const
	{
		FString Detail = TEXT("Item Id: ") + AvatarId.ToString();
		Detail += TEXT("\nHealth: ") + FString::FromInt(Health);
		Detail += TEXT("\nAscension: ") + Ascension.ToString();
		return Detail;
	}

};

