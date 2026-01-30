// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Generated Headers
#include "REntity.generated.h"



/**
 * 
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class AREntity : public ACharacter
{

	GENERATED_BODY()

public:

	AREntity();

	// ~ AREntity
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ~ End of AREntity

};

