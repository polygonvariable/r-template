// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define RCORE_API RCOREDELEGATE_API



/**
 *
 */
class RCORE_API FGameLifecycleDelegates
{

public:

	DECLARE_MULTICAST_DELEGATE(FGameEventDelegate);

	static FGameEventDelegate OnPreGameInitialized;
	static FGameEventDelegate OnPostGameInitialized;

};


// Module Macros
#undef RCORE_API

