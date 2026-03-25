// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IAscensionInstanceData.generated.h"

// Module Macros
#define RCORE_API RCOREASCENSION_API

// Forward Declarations
struct FAscensionData;



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UAscensionInstanceData : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORE_API IAscensionInstanceData
{

	GENERATED_BODY()

public:

	virtual const FAscensionData* GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const = 0;

};



// Module Macros
#undef RCORE_API

