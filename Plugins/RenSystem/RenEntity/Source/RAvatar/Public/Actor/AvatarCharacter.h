// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/RCharacter.h"

// Generated Headers
#include "AvatarCharacter.generated.h"

// Forward Declarations
class USpringArmComponent;
class UCameraComponent;



/**
 *
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class AAvatarCharacter : public ARCharacter
{

	GENERATED_BODY()

public:

	AAvatarCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCameraComponent> Camera;

protected:

	UPROPERTY(EditAnywhere)
	float CameraMinZoom = 100.0f;

	UPROPERTY(EditAnywhere)
	float CameraMaxZoom = 2000.0f;


	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraPan(FVector2D Axis);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraZoom(float Delta, float Multiplier = 5.0f);

};

