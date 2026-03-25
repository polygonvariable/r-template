// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IAssetInstanceData.generated.h"

// Module Macros
#define RCORE_API RCOREASSETINSTANCE_API



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UAssetInstanceData : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 *
 */
class RCORE_API IAssetInstanceData
{

	GENERATED_BODY()

public:

	virtual FGuid GetInstanceId() const = 0;
	virtual void SetInstanceId(const FGuid& InstanceId) = 0;

};


// Module Macros
#undef RCORE_API

