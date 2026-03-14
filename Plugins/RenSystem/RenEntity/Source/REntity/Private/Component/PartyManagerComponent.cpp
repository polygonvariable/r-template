// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/PartyManagerComponent.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "Log/LogMacro.h"
#include "RenAbility/Public/Library/AbilitySystemLibrary.h"



bool FCharacterFragment::IsValid() const
{
	return Class != nullptr;
}


UPartyManagerComponent::UPartyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UPartyManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UPartyManagerComponent::SpawnParty_Implementation()
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("PC is invalid"));
		return;
	}

	if (PartyAvatars.Num() > 0 || AvatarHandles.Num() > 0)
	{
		LOG_ERROR(LogTemp, TEXT("Party is already spawned or avatar handles are not cleared, despawn first"));
		return;
	}

	for (const FCharacterFragment& Fragment : CharacterFragments)
	{
		SpawnAvatar(Fragment);
	}

	ACharacter* FirstAvatar = GetAliveAvatar();
	if (IsValid(FirstAvatar))
	{
		PC->Possess(FirstAvatar);
	}
}

void UPartyManagerComponent::DespawnParty_Implementation()
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("PC is invalid"));
		return;
	}

	PC->UnPossess();

	for (TWeakObjectPtr<ACharacter>& Avatar : PartyAvatars)
	{
		DespawnAvatar(Avatar.Get());
	}

	PartyAvatars.Empty();
	AvatarHandles.Empty();

	LOG_WARNING(LogTemp, TEXT("Party is despawned"));
}


void UPartyManagerComponent::SwitchPartyAvatar_Implementation(int Index)
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("PC is invalid"));
		return;
	}

	if (!PartyAvatars.IsValidIndex(Index))
	{
		LOG_ERROR(LogTemp, TEXT("Index is out of range"));
		return;
	}

	ACharacter* Avatar = PartyAvatars[Index].Get();
	if (!IsValid(Avatar))
	{
		LOG_ERROR(LogTemp, TEXT("Avatar is invalid"));
		return;
	}

	const FAvatarHandle* Handle = AvatarHandles.Find(Avatar);
	if (!Handle || Handle->AvatarState == EAvatarState::Dead)
	{
		LOG_ERROR(LogTemp, TEXT("Handle is invalid or avatar is dead"));
		return;
	}

	APawn* CurrentAvatar = PC->GetPawn();
	if (CurrentAvatar == Avatar)
	{
		LOG_WARNING(LogTemp, TEXT("Avatar is already possessed"));
		return;
	}
	
	FRotator OldRotation = CurrentAvatar->GetActorRotation();
	FVector OldLocation = CurrentAvatar->GetActorLocation();

	Avatar->SetActorRotation(OldRotation);
	PC->Possess(Avatar);
	Avatar->SetActorLocation(OldLocation);
}


TArray<ACharacter*> UPartyManagerComponent::GetPartyAvatars() const
{
	TArray<ACharacter*> Result;
	for (const TWeakObjectPtr<ACharacter>& Avatar : PartyAvatars)
	{
		Result.Add(Avatar.Get());
	}
	return Result;
}


void UPartyManagerComponent::KillParty_Implementation(TSubclassOf<UGameplayEffect> EffectClass)
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("PC is invalid"));
		return;
	}

	for (TWeakObjectPtr<ACharacter> Character : PartyAvatars)
	{
		ACharacter* Avatar = Character.Get();
		FAvatarHandle* Handle = AvatarHandles.Find(Avatar);
		if (!IsValid(Avatar) || !Handle || Handle->AvatarState == EAvatarState::Dead)
		{
			continue;
		}

		ApplyGEInternal(Avatar, EffectClass, 1.0f);
	}

	ACharacter* Avatar = GetAliveAvatar();
	if (IsValid(Avatar))
	{
		PC->Possess(Avatar);
	}
}

void UPartyManagerComponent::ReviveParty_Implementation(TSubclassOf<UGameplayEffect> EffectClass)
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("Spawning avatar requires authority or PC is invalid"));
		return;
	}

	for (TWeakObjectPtr<ACharacter> Character : PartyAvatars)
	{
		ACharacter* Avatar = Character.Get();
		FAvatarHandle* Handle = AvatarHandles.Find(Avatar);
		if (!IsValid(Avatar) || !Handle || Handle->AvatarState == EAvatarState::Alive)
		{
			continue;
		}

		ApplyGEInternal(Avatar, EffectClass, 1.0f);
	}

	ACharacter* Avatar = GetAliveAvatar();
	if (IsValid(Avatar))
	{
		PC->Possess(Avatar);
	}
}


ACharacter* UPartyManagerComponent::GetAliveAvatar() const
{
	ACharacter* Result = nullptr;

	for (const TPair<TWeakObjectPtr<ACharacter>, FAvatarHandle>& Pair : AvatarHandles)
	{
		if (Pair.Value.AvatarState == EAvatarState::Alive)
		{
			Result = Pair.Key.Get();
			break;
		}
	}

	return Result;
}

UAbilitySystemComponent* UPartyManagerComponent::GetAvatarASC(ACharacter* Avatar) const
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Avatar);
	if (!AbilitySystemInterface)
	{
		return nullptr;
	}

	return AbilitySystemInterface->GetAbilitySystemComponent();
}

APlayerController* UPartyManagerComponent::GetControllerWithAuthority() const
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole != ENetRole::ROLE_Authority || !IsValid(PC))
	{
		return nullptr;
	}
	return PC;
}


void UPartyManagerComponent::SpawnAvatar(const FCharacterFragment& Fragment)
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !Fragment.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("World, fragment is invalid"));
		return;
	}
	
	FTransform SpawnTransform;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACharacter* Avatar = World->SpawnActor<ACharacter>(Fragment.Class, SpawnTransform, SpawnParameters);
	if (!IsValid(Avatar))
	{
		LOG_ERROR(LogTemp, TEXT("Spawned avatar is invalid"));
		return;
	}

	UAbilitySystemComponent* ASC = GetAvatarASC(Avatar);
	if (!IsValid(ASC))
	{
		LOG_ERROR(LogTemp, TEXT("ASC is invalid"));
		return;
	}

	PartyAvatars.Add(Avatar);

	RegisterAvatar(Avatar, ASC);

	ApplyInitialAttributes(ASC, Fragment.Attributes);
}

void UPartyManagerComponent::DespawnAvatar(ACharacter* Avatar)
{
	if (!IsValid(Avatar))
	{
		LOG_ERROR(LogTemp, TEXT("Avatar is invalid"));
		return;
	}

	UnregisterAvatar(Avatar);

	Avatar->Destroy();
}

void UPartyManagerComponent::RegisterAvatar(ACharacter* Avatar, UAbilitySystemComponent* ASC)
{
	if (!IsValid(Avatar) || !IsValid(ASC))
	{
		LOG_ERROR(LogTemp, TEXT("Avatar, ASC is invalid"));
		return;
	}

	FAvatarHandle Handle(EAvatarState::Dead);
	Handle.FallenHandle = ASC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(FallenTag), FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UPartyManagerComponent::HandleAvatarFallen));
	Handle.RevivedHandle = ASC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(RevivedTag), FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UPartyManagerComponent::HandleAvatarRevived));

	AvatarHandles.Add(Avatar, Handle);
}

void UPartyManagerComponent::UnregisterAvatar(ACharacter* Avatar)
{
	UAbilitySystemComponent* ASC = GetAvatarASC(Avatar);
	if (!IsValid(ASC))
	{
		LOG_ERROR(LogTemp, TEXT("ASC is invalid"));
		return;
	}

	FAvatarHandle* Handle = AvatarHandles.Find(Avatar);
	if (!Handle)
	{
		LOG_ERROR(LogTemp, TEXT("Handle is invalid"));
		return;
	}

	ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(FallenTag), Handle->FallenHandle);
	ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(RevivedTag), Handle->RevivedHandle);

	Handle->FallenHandle.Reset();
	Handle->RevivedHandle.Reset();

	AvatarHandles.Remove(Avatar);
}


void UPartyManagerComponent::ApplyInitialAttributes(UAbilitySystemComponent* ASC, const TMap<FGameplayTag, float>& Attributes)
{
	if (!IsValid(ASC))
	{
		LOG_ERROR(LogTemp, TEXT("ASC or fragment is invalid"));
		return;
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InitialAttributesEffect, 1.0f, Context);

	if (Spec.IsValid())
	{
		Spec.Data->SetByCallerTagMagnitudes = Attributes;
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void UPartyManagerComponent::ApplyGEInternal(ACharacter* Avatar, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	if (!IsValid(Avatar) || !IsValid(EffectClass))
	{
		LOG_ERROR(LogTemp, TEXT("Avatar, GE is invalid"));
		return;
	}

	UAbilitySystemComponent* ASC = GetAvatarASC(Avatar);
	if (!IsValid(ASC))
	{
		LOG_ERROR(LogTemp, TEXT("ASC is invalid"));
		return;
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(EffectClass, Level, Context);
	if (Spec.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}


void UPartyManagerComponent::HandleAvatarFallen(FGameplayTag MatchingTag, const FGameplayEventData* Payload)
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!Payload || !IsValid(PC))
	{
		LOG_ERROR(LogTemp, TEXT("Payload or PC is invalid"));
		return;
	}

	const ACharacter* Avatar = Cast<ACharacter>(Payload->Target.Get());
	FAvatarHandle* Handle = AvatarHandles.Find(Avatar);
	if (!IsValid(Avatar) || !PartyAvatars.Contains(Avatar) || !Handle)
	{
		LOG_ERROR(LogTemp, TEXT("Avatar is invalid or doesn't exist in party or states"));
		return;
	}

	Handle->AvatarState = EAvatarState::Dead;

	if (PC->GetPawn() == Avatar)
	{
		PC->UnPossess();
	}

	// get next possible alive avatar and possess it
	ACharacter* AliveAvatar = GetAliveAvatar();
	if (!IsValid(AliveAvatar))
	{
		LOG_ERROR(LogTemp, TEXT("No alive avatars"));
		return;
	}

	FRotator OldRotation = Avatar->GetActorRotation();
	FVector OldLocation = Avatar->GetActorLocation();

	AliveAvatar->SetActorRotation(OldRotation);
	PC->Possess(AliveAvatar);
	AliveAvatar->SetActorLocation(OldLocation);
}

void UPartyManagerComponent::HandleAvatarRevived(FGameplayTag MatchingTag, const FGameplayEventData* Payload)
{
	const ACharacter* Avatar = Cast<ACharacter>(Payload->Target.Get());
	FAvatarHandle* Handle = AvatarHandles.Find(Avatar);
	if (!IsValid(Avatar) || !PartyAvatars.Contains(Avatar) || !Handle)
	{
		LOG_ERROR(LogTemp, TEXT("Avatar is invalid or doesn't exist in party or states"));
		return;
	}

	Handle->AvatarState = EAvatarState::Alive;
}

