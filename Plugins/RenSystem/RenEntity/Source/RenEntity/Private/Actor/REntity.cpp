// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/REntity.h"

// Engine Headers
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"



AREntity::AREntity() : Super()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (IsValid(Capsule))
	{
		Capsule->SetCapsuleHalfHeight(90.0f);
		Capsule->SetCapsuleRadius(35.0f);
	}

	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AREntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

