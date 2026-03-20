// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"

// Generated Headers
#include "WorldConfigSettings.generated.h"

// Module Macros
#define RCORE_API RCORESETTINGS_API

// Forward Declarations
class UPrimaryDataAsset;



/**
 * 
 * 
 * 
 */
UCLASS()
class RCORE_API AWorldConfigSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "World Config Settings")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenAsset.ClockAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> ClockAsset;

	UPROPERTY(EditAnywhere, Meta = (AllowedClasses = "/Script/RenEnvironment.EnvironmentAsset"), Category = "World Config Settings")
	TObjectPtr<UPrimaryDataAsset> EnvironmentAsset;

};



// Module Macros
#undef RCORE_API

