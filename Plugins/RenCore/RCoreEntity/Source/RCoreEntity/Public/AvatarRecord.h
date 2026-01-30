// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"

// Generated Headers
#include "AvatarRecord.generated.h"



/**
 * 
 * 
 * 
 */
USTRUCT(BlueprintType)
struct FAvatarRecord
{

	GENERATED_BODY()

public:

	FAvatarRecord() : Health(0) {}

	UPROPERTY()
	int Health = 0;

	UPROPERTY()
	FEnhanceRecord Enhance;

	void Sanitize()
	{
		Health = FMath::Max(0, Health);
		Enhance.Sanitize();
	}

};

