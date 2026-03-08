// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEnhanceUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Asset/AvatarAsset.h"
#include "Definition/TaskType.h"
#include "Filter/FilterLeafCriterion.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Subsystem/AvatarAscensionSubsystem.h"
#include "Widget/AssetEntry.h"
#include "Widget/AvatarCollectionUI.h"
#include "Widget/AvatarDetailUI.h"



void UAvatarEnhanceUI::AddExperiencePoints(const UAssetEntry* Entry)
{
	if (!IsValid(AscensionSubsystem) || !IsValid(Entry))
	{
		LOG_ERROR(LogAvatar, TEXT("AscensionSubsystem, Entry is invalid"));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	AscensionSubsystem->AddExperiencePoints(TaskId, GetActiveAssetId(), Entry->AssetId,
		FTaskCallback::CreateWeakLambda(this,
			[](const FTaskResult& Result)
			{
				if (Result.State == ETaskState::Pending)
				{
					UE_LOG(LogAvatar, Log, TEXT("Task Started"));
				}
				else
				{
					UE_LOG(LogAvatar, Log, TEXT("Task Finished"));
				}
			}
		)
	);
}

void UAvatarEnhanceUI::AddRankPoints()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogAvatar, TEXT("AscensionSubsystem is invalid"));
		return;
	}
}

void UAvatarEnhanceUI::HandleItemSelected(const UAssetEntry* Entry)
{
	AddExperiencePoints(Entry);
}

void UAvatarEnhanceUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{/*
	if (IsValid(AvatarDetail))
	{
		AvatarDetail->InitializeDetail(Entry, Asset);
	}

	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (IsValid(ItemCollection) && IsValid(AvatarAsset))
	{
		UFilterAssetCriterion* AssetId = ItemCollection->GetCriterionByName<UFilterAssetCriterion>(TEXT("AssetId"));
		if (IsValid(AssetId))
		{
			AssetId->Included.Empty();

			UAssetGroup* CollectionGroup = AvatarAsset->ExperienceItems;
			if (IsValid(CollectionGroup))
			{
				const UAssetCollection_Simple* CollectionRule = CollectionGroup->GetCollectionRule<UAssetCollection_Simple>();
				if (IsValid(CollectionRule))
				{
					TArray<FPrimaryAssetId> AssetIds;
					CollectionRule->GetAssetList().GetKeys(AssetIds);

					AssetId->Included.Append(AssetIds);
				}
			}
		}
		ItemCollection->DisplayEntries();
	}*/
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
		AscensionSubsystem = GameInstance->GetSubsystem<UAvatarAscensionSubsystem>();
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

	AscensionSubsystem = nullptr;

	Super::NativeDestruct();
}

