// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"

// Generated Headers
#include "RCharacter.generated.h"

// Module Macros
#define RSYSTEM_API RENTITY_API

// Forward Declarations
class UAbilitySystemComponent;



/**
 *
 * 
 * 
 */
UCLASS(Abstract)
class RSYSTEM_API ARCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{

	GENERATED_BODY()

public:

	ARCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


	// ~ ACharacter
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ~ End of ACharacter

	// ~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~ End of IAbilitySystemInterface

	// ~ IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// ~ End of IGameplayTagAssetInterface

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void DirectionalMove(const FVector& Direction);
	virtual void DirectionalMove_Implementation(const FVector& Direction);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMoving(float Threshold = 0.1f) const;

};


// Module Macros
#undef RSYSTEM_API

