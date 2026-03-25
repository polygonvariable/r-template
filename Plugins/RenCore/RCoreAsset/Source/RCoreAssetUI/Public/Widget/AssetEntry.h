// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "InstancedStruct.h"

// Generated Headers
#include "AssetEntry.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API



/**
 *
 */
UCLASS(MinimalAPI)
class UAssetEntry : public UObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId AssetId = FPrimaryAssetId();
	FInstancedStruct AssetSubDetail;


	RCORE_API virtual FGuid GetAssetInstanceId() const;
	RCORE_API virtual void ResetData();

	// ~ UObject
	RCORE_API virtual void BeginDestroy() override;
	// ~ End of UObject

};


// Module Macros
#undef RCORE_API

