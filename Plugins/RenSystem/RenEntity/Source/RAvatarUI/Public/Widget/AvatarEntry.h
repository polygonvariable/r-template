// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntry.h"

// Generated Headers
#include "AvatarEntry.generated.h"

// Forward Declarations
struct FAvatarData;



/**
 *
 */
UCLASS()
class UAvatarEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	const FAvatarData* AvatarInstance = nullptr;

	// ~ UAvatarEntry
	virtual void ResetData() override
	{
		AvatarInstance = nullptr;
		Super::ResetData();
	}
	// ~ UAvatarEntry

};

