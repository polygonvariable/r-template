// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "InventorySettings.generated.h"

// Forward Declarations
class UInventoryStorage;
class UInventorySubsystem;



/**
 *
 */
UCLASS(Config = Game, DefaultConfig)
class UInventorySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly)
	FString StorageUrl;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UInventoryStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UInventorySubsystem> SubsystemClass;


	static const UInventorySettings* Get()
	{
		return GetDefault<UInventorySettings>();
	}
};

