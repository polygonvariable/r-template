// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Generated Headers
#include "ActorFreeListSubsystem.generated.h"

// Module Macros
#define RCORE_API RCOREPOOL_API



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UActorFreeListSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	RCORE_API AActor* AcquireFromList(TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner);
	RCORE_API void ReturnToList(AActor* Actor);
	RCORE_API void ClearList();

	template<class T>
	T* AcquireFromList(TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner)
	{
		return Cast<T>(AcquireFromList(ActorClass, Transform, Owner));
	}

protected:

	UPROPERTY()
	TMap<TSubclassOf<AActor>, TObjectPtr<AActor>> ActorList;


	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static RCORE_API UActorFreeListSubsystem* Get(UWorld* World);

};


// Module Macros
#undef RCORE_API

