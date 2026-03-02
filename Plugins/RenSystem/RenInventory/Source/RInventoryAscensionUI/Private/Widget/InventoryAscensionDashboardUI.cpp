// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "InstancedStruct.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/Runtime/InventoryItem.h"
#include "Filter/FilterLeafCriterion.h"
#include "Interface/AscensionProviderInterface.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/Collection/AssetCollectionSimple.h"
#include "Subsystem/InventoryAscensionSubsystem.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/InventoryEntry.h"



void UInventoryAscensionDashboardUI::SetContainerId(const FGuid& Id)
{
	Super::SetContainerId(Id);

	PrimaryCollection->SetContainerId(Id);
	PrimaryDetail->SetContainerId(Id);
	SecondaryCollection->SetContainerId(Id);
}

void UInventoryAscensionDashboardUI::RefreshDetails()
{
	if (!IsValid(InventorySubsystem))
	{
		return;
	}
	const FInventoryItem* Item = InventorySubsystem->GetItemById(ContainerId, ActiveAssetId, ActiveItemId);
	ToggleAscension(Item);
}



void UInventoryAscensionDashboardUI::EnableControls()
{
	LevelUpButton->SetIsEnabled(true);
	RankUpButton->SetIsEnabled(true);
}

void UInventoryAscensionDashboardUI::DisableControls()
{
	LevelUpButton->SetIsEnabled(false);
	RankUpButton->SetIsEnabled(false);
}

void UInventoryAscensionDashboardUI::ToggleAscension(const FInventoryItem* Item)
{
	if (!ActiveItemAscension || !Item)
	{
		return;
	}

	int LevelPerRank = ActiveItemAscension->GetLevelInterval(Item->Ascension.Rank);
	int MaxLevel = ActiveItemAscension->GetMaxLevel();
	int MaxRank = ActiveItemAscension->GetMaxRank();

	if (UAscensionLibrary::IsRankUpRequired(Item->Ascension, LevelPerRank, MaxLevel, MaxRank))
	{
		ToggleRankUp(Item);
	}
	else
	{
		ToggleLevelUp(Item);
	}
}

void UInventoryAscensionDashboardUI::ToggleLevelUp(const FInventoryItem* Item)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Visible);
	RankUpButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryAscensionDashboardUI::ToggleRankUp(const FInventoryItem* Item)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Collapsed);
	RankUpButton->SetVisibility(ESlateVisibility::Visible);

	if (!ActiveItemAscension || !Item)
	{
		return;
	}

	SecondaryCollection->ClearSubDetails();

	UFilterAssetCriterion* AssetFilter = SecondaryCollection->GetCriterionByName<UFilterAssetCriterion>(AssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const FAscensionData& AscensionData = Item->Ascension;
		const UAssetCollection* ItemCollection = ActiveItemAscension->GetRankItems(AscensionData);
		if (IsValid(ItemCollection))
		{
			TMap<FPrimaryAssetId, int> AssetList;
			ItemCollection->GetAssetList(AssetList);

			for (const TPair<FPrimaryAssetId, FAssetDetail>& AssetKv : AssetList)
			{
				const FPrimaryAssetId& AssetId = AssetKv.Key;

				AssetFilter->Included.Add(AssetId);
				SecondaryCollection->AddSubDetails(AssetId, FInstancedStruct::Make(AssetKv.Value));
			}
		}
	}

	SecondaryCollection->RefreshEntries();
}


void UInventoryAscensionDashboardUI::HandleTaskCallback(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Pending)
	{
		DisableControls();
		UE_LOG(LogInventoryInstance, Log, TEXT("Task Started"));
	}
	else
	{
		EnableControls();
		UE_LOG(LogInventoryInstance, Log, TEXT("Task Finished, Message: %s"), *Result.Message);
	}
}



void UInventoryAscensionDashboardUI::HandleLevelUp()
{
	const UInventoryEntry* InventoryEntry = PrimaryCollection->GetSelectedEntry<UInventoryEntry>();
	if (!IsValid(AscensionSubsystem) || !IsValid(InventoryEntry))
	{
		LOG_ERROR(LogInventoryInstance, TEXT("AscensionSubsystem, InventoryEntry is not valid"));
		return;
	}

	const FInventoryItem* Item = InventoryEntry->Item;
	if (!Item)
	{
		LOG_ERROR(LogInventoryInstance, TEXT("Item is not valid"));
		return;
	}

	FGuid MaterialId = Item->ItemId;
	FPrimaryAssetId MaterialAssetId = InventoryEntry->AssetId;
	
	FGuid TaskId = FGuid::NewGuid();
	AscensionSubsystem->AddExperiencePoints(TaskId, ContainerId, ActiveAssetId, ActiveItemId, MaterialAssetId, MaterialId, FTaskCallback::CreateUObject(this, &UInventoryAscensionDashboardUI::HandleTaskCallback));
}

void UInventoryAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogInventoryInstance, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	AscensionSubsystem->AddRankPoints(TaskId, ContainerId, ActiveAssetId, ActiveItemId, FTaskCallback::CreateUObject(this, &UInventoryAscensionDashboardUI::HandleTaskCallback));
}

void UInventoryAscensionDashboardUI::HandleOnItemUpdated(const FGuid& InventoryId)
{
	if (ContainerId == InventoryId)
	{
		RefreshDetails();
	}
}



void UInventoryAscensionDashboardUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	const IAscensionProviderInterface* AscensionProvider = Cast<IAscensionProviderInterface>(Asset);
	if (!IsValid(InventoryEntry) || !AscensionProvider)
	{
		return;
	}
	 
	const FInventoryItem* Item = InventoryEntry->Item;
	if (!Item)
	{
		return;
	}

	ActiveItemId = Item->ItemId;
	ActiveItemAscension = AscensionProvider;

	UFilterAssetCriterion* AssetIdFilter = PrimaryCollection->GetCriterionByName<UFilterAssetCriterion>(AssetFilterProperty::AssetId);
	if (IsValid(AssetIdFilter))
	{
		AssetIdFilter->Included.Empty();

		const FAscensionData& AscensionData = Item->Ascension;
		const UAssetCollection* ItemCollection = AscensionProvider->GetExperienceItems(AscensionData);
		if (IsValid(ItemCollection))
		{
			TArray<FPrimaryAssetId> AssetList;
			ItemCollection->GetAssetIds(AssetList);

			AssetIdFilter->Included.Append(AssetList);
		}
	}

	PrimaryCollection->DisplayEntries();

	ToggleAscension(Item);
}

void UInventoryAscensionDashboardUI::GetAllAssetUI_Implementation(TArray<UAssetUI*>& OutAssetUI) const
{
	OutAssetUI.Add(PrimaryDetail);
}



void UInventoryAscensionDashboardUI::NativeConstruct()
{
	RankUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UInventoryAscensionSubsystem::Get(GetGameInstance());
	InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (IsValid(InventorySubsystem))
	{
		if (bAutoRefresh)
		{
			InventorySubsystem->OnInventoryRefreshed.AddUObject(this, &UInventoryAscensionDashboardUI::HandleOnItemUpdated);
		}
	}

	Super::NativeConstruct();
}

void UInventoryAscensionDashboardUI::NativeDestruct()
{
	RankUpButton->OnClicked.RemoveAll(this);
	LevelUpButton->OnClicked.RemoveAll(this);

	if (IsValid(InventorySubsystem))
	{
		InventorySubsystem->OnInventoryRefreshed.RemoveAll(this);
	}
	AscensionSubsystem = nullptr;
	InventorySubsystem = nullptr;
	ActiveItemAscension = nullptr;

	Super::NativeDestruct();
}

