// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers

// Generated Headers
#include "AssetSettings.generated.h"

// Module Macros
#define RCORE_API RCOREASSET_API

// Forward Declarations



/**
 *
 */
UCLASS(Config = Game, DefaultConfig)
class UAssetSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	TMap<FPrimaryAssetType, TSubclassOf<UGameInstanceSubsystem>> AssetInterchangeClasses;

};



// Module Macros
#undef RCORE_API

