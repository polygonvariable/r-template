// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/RCharacterBase.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Settings/CharacterSettings.h"



ARCharacterBase::ARCharacterBase() : Super()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (IsValid(Capsule))
	{
		Capsule->SetCapsuleHalfHeight(90.0f);
		Capsule->SetCapsuleRadius(35.0f);
	}

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
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	PrimaryActorTick.bStartWithTickEnabled = false;
}


bool ARCharacterBase::IsAlive() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasMatchingGameplayTag(UCharacterSettings::Get()->StateAliveTag);
}

void ARCharacterBase::InitializeCharacter(const UCharacterAsset* CharacterAsset)
{
}

void ARCharacterBase::InitializeAttributes(const TMap<FGameplayTag, float>& Attributes)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InitialAttributeEffectClass, 1.0f, Context);

	if (Spec.IsValid())
	{
		Spec.Data->SetByCallerTagMagnitudes = Attributes;
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void ARCharacterBase::InitializeTags(const TMap<FGameplayTag, float>& Attributes)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	const UCharacterSettings* Settings = UCharacterSettings::Get();

	int Health = Attributes.FindRef(Settings->AttributeHealthTag);
	FGameplayTag HealthTag = (Health > 0) ? Settings->StateAliveTag : Settings->StateDeadTag;

	ASC->AddLooseGameplayTag(HealthTag);
}

void ARCharacterBase::DeinitializeCharacter()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (IsValid(MovementComponent))
	{
		MovementComponent->SetMovementMode(MOVE_None);
	}
}



void ARCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


UAbilitySystemComponent* ARCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ARCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ARCharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool ARCharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasAllMatchingGameplayTags(TagContainer);
}

bool ARCharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return false;
	}
	return ASC->HasAnyMatchingGameplayTags(TagContainer);
}


void ARCharacterBase::DirectionalMove_Implementation(const FVector& Direction)
{
	FRotator Rotation = GetControlRotation();
	FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Rotation.Yaw, Rotation.Roll));
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Rotation.Yaw, 0.0f));

	AddMovementInput(RightVector, Direction.X, false);
	AddMovementInput(ForwardVector, Direction.Y, false);
}

bool ARCharacterBase::IsMoving(float Threshold) const
{
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size2D();

	return Speed > Threshold;
}

