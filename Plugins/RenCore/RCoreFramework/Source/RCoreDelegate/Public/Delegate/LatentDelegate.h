// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Module Macros
#define RCORE_API RCOREDELEGATE_API

// Forward Declarations



/**
 *
 */
class RCORE_API FLatentDelegate
{

public:

	DECLARE_MULTICAST_DELEGATE(FLatentTaskDelegate);

	static FLatentTaskDelegate OnStorageLoaded;
	static FLatentTaskDelegate OnPreGameInitialized;
	static FLatentTaskDelegate OnPostGameInitialized;

};



// Module Macros
#undef RCORE_API

