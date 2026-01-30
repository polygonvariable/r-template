// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "RCoreEntity/Public/AvatarRecord.h"

// Generated Headers
#include "AvatarEntry.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS()
class UAvatarEntry : public UObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId AvatarId = FPrimaryAssetId();
	FAvatarRecord Record = FAvatarRecord();

	void ResetData()
	{
		AvatarId = FPrimaryAssetId();
		Record = FAvatarRecord();
	}

	// ~ UObject
	virtual void BeginDestroy() override
	{
		ResetData();
		Super::BeginDestroy();
	}
	// ~ End of UObject

};

