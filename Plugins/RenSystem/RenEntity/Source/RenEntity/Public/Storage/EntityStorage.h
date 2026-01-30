// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "RCoreEntity/Public/AvatarRecord.h"

// Generated Headers
#include "EntityStorage.generated.h"



/**
 *
 */
UCLASS()
class UAvatarStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FPrimaryAssetId, FAvatarRecord> Records;

protected:

	//UPROPERTY(Transient)
	//bool bAllowNetSync = false;

	//UPROPERTY(Transient)
	//TArray<FAvatarCrudHandle> UnverifiedRecords;

};



/*
UENUM()
enum class ECrudType : uint8
{
	Create,
	Read,
	Update,
	Delete
};

USTRUCT()
struct FCrudHandle
{

	GENERATED_BODY()

public:

	FGuid ActionId;
	ECrudType ActionType;

};

USTRUCT()
struct FAvatarCrudHandle : public FCrudHandle
{

	GENERATED_BODY()

public:

	FAvatarRecord Record;

};
*/