// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "TaskDefinition.h"

// Generated Headers
#include "InventoryStorageSubsystem.generated.h"

// Forward Declarations
class USaveGame;
class UInventoryStorage;
class IStorageProviderInterface;



/**
 *
 * 
 *
 */
UCLASS(MinimalAPI)
class UInventoryStorageSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	USaveGame* GetStorage(const FGuid& StorageId);

	template<typename T>
	T* GetStorage(const FGuid& StorageId)
	{
		return Cast<T>(GetStorage(StorageId));
	}

	void WaitForStorage(const FGuid& StorageId, FTaskCallback Callback);

	virtual FGuid GetDefaultStorageId() const;

protected:

	virtual FString GetRootPath() const;
	virtual void GetDefaultQuery(TArray<FString>& OutQuery);

	void MakeQueryString(const TArray<FString>& InQuery, FString& OutString) const;
	void MakePath(const FGuid& StorageId, FString& OutString);


	UPROPERTY()
	TMap<FGuid, TObjectPtr<USaveGame>> StorageCollection;

	TMap<FGuid, FString> PathCollection;

	TWeakInterfacePtr<IStorageProviderInterface> StorageProviderInterface;


	virtual void LoadStorage(const FGuid& StorageId, FTaskCallback Callback);
	virtual void SaveStorage(FTaskCallback Callback);


	void OnGameLoaded();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

