// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers

// Generated Headers
#include "Storage.generated.h"

// Module Macros
#define RCORE_API RCORESTORAGE_API

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UStorage : public USaveGame
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnStorageUpdated);
	FOnStorageUpdated OnStorageUpdated;

	RCORE_API virtual void InitializeDefaults();
	RCORE_API virtual void InitializeStorage();
	RCORE_API virtual void DeinitializeStorage();

	RCORE_API virtual void NetDeserialize(TSharedPtr<FJsonObject>& JsonObject);
	RCORE_API bool GetForceSave() const;

protected:

	UPROPERTY(Transient)
	bool bForceSave = true;

};



// Module Macros
#undef RCORE_API

