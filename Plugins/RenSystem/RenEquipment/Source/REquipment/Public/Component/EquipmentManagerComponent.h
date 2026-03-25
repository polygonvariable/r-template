// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Definition/AscensionData.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/AssetQuerySource.h"
#include "Interface/IActorFreeList.h"

// Generated Headers
#include "EquipmentManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class URAssetManager;
class URPrimaryDataAsset;
class UEquipmentStorage;
class UEquipmentSubsystem;
class UActorFreeListSubsystem;
class IAssetInstanceCollection;



USTRUCT(BlueprintType)
struct FEquipmentSpawnData
{

	GENERATED_BODY()

public:

	FEquipmentSpawnData() {};

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<URPrimaryDataAsset> DataAsset;

#endif

	UPROPERTY()
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentSlot;

	UPROPERTY(EditAnywhere)
	FGuid EquipmentId;

	UPROPERTY(EditAnywhere)
	FAscensionData AscensionData;


	void Reset();

};


USTRUCT(BlueprintType)
struct FEquipmentSpawnedData
{

	GENERATED_BODY()

public:

	FEquipmentSpawnedData() {};

	UPROPERTY(VisibleAnywhere)
	FEquipmentSpawnData Data;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEquipmentActor> Actor;

};



/**
 *
 *
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class UEquipmentManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<FEquipmentSpawnData> SpawnData;


	UFUNCTION(BlueprintCallable)
	void SpawnEquipment();

	UFUNCTION(BlueprintCallable)
	void RemoveEquipment();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	UPROPERTY(VisibleAnywhere)
	FGuid OwnerId;

	UPROPERTY(EditAnywhere)
	EAssetQuerySource EquipmentSource = EAssetQuerySource::Instance;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Spawn Data (Editor)"))
	TArray<FEquipmentSpawnData> SpawnDataEd;

#endif
	
	UPROPERTY()
	URAssetManager* AssetManager = nullptr;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<AEquipmentActor>> SpawnedEquipment;

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> EquipmentStorage = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UActorFreeListSubsystem> ActorFreeList = nullptr;


	void SyncOwnerEquipment(const FGuid& InOwnerId);


	void FetchSpawnDataFromInstance();
	void SpawnEquipment_Internal();

private:

	FGuid _EquipmentSpawnId;

};


UCLASS(Abstract)
class AEquipmentActor : public AActor, public IActorFreeList
{

	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const URPrimaryDataAsset> EquipmentAsset;

	UPROPERTY(VisibleAnywhere)
	FGuid OwnerId;

	UPROPERTY(VisibleAnywhere)
	FEquipmentSpawnData SpawnData;


	void PreInitializeEquipment();
	void InitializeEquipment();
	void DeinitializeEquipment();

	void RefreshEquipment();

	// ~ IActorFreeList
	virtual AActor* GetNextNode() const override;
	virtual void SetNextNode(AActor* Node) override;
	// ~ End of IActorFreeList
	
	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

protected:

	IAssetInstanceCollection* InstanceCollection;

private:

	UPROPERTY()
	AEquipmentActor* _NextNode = nullptr;

};