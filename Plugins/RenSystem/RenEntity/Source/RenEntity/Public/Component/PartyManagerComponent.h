// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers

// Generated Headers
#include "PartyManagerComponent.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UGameplayEffect;

struct FGameplayEventData;



/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FCharacterFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> Class;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> Attributes;

	bool IsValid() const;

};


/**
 *
 *
 */
UENUM()
enum class EAvatarState : uint8
{
	Alive,
	Dead
};


/**
 *
 *
 */
USTRUCT()
struct FAvatarHandle
{

	GENERATED_BODY()

public:

	FAvatarHandle() : AvatarState(EAvatarState::Dead) {}
	FAvatarHandle(EAvatarState InState) : AvatarState(InState) {}

	FDelegateHandle FallenHandle;
	FDelegateHandle RevivedHandle;

	EAvatarState AvatarState;

};


/**
 *
 *
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class UPartyManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UPartyManagerComponent(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintCallable, Server, Reliable, BlueprintAuthorityOnly)
	void SpawnParty();
	virtual void SpawnParty_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, BlueprintAuthorityOnly)
	void DespawnParty();
	virtual void DespawnParty_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, BlueprintAuthorityOnly)
	void SwitchPartyAvatar(int Index);
	virtual void SwitchPartyAvatar_Implementation(int Index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ACharacter*> GetPartyAvatars() const;

	UFUNCTION(BlueprintCallable, Server, Reliable, BlueprintAuthorityOnly)
	void KillParty(TSubclassOf<UGameplayEffect> EffectClass);
	virtual void KillParty_Implementation(TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Server, Reliable, BlueprintAuthorityOnly)
	void ReviveParty(TSubclassOf<UGameplayEffect> EffectClass);
	virtual void ReviveParty_Implementation(TSubclassOf<UGameplayEffect> EffectClass);


	// ~ UActorComponent
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

protected:

	UPROPERTY(EditAnywhere)
	FGameplayTag FallenTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag RevivedTag;

	UPROPERTY(EditAnywhere)
	TArray<FCharacterFragment> CharacterFragments;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitialAttributesEffect;

	UPROPERTY()
	TArray<TWeakObjectPtr<ACharacter>> PartyAvatars;

	UPROPERTY()
	TMap<TWeakObjectPtr<ACharacter>, FAvatarHandle> AvatarHandles;

	ACharacter* GetAliveAvatar() const;
	UAbilitySystemComponent* GetAvatarASC(ACharacter* Avatar) const;
	APlayerController* GetControllerWithAuthority() const;

	void SpawnAvatar(const FCharacterFragment& Fragment);
	void DespawnAvatar(ACharacter* Avatar);

	void RegisterAvatar(ACharacter* Avatar, UAbilitySystemComponent* ASC);
	void UnregisterAvatar(ACharacter* Avatar);

	void ApplyInitialAttributes(UAbilitySystemComponent* ASC, const TMap<FGameplayTag, float>& Attributes);
	void ApplyGEInternal(ACharacter* Avatar, TSubclassOf<UGameplayEffect> EffectClass, float Level);

	virtual void HandleAvatarFallen(FGameplayTag MatchingTag, const FGameplayEventData* Payload);
	virtual void HandleAvatarRevived(FGameplayTag MatchingTag, const FGameplayEventData* Payload);

};

