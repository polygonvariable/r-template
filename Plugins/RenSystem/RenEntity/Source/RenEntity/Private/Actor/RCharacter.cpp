// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/RCharacter.h"

// Engine Headers
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "Log/LogMacro.h"
#include "RenAbility/Public/Attributes/DamageSet.h"
#include "RenAbility/Public/Library/AbilitySystemLibrary.h"
#include "RenAbility/Public/Component/RAbilitySystemComponent.h"




ARCharacter::ARCharacter() : Super()
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
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (IsValid(Camera))
	{
		Camera->SetupAttachment(SpringArm);
	}
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (IsValid(MovementComponent))
	{
		MovementComponent->GravityScale = 1.75f;
		MovementComponent->MaxAcceleration = 1500.0f;
		MovementComponent->bUseSeparateBrakingFriction = true;

		MovementComponent->MaxWalkSpeed = 500.0f;
		MovementComponent->MinAnalogWalkSpeed = 20.0f;
		MovementComponent->BrakingDecelerationWalking = 2000.0f;

		MovementComponent->BrakingDecelerationFalling = 1500.0f;
		MovementComponent->AirControl = 0.35f;

		MovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		MovementComponent->bOrientRotationToMovement = true;
	}

	bUseControllerRotationYaw = false;
}

void ARCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ARCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool ARCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasAllMatchingGameplayTags(TagContainer);
}

bool ARCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasAnyMatchingGameplayTags(TagContainer);
}



UAbilitySystemComponent* ARCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARCharacter::CameraPan(FVector2D Axis)
{
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void ARCharacter::CameraZoom(float Delta, float Multiplier)
{
	if (!IsValid(SpringArm))
	{
		return;
	}
	
	float CurrentLength = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = FMath::Clamp(CurrentLength + (Delta * Multiplier), CameraMinZoom, CameraMaxZoom);
}


void ARCharacter::DirectionalMove_Implementation(const FVector& Direction)
{
	FRotator Rotation = GetControlRotation();
	FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Rotation.Yaw, Rotation.Roll));
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Rotation.Yaw, 0.0f));

	AddMovementInput(RightVector, Direction.X, false);
	AddMovementInput(ForwardVector, Direction.Y, false);
}

void ARCharacter::DealDamage(TSubclassOf<UGameplayEffect> EffectClass, AActor* Target, AActor* EffectCauser)
{
	// If the target has lazy loading of ASC, try to call the function
	// to load the ASC and then apply the effect here

	/*
	UAbilitySystemComponent* TargetASC = Target->GetComponentByClass<UAbilitySystemComponent>();

	if (IsValid(RAbilitySystemComponent) && IsValid(TargetASC))
	{
		TArray<TWeakObjectPtr<AActor>> ContextActors;
		for (AActor* Actor : OwnedActors)
		{
			ContextActors.Add(TWeakObjectPtr<AActor>(Actor));
		}

		UAbilitySystemLibrary::ApplyGameplayEffectToTarget(Target, this, this, EffectClass, 1.0f, [&](FGameplayEffectContextHandle& EffectContext) {
			EffectContext.AddActors(ContextActors);
			EffectContext.AddOrigin(GetActorLocation());
		});

		TArray<TWeakObjectPtr<AActor>> TargetActors;
		TargetActors.Add(TWeakObjectPtr<AActor>(Target));

		FGameplayEffectContextHandle EffectContext = RAbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddActors(TargetActors);
		EffectContext.AddOrigin(GetActorLocation());
		EffectContext.AddInstigator(this, EffectCauser);

		if (!EffectContext.IsValid())
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create effect context"));
			return;
		}

		FGameplayEffectSpecHandle SpecHandle = RAbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
		if (!SpecHandle.IsValid())
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create effect spec handle"));
			return;
		}
		// Damage type
		// SpecHandle.Data->AddDynamicAssetTag();

		RAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}*/
}

void ARCharacter::CancelAbility(FGameplayTagContainer WithTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTags, nullptr, nullptr);
}



bool ARCharacter::IsMoving(float Threshold) const
{
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size2D();

	return Speed > Threshold;
}

