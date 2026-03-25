// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EquipmentManagerComponent.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Interface/IAssetInstanceData.h"
#include "Interface/IEquipmentProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Storage/EquipmentStorage.h"
#include "Subsystem/EquipmentSubsystem.h"
#include "Settings/EquipmentSettings.h"
#include "Subsystem/ActorFreeListSubsystem.h"
#include "Interface/IAssetInstanceCollection.h"
#include "Interface/IAssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Interface/IAscensionInstanceData.h"



void FEquipmentSpawnData::Reset()
{
#if WITH_EDITOR
	DataAsset.Reset();
#endif

	EquipmentId.Invalidate();
	AscensionData.Reset();
}

UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UEquipmentManagerComponent::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	SpawnData.Empty();

	for (FEquipmentSpawnData Data : SpawnDataEd)
	{
		URPrimaryDataAsset* DataAsset = Data.DataAsset.LoadSynchronous();
		if (!IsValid(DataAsset) || !DataAsset->Implements<UEquipmentProvider>())
		{
			LOG_ERROR(LogTemp, TEXT("Invalid asset or asset does not implement EquipmentProvider"));
			continue;
		}

		Data.AssetId = DataAsset->GetPrimaryAssetId();
		SpawnData.Add(Data);
	}
#endif
}

void UEquipmentManagerComponent::SyncOwnerEquipment(const FGuid& InOwnerId)
{
	if (OwnerId == InOwnerId)
	{
		RemoveEquipment();
		SpawnEquipment();
	}
}

void UEquipmentManagerComponent::BeginPlay()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	ActorFreeList = UActorFreeListSubsystem::Get(GetWorld());

	IAssetInstanceData* InstanceData = Cast<IAssetInstanceData>(GetOwner());
	if (InstanceData)
	{
		OwnerId = InstanceData->GetInstanceId();
	}

	if (EquipmentSource == EAssetQuerySource::Instance)
	{
		EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
		if (IsValid(EquipmentSubsystem))
		{
			EquipmentSubsystem->OnSyncEquipment.AddUObject(this, &UEquipmentManagerComponent::SyncOwnerEquipment);
			EquipmentStorage = EquipmentSubsystem->GetEquipment();
		}
	}

	Super::BeginPlay();
}

void UEquipmentManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveEquipment();

	if (IsValid(EquipmentStorage))
	{
		EquipmentStorage->OnStorageUpdated.RemoveAll(this);
	}

	if (IsValid(AssetManager) && _EquipmentSpawnId.IsValid())
	{
		AssetManager->CancelFetch(_EquipmentSpawnId);
	}
	AssetManager = nullptr;

	Super::EndPlay(EndPlayReason);
}

void UEquipmentManagerComponent::SpawnEquipment()
{
	if (EquipmentSource == EAssetQuerySource::Instance)
	{
		FetchSpawnDataFromInstance();
	}

	if (!IsValid(AssetManager) || SpawnData.Num() == 0)
	{
		LOG_ERROR(LogTemp, TEXT("ActorFreeList, AssetManager is invalid or EquipmentSpawnData is empty"));
		return;
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();

	AssetManager->CancelFetch(_EquipmentSpawnId);

	_EquipmentSpawnId = FGuid::NewGuid();

	TArray<FPrimaryAssetId> AssetIds;
	for (const FEquipmentSpawnData& Data : SpawnData)
	{
		AssetIds.Add(Data.AssetId);
	}

	const TArray<FName> AssetBundles = Settings->EquipmentBundles;

	TFuture<FLatentLoadedAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(_EquipmentSpawnId, AssetIds, AssetBundles);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Failed to initialize load equipment assets"));
		return;
	}

	TWeakObjectPtr<UEquipmentManagerComponent> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<URPrimaryDataAsset>& Result)
		{
			UEquipmentManagerComponent* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->SpawnEquipment_Internal();
			}
		}
	);
}

void UEquipmentManagerComponent::FetchSpawnDataFromInstance()
{
	if (!IsValid(EquipmentStorage))
	{
		LOG_ERROR(LogTemp, TEXT("EquipmentStorage is invalid"));
		return;
	}

	SpawnData.Empty();

	const TMap<FGameplayTag, FEquipmentKey>* OwnedEquipment = EquipmentStorage->GetOwnedEquipment(OwnerId);
	if (!OwnedEquipment)
	{
		LOG_ERROR(LogTemp, TEXT("OwnedEquipment is invalid"));
		return;
	}

	for (const TPair<FGameplayTag, FEquipmentKey>& Kv : *OwnedEquipment)
	{
		const FEquipmentKey& EquipmentKey = Kv.Value;

		FEquipmentSpawnData Data;
		Data.AssetId = EquipmentKey.AssetId;
		Data.EquipmentId = EquipmentKey.AssetInstanceId;
		Data.EquipmentSlot = Kv.Key;

		SpawnData.Add(Data);
	}
}


void UEquipmentManagerComponent::RemoveEquipment()
{
	for (const TPair<FGameplayTag, TObjectPtr<AEquipmentActor>>& Kv : SpawnedEquipment)
	{
		AEquipmentActor* Actor = Kv.Value;
		if (IsValid(Actor))
		{
			Actor->DeinitializeEquipment();
			ActorFreeList->ReturnToList(Actor);
		}
	}

	SpawnedEquipment.Empty();
}

void UEquipmentManagerComponent::SpawnEquipment_Internal()
{
	UWorld* World = GetWorld();
	FTransform SpawnTransform;

	for (const FEquipmentSpawnData& Data : SpawnData)
	{
		URPrimaryDataAsset* Asset = Cast<URPrimaryDataAsset>(AssetManager->GetPrimaryAssetObject(Data.AssetId));
		const IEquipmentProvider* EquipmentProvider = Cast<IEquipmentProvider>(Asset);
		if (!EquipmentProvider)
		{
			LOG_ERROR(LogTemp, TEXT("Asset does not implement IEquipmentProvider"));
			continue;
		}

		const FGameplayTag& EquipmentSlot = Data.EquipmentSlot;
		if (SpawnedEquipment.Contains(EquipmentSlot))
		{
			LOG_ERROR(LogTemp, TEXT("Slot is already in use"));
			continue;
		}
		
		UClass* EquipmentClass = EquipmentProvider->GetEquipmentClass().Get();
		if (!IsValid(EquipmentClass) || !EquipmentClass->IsChildOf(AEquipmentActor::StaticClass()))
		{
			LOG_ERROR(LogTemp, TEXT("Class is invalid or not child of EquipmentActor"));
			continue;
		}

		AEquipmentActor* EquipmentActor = ActorFreeList->AcquireFromList<AEquipmentActor>(EquipmentClass, SpawnTransform, GetOwner());
		if (!IsValid(EquipmentActor))
		{
			LOG_ERROR(LogTemp, TEXT("Spawned character is invalid"));
			return;
		}

		EquipmentActor->EquipmentAsset = Asset;
		EquipmentActor->OwnerId = OwnerId;
		EquipmentActor->SpawnData = Data;

		if (!EquipmentActor->HasActorBegunPlay())
		{
			EquipmentActor->PreInitializeEquipment();
			EquipmentActor->FinishSpawning(SpawnTransform);
		}

		EquipmentActor->InitializeEquipment();

		SpawnedEquipment.Add(EquipmentSlot, EquipmentActor);
	}
}
































AActor* AEquipmentActor::GetNextNode() const
{
	return _NextNode;
}

void AEquipmentActor::SetNextNode(AActor* Node)
{
	_NextNode = Cast<AEquipmentActor>(Node);
}

void AEquipmentActor::PreInitializeEquipment()
{
	IAssetInstanceCollectionProvider* CollectionProvider = FAssetInstanceUtil::GetAssetInterchange(GetWorld(), SpawnData.AssetId);
	if (!CollectionProvider)
	{
		return;
	}

	FName CollectionId = CollectionProvider->GetDefaultCollectionId();
	InstanceCollection = CollectionProvider->GetInstanceCollection(CollectionId);
	if (!InstanceCollection)
	{
		return;
	}

	InstanceCollection->GetOnAssetInstanceCollectionUpdated().AddUObject(this, &AEquipmentActor::RefreshEquipment);

	IAscensionInstanceData* AscensionInterface = Cast<IAscensionInstanceData>(InstanceCollection);
	if (!AscensionInterface)
	{
		return;
	}
	
	const FAscensionData* AscensionData = AscensionInterface->GetAscensionInstance(SpawnData.AssetId, SpawnData.EquipmentId);
	if (!AscensionData)
	{
		return;
	}

	SpawnData.AscensionData = *AscensionData;
}

void AEquipmentActor::InitializeEquipment()
{
	AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AEquipmentActor::DeinitializeEquipment()
{
	if (InstanceCollection)
	{
		InstanceCollection->GetOnAssetInstanceCollectionUpdated().RemoveAll(this);
	}

	EquipmentAsset = nullptr;
	OwnerId.Invalidate();
	SpawnData.Reset();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AEquipmentActor::RefreshEquipment()
{
	IAscensionInstanceData* AscensionInterface = Cast<IAscensionInstanceData>(InstanceCollection);
	if (!AscensionInterface)
	{
		return;
	}

	const FAscensionData* AscensionData = AscensionInterface->GetAscensionInstance(SpawnData.AssetId, SpawnData.EquipmentId);
	if (!AscensionData)
	{
		return;
	}

	SpawnData.AscensionData = *AscensionData;
}

void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipmentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeEquipment();

	Super::EndPlay(EndPlayReason);
}
