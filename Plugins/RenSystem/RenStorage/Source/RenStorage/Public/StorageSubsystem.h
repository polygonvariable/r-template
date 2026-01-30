// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreStorage/Public/StorageProviderInterface.h"

// Generated Headers
#include "StorageSubsystem.generated.h"

// Forward Declarations
class UStorage;



/**
 * 
 */
UCLASS()
class UStorageSubsystem : public UGameInstanceSubsystem, public IStorageProviderInterface
{

	GENERATED_BODY()

public:

	bool ReadStorage(FName SlotId = "Default", int UserIndex = 0);
	bool UpdateStorage(FName SlotId = "Default", int UserIndex = 0);
	bool DoesStorageExist(FName SlotId = "Default", int UserIndex = 0);


	// ~ IStorageProviderInterface
	virtual USaveGame* GetStorage(TSubclassOf<USaveGame> StorageClass, FName Path) override;
	virtual bool SaveStorage(USaveGame* Storage, FName Path) override;
	// ~ End of IStorageProviderInterface

protected:

	TMap<FString, FString> IdMap;

	FName CurrentSlotId = NAME_None;
	int CurrentUserIndex = 0;

	UPROPERTY()
	TObjectPtr<UStorage> CurrentStorage;

	bool CreateNewStorage(FName SlotId, int UserIndex);
	void HandleGameInitialized();

	bool MakeStorageId(TSubclassOf<USaveGame> InStorageClass, FName InSlotName, int InUserIndex, FString& OutStorageId) const;

public:

	virtual USaveGame* GetLocalStorage() override;

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Object) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

