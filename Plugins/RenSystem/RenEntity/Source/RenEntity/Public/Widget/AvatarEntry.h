// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "AvatarRecord.h"
#include "Widget/AssetEntry.h"

// Generated Headers
#include "AvatarEntry.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS()
class UAvatarEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	FAvatarData AvatarData;

};

