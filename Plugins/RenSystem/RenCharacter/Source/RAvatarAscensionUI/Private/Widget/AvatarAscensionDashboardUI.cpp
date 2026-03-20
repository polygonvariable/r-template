// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "InstancedStruct.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/Runtime/AvatarInstance.h"
#include "Filter/FilterLeafCriterion.h"
#include "Interface/IAscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarAscensionSubsystem.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/AvatarEntry.h"



void UAvatarAscensionDashboardUI::InitializeDetail()
{
	AvatarDetail->AssetSourceId = AssetSourceId;
	AvatarDetail->InitializeDetail();

	LevelItemCollection->InitializeCollection();
	RankItemCollection->InitializeCollection();

	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetGameInstance());
	if (!IsValid(AvatarSubsystem))
	{
		return;
	}

	UAvatarStorage* AvatarCollection = AvatarSubsystem->GetAvatarCollection(AssetSourceId);
	if (IsValid(AvatarCollection) && !bAutoRefresh)
	{
		AvatarCollection->OnRefreshed.AddUObject(this, &UAvatarAscensionDashboardUI::RefreshDetail);
	}

	AvatarStorage = TWeakObjectPtr<UAvatarStorage>(AvatarCollection);
}

void UAvatarAscensionDashboardUI::RefreshDetail()
{
	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (!IsValid(AvatarCollection))
	{
		return;
	}

	const FAvatarInstance* Instance = AvatarCollection->GetInstance(GetActiveAssetId());
	ToggleAscension(Instance);
}



void UAvatarAscensionDashboardUI::EnableControls()
{
	LevelUpButton->SetIsEnabled(true);
	RankUpButton->SetIsEnabled(true);
}

void UAvatarAscensionDashboardUI::DisableControls()
{
	LevelUpButton->SetIsEnabled(false);
	RankUpButton->SetIsEnabled(false);
}



void UAvatarAscensionDashboardUI::ToggleAscension(const FAvatarInstance* Instance)
{
	if (!ActiveInstanceAscension || !Instance)
	{
		return;
	}

	int LevelPerRank = ActiveInstanceAscension->GetLevelInterval(Instance->Ascension.Rank);
	int MaxLevel = ActiveInstanceAscension->GetMaxLevel();
	int MaxRank = ActiveInstanceAscension->GetMaxRank();

	if (UAscensionLibrary::IsRankUpRequired(Instance->Ascension, LevelPerRank, MaxLevel, MaxRank))
	{
		ToggleRankUp(Instance);
	}
	else
	{
		ToggleLevelUp(Instance);
	}
}

void UAvatarAscensionDashboardUI::ToggleLevelUp(const FAvatarInstance* Instance)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Visible);
	RankUpButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UAvatarAscensionDashboardUI::ToggleRankUp(const FAvatarInstance* Instance)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Collapsed);
	RankUpButton->SetVisibility(ESlateVisibility::Visible);

	if (!ActiveInstanceAscension || !Instance)
	{
		return;
	}

	RankItemCollection->ClearSubDetails();

	UFilterAssetCriterion* AssetFilter = RankItemCollection->GetCriterionByName<UFilterAssetCriterion>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const FAscensionData& AscensionData = Instance->Ascension;
		const UAssetCollection* ItemCollection = ActiveInstanceAscension->GetRankAssets(AscensionData);
		if (IsValid(ItemCollection))
		{
			TMap<FPrimaryAssetId, FAssetDetail> AssetList;
			ItemCollection->GetAssetList(AssetList);

			for (const TPair<FPrimaryAssetId, FAssetDetail>& AssetKv : AssetList)
			{
				const FPrimaryAssetId& AssetId = AssetKv.Key;

				AssetFilter->Included.Add(AssetId);
				RankItemCollection->AddSubDetails(AssetId, FInstancedStruct::Make(AssetKv.Value));
			}
		}
	}

	RankItemCollection->RefreshEntries();
}



void UAvatarAscensionDashboardUI::HandleTaskCallback(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Pending)
	{
		DisableControls();
		LOG_WARNING(LogAvatarAscension, TEXT("Task Started"));
	}
	else
	{
		EnableControls();
		LOG_WARNING(LogAvatarAscension, TEXT("Task Finished, Message: %s"), *Result.Message);
	}
}



void UAvatarAscensionDashboardUI::HandleLevelUp()
{
	const UAssetEntry* Entry = LevelItemCollection->GetSelectedEntry();
	if (!IsValid(AscensionSubsystem) || !IsValid(Entry))
	{
		LOG_ERROR(LogAvatarAscension, TEXT("AscensionSubsystem, Entry is not valid"));
		return;
	}

	FGuid MaterialId = Entry->GetAssetInstanceId();
	FPrimaryAssetId MaterialAssetId = Entry->AssetId;
	
	AscensionSubsystem->AddExperiencePoints(AssetSourceId, GetActiveAssetId(), MaterialAssetId, MaterialId, FTaskCallback::CreateUObject(this, &UAvatarAscensionDashboardUI::HandleTaskCallback));
}

void UAvatarAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogAvatarAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	AscensionSubsystem->AddRankPoints(AssetSourceId, GetActiveAssetId(), FTaskCallback::CreateUObject(this, &UAvatarAscensionDashboardUI::HandleTaskCallback));
}



void UAvatarAscensionDashboardUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	AvatarDetail->InitializeDetail(Entry, Asset);
}

void UAvatarAscensionDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(Asset);
	if (!IsValid(AvatarEntry) || !AscensionProvider)
	{
		return;
	}
	 
	const FAvatarInstance* Instance = AvatarEntry->AvatarInstance;
	if (!Instance)
	{
		return;
	}

	ActiveInstanceAscension = AscensionProvider;

	UFilterAssetCriterion* AssetIdFilter = LevelItemCollection->GetCriterionByName<UFilterAssetCriterion>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetIdFilter))
	{
		AssetIdFilter->Included.Empty();

		const FAscensionData& AscensionData = Instance->Ascension;
		const UAssetCollection* ExperienceCollection = AscensionProvider->GetExperienceAssets(AscensionData);
		if (IsValid(ExperienceCollection))
		{
			TArray<FPrimaryAssetId> AssetList;
			ExperienceCollection->GetAssetIds(AssetList);

			AssetIdFilter->Included.Append(AssetList);
		}
	}

	LevelItemCollection->DisplayEntries();

	ToggleAscension(Instance);
}



void UAvatarAscensionDashboardUI::NativeConstruct()
{
	RankUpButton->OnClicked.AddDynamic(this, &UAvatarAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UAvatarAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UAvatarAscensionSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UAvatarAscensionDashboardUI::NativeDestruct()
{
	RankUpButton->OnClicked.RemoveAll(this);
	LevelUpButton->OnClicked.RemoveAll(this);

	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnRefreshed.RemoveAll(this);
	}
	AvatarStorage.Reset();

	AscensionSubsystem = nullptr;
	ActiveInstanceAscension = nullptr;

	Super::NativeDestruct();
}

