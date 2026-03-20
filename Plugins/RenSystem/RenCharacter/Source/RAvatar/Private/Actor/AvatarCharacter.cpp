// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/AvatarCharacter.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Settings/AvatarSettings.h"
#include "Settings/CharacterSettings.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarSubsystem.h"



AAvatarCharacter::AAvatarCharacter() : Super()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (IsValid(SpringArm))
	{
		SpringArm->SetupAttachment(GetRootComponent());
		SpringArm->TargetArmLength = 400.f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = false;
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		if (IsValid(Camera))
		{
			Camera->SetupAttachment(SpringArm);
		}
	}
	
	bUseControllerRotationYaw = false;
}

void AAvatarCharacter::CameraPan(FVector2D Axis)
{
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AAvatarCharacter::CameraZoom(float Delta, float Multiplier)
{
	if (IsValid(SpringArm))
	{
		float NewLength = SpringArm->TargetArmLength + (Delta * Multiplier);
		SpringArm->TargetArmLength = FMath::Clamp(NewLength, CameraMinZoom, CameraMaxZoom);
	}
}



void AAvatarCharacter::InitializeCharacter(const UCharacterAsset* CharacterAsset)
{
	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetWorld());
	if (!IsValid(CharacterAsset) || !IsValid(AvatarSubsystem))
	{
		return;
	}

	UAvatarStorage* AvatarCollection = AvatarSubsystem->GetAvatarCollection();
	if (!IsValid(AvatarCollection))
	{
		return;
	}

	FPrimaryAssetId AssetId = CharacterAsset->GetPrimaryAssetId();
	const FAvatarInstance* AvatarInstance = AvatarCollection->GetInstance(AssetId);
	if (AvatarInstance)
	{
		const UCharacterSettings* Settings = UCharacterSettings::Get();

		TMap<FGameplayTag, float> Attributes;
		Attributes.Add(Settings->AttributeHealthTag, AvatarInstance->Health);

		InitializeAttributes(Attributes);
		InitializeTags(Attributes);
	}

	AvatarStorage = TWeakObjectPtr<UAvatarStorage>(AvatarCollection);
}

