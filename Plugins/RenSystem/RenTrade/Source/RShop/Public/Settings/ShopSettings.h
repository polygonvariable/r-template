// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "ShopSettings.generated.h"

// Forward Declarations
class UShopStorage;
class UShopSubsystem;



/**
 *
 */
UCLASS(MinimalAPI, Config = Game, DefaultConfig)
class UShopSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly)
	FString StorageUrl;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UShopStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UShopSubsystem> SubsystemClass;


	static const UShopSettings* Get()
	{
		return GetDefault<UShopSettings>();
	}

};

