// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IActorFreeList.generated.h"

// Module Macros
#define RCORE_API RCOREPOOL_API



UINTERFACE(MinimalAPI)
class UActorFreeList : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORE_API IActorFreeList
{
	
	GENERATED_BODY()

public:

	virtual AActor* GetNextNode() const = 0;
	virtual void SetNextNode(AActor* Node) = 0;
};


// Module Macros
#undef RCORE_API

