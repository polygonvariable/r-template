// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "PartyManagerComponent2.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UGameplayEffect;
class UCharacterAsset;
class URAssetManager;
class UPartyStorage;
class ARCharacterBase;


struct FGameplayEventData;




/**
 *
 *
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class UPartyManagerComponent2 : public UActorComponent
{

	GENERATED_BODY()

public:

	UPartyManagerComponent2(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintCallable)
	void SpawnParty();

	UFUNCTION(BlueprintCallable)
	void ClearParty();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

protected:

	void SpawnParty_Internal(const TArray<UCharacterAsset*>& Assets);

	void SpawnCharacter(UWorld* World, const UCharacterAsset* CharacterAsset);
	void RemoveCharacter(ARCharacterBase* Character);

	void RegisterCharacter(ARCharacterBase* Character);
	void UnregisterCharacter(ARCharacterBase* Character);

	void HandleCharacterDied();



	UPROPERTY()
	TArray<TWeakObjectPtr<ARCharacterBase>> PartyCharacters;


	UPROPERTY()
	TWeakObjectPtr<UPartyStorage> PartyStorage;

	UPROPERTY()
	URAssetManager* AssetManager;



	ACharacter* GetAliveCharacter() const;
	APlayerController* GetControllerWithAuthority() const;

private:

	FVector _PartySpawnLocation;
	FGuid _PartySpawnId;

};

