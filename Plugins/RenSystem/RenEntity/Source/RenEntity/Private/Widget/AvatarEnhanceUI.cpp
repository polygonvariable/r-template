// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEnhanceUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "AvatarAsset.h"
#include "FilterLeafCriterion.h"
#include "LogCategory.h"
#include "LogMacro.h"
#include "Widget/CatalogEntry.h"

#include "EntityPrimaryAsset.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Widget/AvatarCollectionUI.h"
#include "Widget/AvatarDetailUI.h"



void UAvatarEnhanceUI::AddExperiencePoints(const UCatalogEntry* Entry)
{
	if (!IsValid(EnhanceSubsystem) || !IsValid(Entry))
	{
		LOG_ERROR(LogAvatar, TEXT("EnhanceSubsystem or Entry is invalid"));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	ULatentTask* Task = EnhanceSubsystem->AddExperiencePoints(TaskId, ActiveAssetId, Entry->AssetId);
	if (IsValid(Task))
	{
		FLatentTaskDelegate& TaskDelegate = Task->LatentDelegate;
		TaskDelegate.OnStarted.AddWeakLambda(this, [](FGuid) { UE_LOG(LogAvatar, Log, TEXT("Enhance Task Started")); });
		TaskDelegate.OnFinished.AddWeakLambda(this, [](FGuid, bool, const FString&) { UE_LOG(LogAvatar, Log, TEXT("Enhance Task Finished")); });

		Task->StartTask();
	}
}

void UAvatarEnhanceUI::AddRankPoints()
{
	if (!IsValid(EnhanceSubsystem))
	{
		LOG_ERROR(LogAvatar, TEXT("EnhanceSubsystem is invalid"));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	ULatentTask* Task = EnhanceSubsystem->AddRankPoints(TaskId, ActiveAssetId);
	if (IsValid(Task))
	{
		FLatentTaskDelegate& TaskDelegate = Task->LatentDelegate;
		TaskDelegate.OnStarted.AddWeakLambda(this, [](FGuid) { UE_LOG(LogAvatar, Log, TEXT("Enhance Task Started")); });
		TaskDelegate.OnFinished.AddWeakLambda(this, [](FGuid, bool, const FString&) { UE_LOG(LogAvatar, Log, TEXT("Enhance Task Finished")); });

		Task->StartTask();
	}
}

void UAvatarEnhanceUI::HandleItemSelected(const UCatalogEntry* Entry)
{
	AddExperiencePoints(Entry);
}

void UAvatarEnhanceUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	if (IsValid(AvatarDetail))
	{
		AvatarDetail->InitializeDetails(Entry, Asset);
	}

	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (IsValid(ItemCollection) && IsValid(AvatarAsset))
	{
		UFilterAssetCriterion* AssetId = ItemCollection->GetCriterionByName<UFilterAssetCriterion>(TEXT("AssetId"));
		if (IsValid(AssetId))
		{
			AssetId->Included.Empty();

			UAssetCollectionGroup* CollectionGroup = AvatarAsset->ExperienceItems;
			if (IsValid(CollectionGroup))
			{
				const UAssetCollectionRule_Dictionary* CollectionRule = CollectionGroup->GetCollectionRule<UAssetCollectionRule_Dictionary>();
				if (IsValid(CollectionRule))
				{
					TArray<FPrimaryAssetId> AssetIds;
					CollectionRule->AssetIds.GetKeys(AssetIds);

					AssetId->Included.Append(AssetIds);
				}
			}
		}
		ItemCollection->DisplayEntries();
	}
}

void UAvatarEnhanceUI::NativeConstruct()
{
	if (IsValid(ItemCollection))
	{
		ItemCollection->OnEntrySelected.BindUObject(this, &UAvatarEnhanceUI::HandleItemSelected);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.AddDynamic(this, &UAvatarEnhanceUI::CloseWidget);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		EnhanceSubsystem = GameInstance->GetSubsystem<UAvatarEnhanceSubsystem>();
	}

	Super::NativeConstruct();
}

void UAvatarEnhanceUI::NativeDestruct()
{
	if (IsValid(ItemCollection))
	{
		ItemCollection->OnEntrySelected.Unbind();
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveAll(this);
	}

	EnhanceSubsystem = nullptr;

	Super::NativeDestruct();
}

