// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Http.h"

// Project Headers
#include "Interface/StorageProviderInterface.h"
#include "Definition/StorageHandle.h"
#include "Definition/TaskType.h"

// Generated Headers
#include "StorageSubsystem.generated.h"

// Forward Declarations
class UStorage;
class UStorage;



/**
 * 
 */
UCLASS()
class UStorageSubsystem : public UGameInstanceSubsystem, public IStorageProviderInterface
{

	GENERATED_BODY()

public:

	// ~ IStorageProviderInterface
	virtual UStorage* GetStorage(const FGuid& StorageId) override;
	virtual void LoadStorage(FStorageHandle&& Handle) override;
	virtual void SaveStorage(const FGuid& StorageId) override;
	// ~ End of IStorageProviderInterface

protected:

	bool bNetLoad = false;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UStorage>> StorageCollection;


	bool MakeStorageId(TSubclassOf<UStorage> InStorageClass, const FString& InSlotName, int InUserIndex, FString& OutStorageId) const;
	void GetDefaultQuery(const FGuid& StorageId, TSharedPtr<FJsonObject>& QueryJson);
	void SerializeQuery(TSharedPtr<FJsonObject>& QueryJson, FString& OutString);


	UStorage* CreateStorage_Internal(TSubclassOf<UStorage> StorageClass, const FGuid& StorageId);
	bool SaveStorage_Internal(UStorage* Storage, const FGuid& StorageId);


	void LoadStorage_Network(FStorageHandle Handle);
	void LoadStorage_NetworkResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded, FStorageHandle Handle);
	UStorage* LoadStorage_Internal(const FGuid& StorageId, TSubclassOf<UStorage> StorageClass);


	void OnPreGameInitialized();
	void SaveAllStorages();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Object) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

