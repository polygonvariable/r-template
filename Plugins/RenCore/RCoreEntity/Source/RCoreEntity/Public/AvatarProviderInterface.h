// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "AvatarProviderInterface.generated.h"

// Forward Declarations
struct FAvatarRecord;



UINTERFACE(MinimalAPI)
class UAvatarProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCOREENTITY_API IAvatarProviderInterface
{

	GENERATED_BODY()

public:

	virtual const TMap<FPrimaryAssetId, FAvatarRecord>& GetAvatarCollection() const = 0;
	virtual TMap<FPrimaryAssetId, FAvatarRecord>& GetMutableAvatarCollection() = 0;

};

