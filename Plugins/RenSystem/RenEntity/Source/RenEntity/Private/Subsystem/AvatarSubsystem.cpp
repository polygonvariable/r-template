// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AvatarSubsystem.h"

// Engine Headers

// Project Headers
#include "AssetCollection.h"
#include "AvatarAsset.h"
#include "LatentDelegates.h"
#include "LogCategory.h"
#include "LogMacro.h"
#include "RCoreAssetManager/Private/RAssetManager.inl"
#include "StorageProviderInterface.h"
#include "SubsystemUtils.h"

#include "RenEntity/Public/EntityPrimaryAsset.h"
#include "Storage/EntityStorage.h"

#include "Interface/AssetDecompose.h"
#include "Interface/AssetDecomposeInterface.h"



TMap<FPrimaryAssetId, FAvatarRecord>* UAvatarSubsystem::GetMutableAvatarRecords() const
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return nullptr;
	}
	return &Storage->Records;
}




const TMap<FPrimaryAssetId, FAvatarRecord>* UAvatarSubsystem::GetAvatarRecords() const
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return nullptr;
	}
	return &Storage->Records;
}

const FAvatarRecord* UAvatarSubsystem::GetAvatarRecord(const FPrimaryAssetId& AvatarId) const
{
	const TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return nullptr;
	}

	return Records->Find(AvatarId);
}

FAvatarRecord* UAvatarSubsystem::GetMutableAvatarRecord(const FPrimaryAssetId& AvatarId) const
{
	TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return nullptr;
	}

	return Records->Find(AvatarId);
}

bool UAvatarSubsystem::AddAvatar(const FPrimaryAssetId& AvatarId)
{
	TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return false;
	}

	if (Records->Contains(AvatarId))
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar already exists"));
		return false;
	}

	FAvatarRecord NewRecord;

	Records->Add(AvatarId, NewRecord);

	return true;
}

bool UAvatarSubsystem::RemoveAvatar(const FPrimaryAssetId& AvatarId)
{
	TMap<FPrimaryAssetId, FAvatarRecord>* Records = GetMutableAvatarRecords();
	if (!EntityPrimaryAsset::IsValid(AvatarId) || !Records)
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarId is invalid or Avatar records not found"));
		return false;
	}

	if (!Records->Contains(AvatarId))
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar not found"));
		return false;
	}

	Records->Remove(AvatarId);

	return true;
}

bool UAvatarSubsystem::UpdateAvatar(const FPrimaryAssetId& AvatarId, const FAvatarRecord& AvatarRecord)
{
	FAvatarRecord* Record = GetMutableAvatarRecord(AvatarId);
	if (!Record)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar not found"));
		return false;
	}

	Record->Health = AvatarRecord.Health;
	Record->Enhance = AvatarRecord.Enhance;
	Record->Sanitize();

	OnAvatarUpdated.Broadcast(AvatarId);

	return true;
}

bool UAvatarSubsystem::UpdateAvatar(const FPrimaryAssetId& AvatarId, TFunctionRef<bool(FAvatarRecord*)> Callback)
{
	FAvatarRecord* Record = GetMutableAvatarRecord(AvatarId);
	if (!Callback(Record))
	{
		LOG_ERROR(LogAvatar, TEXT("Record update callback returned false"));
		return false;
	}

	OnAvatarUpdated.Broadcast(AvatarId);

	return true;
}







void UAvatarSubsystem::LoadAvatarStorage()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<IStorageProviderInterface>(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogAvatar, TEXT("Storage provider not found"));
		return;
	}

	UAvatarStorage* Storage = StorageProvider->GetStorage<UAvatarStorage>(TEXT("/get/avatars"));
	if (!Storage)
	{
		LOG_ERROR(LogAvatar, TEXT("Avatar storage not found"));
		return;
	}

	StorageProviderInterface = TWeakInterfacePtr<IStorageProviderInterface>(StorageProvider);
	AvatarStorage = Storage;

	LOG_INFO(LogAvatar, TEXT("Storage loaded"));
}

void UAvatarSubsystem::SaveAvatarStorage()
{
	UAvatarStorage* Storage = AvatarStorage.Get();
	IStorageProviderInterface* StorageProvider = StorageProviderInterface.Get();
	if (StorageProvider || IsValid(Storage))
	{
		StorageProvider->SaveStorage(Storage, TEXT("/get/avatars"));
	}

	AvatarStorage = nullptr;
	StorageProviderInterface.Reset();
}





bool UAvatarSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAvatarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FLatentDelegates::OnStorageLoaded.AddUObject(this, &UAvatarSubsystem::LoadAvatarStorage);

	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem initialized"));
}

void UAvatarSubsystem::Deinitialize()
{
	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem deinitialized"));

	SaveAvatarStorage();

	Super::Deinitialize();
}










ULatentTask* UAvatarEnhanceSubsystem::AddExperiencePoints(const FGuid& LatentId, const FPrimaryAssetId& AvatarId, const FPrimaryAssetId& ItemId)
{
	if (!IsValid(TaskSubsystem)) return nullptr;

	UAddExperienceTask* Task = TaskSubsystem->CreateTask<UAddExperienceTask>(LatentId);
	if (!IsValid(Task)) return nullptr;

	Task->AvatarId = AvatarId;
	Task->ItemId = ItemId;

	return Task;
}

ULatentTask* UAvatarEnhanceSubsystem::AddRankPoints(const FGuid& LatentId, const FPrimaryAssetId& AvatarId)
{
	return nullptr;
}

bool UAvatarEnhanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAvatarEnhanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		TaskSubsystem = GameInstance->GetSubsystem<UTaskSubsystem>();
	}

	LOG_WARNING(LogAvatar, TEXT("AvatarEnhanceSubsystem initialized"));
}

void UAvatarEnhanceSubsystem::Deinitialize()
{
	LOG_WARNING(LogAvatar, TEXT("AvatarEnhanceSubsystem deinitialized"));
	Super::Deinitialize();
}






void UAddExperienceTask::StartTask()
{
	Super::StartTask();

	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (IsValid(GameInstance))
	{
		AvatarSubsystem = Cast<UAvatarSubsystem>(GameInstance->GetSubsystem<UAvatarSubsystem>());
	}

	Step_ValidatePlayer();
}

void UAddExperienceTask::StopTask()
{
	AssetManager->CancelFetch(LatentId);

	Super::StopTask();
}


void UAddExperienceTask::Step_ValidatePlayer()
{
	const FAvatarRecord* Record = AvatarSubsystem->GetAvatarRecord(AvatarId);
	if (!Record)
	{
		Fail(TEXT("Avatar not found"));
		return;
	}

	Step_LoadCharacter();
}

void UAddExperienceTask::Step_LoadCharacter()
{
	TArray<FPrimaryAssetId> Assets;
	Assets.Add(AvatarId);
	Assets.Add(ItemId);

	TFuture<FLatentResultAssets<UPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<UPrimaryDataAsset>(LatentId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UAddExperienceTask> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAssets<UPrimaryDataAsset>& Result)
		{
			UAddExperienceTask* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<UPrimaryDataAsset*>& Assets = Result.Get();
			This->AvatarAsset = Cast<UAvatarAsset>(Assets[0]);
			This->ItemAsset = Assets[1];

			This->Step_CheckRules();
		}
	);
}

void UAddExperienceTask::Step_CheckRules()
{
	const FAvatarRecord* Record = AvatarSubsystem->GetAvatarRecord(AvatarId);
	if (!Record || !AvatarAsset || !ItemAsset)
	{
		Fail(TEXT("Avatar not found, AvatarAsset or ItemAsset is invalid"));
		return;
	}

	const FEnhanceRecord& Enhance = Record->Enhance;
	const UAssetCollectionRule_Dictionary* AssetCollection = AvatarAsset->GetExperienceItems(Enhance.Experience, Enhance.Level, Enhance.Rank);
	if (!IsValid(AssetCollection) || !AssetCollection->AssetIds.Contains(ItemId))
	{
		Fail(TEXT("Item not found"));
		return;
	}

	const IAssetStructureInterface* AssetStructure = Cast<IAssetStructureInterface>(ItemAsset);
	if (!AssetStructure)
	{
		Fail(TEXT("AssetStructure is invalid"));
		return;
	}

	Step_RemoveItem();
}

void UAddExperienceTask::Step_RemoveItem()
{
	Step_ApplyLevelUp();
}

void UAddExperienceTask::Step_ApplyLevelUp()
{
	int Experience = 100;

	bool bResult = AvatarSubsystem->UpdateAvatar(AvatarId, [Experience](FAvatarRecord* Record)
		{
			if (!Record)
			{
				return false;
			}
			Record->Enhance.Experience += Experience;
			return true;
		}
	);

	if (!bResult)
	{
		Fail(TEXT("Failed to update avatar"));
		return;
	}

	Succeed();
}





void ULatentTask::StartTask()
{
	LatentDelegate.OnStarted.Broadcast(LatentId);
}

void ULatentTask::StopTask()
{
	Fail(TEXT("Task stopped"));
}

void ULatentTask::Succeed()
{
	LatentDelegate.OnFinished.Broadcast(LatentId, true, TEXT(""));
	Cleanup();
}

void ULatentTask::Fail(const FString& Reason)
{
	LatentDelegate.OnFinished.Broadcast(LatentId, false, Reason);
	Cleanup();
}

void ULatentTask::Cleanup()
{
	LatentDelegate.ClearAll();
}



ULatentTask* UTaskSubsystem::CreateTask(TSubclassOf<ULatentTask> TaskClass, const FGuid& LatentId)
{
	if (LatentTasks.Contains(LatentId))
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task already exists"));
		return nullptr;
	}

	ULatentTask* Task = NewObject<ULatentTask>(this, TaskClass);
	if (Task)
	{
		LatentTasks.Add(LatentId, Task);

		Task->LatentId = LatentId;

		FLatentTaskDelegate& LatentDelegate = Task->LatentDelegate;
		LatentDelegate.OnStarted.AddUObject(this, &UTaskSubsystem::HandleOnTaskStarted);
		LatentDelegate.OnFinished.AddUObject(this, &UTaskSubsystem::HandleOnTaskFinished);
	}

	return Task;
}

void UTaskSubsystem::StartTask(const FGuid& LatentId)
{
	TObjectPtr<ULatentTask>* TaskItem = LatentTasks.Find(LatentId);
	if (!TaskItem)
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task not found"));
		return;
	}

	ULatentTask* Task = TaskItem->Get();
	if (!IsValid(Task))
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task is invalid"));
		return;
	}

	Task->StartTask();
}

void UTaskSubsystem::StopTask(const FGuid& LatentId)
{
	TObjectPtr<ULatentTask>* TaskItem = LatentTasks.Find(LatentId);
	if (!TaskItem)
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task not found"));
		return;
	}

	ULatentTask* Task = TaskItem->Get();
	if (!IsValid(Task))
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task is invalid"));
		return;
	}

	Task->StopTask();
}

void UTaskSubsystem::HandleOnTaskStarted(FGuid LatentId)
{

}

void UTaskSubsystem::HandleOnTaskFinished(FGuid LatentId, bool bSuccess, const FString& Reason)
{
	TObjectPtr<ULatentTask>* TaskItem = LatentTasks.Find(LatentId);
	if (!TaskItem)
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task not found"));
		return;
	}

	ULatentTask* Task = TaskItem->Get();
	if (!IsValid(Task))
	{
		LOG_ERROR(LogAvatar, TEXT("Latent task is invalid"));
		return;
	}

	Task->MarkAsGarbage();

	LatentTasks.Remove(LatentId);
}


bool UTaskSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UTaskSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LOG_WARNING(LogAvatar, TEXT("TaskSubsystem initialized"));
}

void UTaskSubsystem::Deinitialize()
{
	LOG_WARNING(LogAvatar, TEXT("TaskSubsystem deinitialized"));

	Super::Deinitialize();
}
