// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "CraftProviderInterface.generated.h"

// Module Macros
#define RCORE_API RCORECRAFT_API

// Forward Declarations
class UAssetCollection;



UINTERFACE(MinimalAPI)
class UCraftProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORECRAFT_API ICraftProviderInterface
{

	GENERATED_BODY()

public:

	virtual const UAssetCollection* GetCraftingItems() const = 0;

};



// Module Macros
#undef RCORE_API

