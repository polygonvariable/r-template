// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "PartySubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RCHARACTERPARTY_API

// Forward Declarations
class IStorageProvider;
class UPartyStorage;
class UPartyManagerComponent;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UPartySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	RSYSTEM_API UPartyStorage* GetPartyCollection();
	
	UFUNCTION(BlueprintCallable)
	RSYSTEM_API void RequestSpawnParty();

	RSYSTEM_API void RegisterManager(UPartyManagerComponent* Manager);
	RSYSTEM_API void UnregisterManager();

protected:

	UPROPERTY()
	TWeakObjectPtr<UPartyManagerComponent> ManagerComponent;

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static RSYSTEM_API UPartySubsystem* Get(UWorld* World);
	static RSYSTEM_API UPartySubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef RSYSTEM_API

