// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Definition/Runtime/AvatarInstance.h"

// Generated Headers
#include "AvatarSettings.generated.h"

// Forward Declarations
class UAvatarStorage;
class UAvatarSubsystem;



/**
 *
 */
UCLASS(MinimalAPI, Config = Game, DefaultConfig)
class UAvatarSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly)
	FString StorageUrl = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UAvatarStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UAvatarSubsystem> SubsystemClass;


	static const UAvatarSettings* Get()
	{
		return GetDefault<UAvatarSettings>();
	}

};

