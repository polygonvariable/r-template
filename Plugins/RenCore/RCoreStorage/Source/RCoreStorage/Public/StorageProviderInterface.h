// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "GameFramework/SaveGame.h"

// Generated Headers
#include "StorageProviderInterface.generated.h"

// Forward Declarations



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

	UE_DEPRECATED(5.4, "Use GetLocalStorage instead")
	virtual USaveGame* GetLocalStorage() = 0;

	virtual USaveGame* GetStorage(TSubclassOf<USaveGame> StorageClass, FName Path) = 0;
	virtual bool SaveStorage(USaveGame* Storage, FName Path) = 0;

	template<typename T>
	T* GetStorage(FName Path)
	{
		return Cast<T>(GetStorage(T::StaticClass(), Path));
	}

};

