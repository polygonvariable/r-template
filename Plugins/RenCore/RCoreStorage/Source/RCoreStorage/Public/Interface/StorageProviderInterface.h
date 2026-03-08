// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "Definition/StorageHandle.h"

// Generated Headers
#include "StorageProviderInterface.generated.h"

// Forward Declarations
class UStorage;



UINTERFACE(MinimalAPI, NotBlueprintable)
class UStorageProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 * 
 */
class RCORESTORAGE_API IStorageProviderInterface
{

	GENERATED_BODY()

public:

	virtual UStorage* GetStorage(const FGuid& StorageId) = 0;

	template<typename T>
	T* GetStorage(const FGuid& StorageId)
	{
		return Cast<T>(GetStorage(StorageId));
	}

	virtual void LoadStorage(FStorageHandle&& Handle) = 0;
	virtual void SaveStorage(const FGuid& StorageId) = 0;

public:

	static IStorageProviderInterface* Get(UWorld* World);
	static IStorageProviderInterface* Get(UGameInstance* GameInstance);

};

