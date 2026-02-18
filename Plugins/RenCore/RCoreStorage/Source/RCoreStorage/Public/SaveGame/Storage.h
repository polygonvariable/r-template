// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers

// Generated Headers
#include "Storage.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UStorage : public USaveGame
{

	GENERATED_BODY()

public:

	RCORESTORAGE_API virtual void NetDeserialize(TSharedPtr<FJsonObject>& JsonObject);
	RCORESTORAGE_API bool GetForceSave() const;

protected:

	UPROPERTY(Transient)
	bool bForceSave = true;

};

