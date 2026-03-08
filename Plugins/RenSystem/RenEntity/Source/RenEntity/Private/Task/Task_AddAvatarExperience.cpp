// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_AddAvatarExperience.h"

// Engine Headers

// Project Headers
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Asset/AvatarAsset.h"
#include "Manager/RAssetManager.inl"
#include "Interface/AssetStructureInterface.h"
#include "Subsystem/AvatarSubsystem.h"



void UTask_AddAvatarExperience::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (IsValid(GameInstance))
	{
		AvatarSubsystem = Cast<UAvatarSubsystem>(GameInstance->GetSubsystem<UAvatarSubsystem>());
	}

	Step_LoadAssets();
}

void UTask_AddAvatarExperience::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UTask_AddAvatarExperience::OnCleanup()
{
	AvatarAsset = nullptr;
	ItemAsset = nullptr;
	AvatarSubsystem = nullptr;
	AssetManager = nullptr;

	AvatarId = FPrimaryAssetId();
	ItemId = FPrimaryAssetId();
}

void UTask_AddAvatarExperience::Step_LoadAssets()
{
	if (!IsValid(AssetManager) || !IsValid(AvatarSubsystem))
	{
		Fail(TEXT("AssetManager or AvatarSubsystem is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(AvatarId);
	Assets.Add(ItemId);

	TFuture<FLatentLoadedAssets<URPrimaryDataAsset>> Future = AssetManager->FetchPrimaryAssets<URPrimaryDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_AddAvatarExperience> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<URPrimaryDataAsset>& Result)
		{
			UTask_AddAvatarExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<URPrimaryDataAsset*>& Assets = Result.Get();
			This->AvatarAsset = Cast<UAvatarAsset>(Assets[0]);
			This->ItemAsset = Assets[1];

			This->Step_CheckAssets();
		}
	);
}

void UTask_AddAvatarExperience::Step_CheckAssets()
{/*
	const FAvatarData* Record = AvatarSubsystem->GetAvatarData(AvatarId);
	if (!Record || !IsValid(AvatarAsset) || !IsValid(ItemAsset))
	{
		Fail(TEXT("Avatar not found, AvatarAsset or ItemAsset is invalid"));
		return;
	}

	const FAscensionData& Ascension = Record->Ascension;
	const UAssetCollection* ExperienceItems = AvatarAsset->GetExperienceItems(Ascension.Experience, Ascension.Level, Ascension.Rank);
	if (!IsValid(ExperienceItems) || !ExperienceItems->AssetIds.Contains(ItemId))
	{
		Fail(TEXT("Item not found"));
		return;
	}

	const IAssetStructureInterface* AssetStructure = Cast<IAssetStructureInterface>(ItemAsset);
	if (!AssetStructure)
	{
		Fail(TEXT("AssetStructure is invalid"));
		return;
	}*/

	//const UAssetCollection* BreakdownAssets = AssetStructure->GetBreakdownAssets(ExperienceItems->Tags);
	// TODO:
	// cast BreakdownAsset[0] to ExperiencePoint
	// get exp point from ExperiencePoint

	Step_RemoveItem();
}

void UTask_AddAvatarExperience::Step_RemoveItem()
{
	Step_AddExperience();
}

void UTask_AddAvatarExperience::Step_AddExperience()
{
	const FAvatarData* Record = AvatarSubsystem->GetAvatarData(AvatarId);
	if (!Record)
	{
		Fail(TEXT("Avatar not found"));
		return;
	}

	int Experience = 100;

	bool bSuccess = AvatarSubsystem->UpdateAvatar(AvatarId, [Experience](FAvatarData* Record)
		{
			if (!Record) return false;
			Record->Ascension.Experience += Experience;
			Record->Sanitize();
			return true;
		}
	);

	if (!bSuccess)
	{
		Fail(TEXT("Failed to update avatar"));
		return;
	}
	Succeed();
}

