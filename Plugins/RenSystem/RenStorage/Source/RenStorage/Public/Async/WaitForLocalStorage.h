// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Kismet/BlueprintAsyncActionBase.h"

// Project Headers

// Generated Headers
#include "WaitForLocalStorage.generated.h"

// Forward Declarations
class UStorage;

struct FTaskResult;



/**
 *
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForLocalStorage : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleted, bool, bSuccess);
	UPROPERTY(BlueprintAssignable)
	FOnCompleted OnCompleted;

	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "InWorldContext", BlueprintInternalUseOnly = "true"))
	static UWaitForLocalStorage* WaitForLocalStorage(const UObject* InWorldContext, TSubclassOf<UStorage> InStorageClass, FGuid InStorageId);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	TWeakObjectPtr<const UObject> WorldContext;
    TSubclassOf<UStorage> StorageClass;
    FGuid StorageId;

	void OnStorageLoaded(const FTaskResult& Result);

    // ~ UBlueprintAsyncActionBase
	virtual void Activate() override;
	virtual UWorld* GetWorld() const override;
    // ~ End of UBlueprintAsyncActionBase

};

