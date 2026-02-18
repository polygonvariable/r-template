// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ItemAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "AssetCollection.h"
#include "Definition/InventoryFilterProperty.h"
#include "Definition/InventoryItem.h"
#include "FilterLeafCriterion.h"
#include "Interface/AscensionProviderInterface.h"
#include "InventoryEntry.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/InventorySubsystem.h"
#include "Subsystem/ItemAscensionSubsystem.h"
#include "Widget/CatalogEntry.h"
#include "Widget/InventoryCollectionUI.h"
#include "Widget/InventoryDetailUI.h"



void UItemAscensionDashboardUI::SetCatalogId(const FGuid& Id)
{
	Super::SetCatalogId(Id);

	if (IsValid(PrimaryCollection)) PrimaryCollection->SetCatalogId(Id);
	if (IsValid(PrimaryDetail)) PrimaryDetail->SetCatalogId(Id);
}

void UItemAscensionDashboardUI::RefreshDetails()
{
	if (!IsValid(InventorySubsystem))
	{
		return;
	}
	const FInventoryItem* Item = InventorySubsystem->GetItemById(CatalogId, ActiveAssetId, ActiveItemId);
	ToggleAscension(Item);
}



void UItemAscensionDashboardUI::ToggleAscension(const FInventoryItem* Item)
{
	if (!ActiveItemAscension || !Item)
	{
		return;
	}

	int LevelPerRank = ActiveItemAscension->GetLevelInterval(Item->Ascension.Rank);
	int MaxLevel = ActiveItemAscension->GetMaxLevel();
	int MaxRank = ActiveItemAscension->GetMaxRank();

	bool bIsRankUpRequired = UAscensionLibrary::IsRankUpRequired(Item->Ascension, LevelPerRank, MaxLevel, MaxRank);
	if (bIsRankUpRequired)
	{
		ToggleRankUp();
	}
	else
	{
		ToggleLevelUp();
	}
}

void UItemAscensionDashboardUI::ToggleLevelUp()
{
	LevelUpButton->SetVisibility(ESlateVisibility::Visible);
	RankUpButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemAscensionDashboardUI::ToggleRankUp()
{
	LevelUpButton->SetVisibility(ESlateVisibility::Collapsed);
	RankUpButton->SetVisibility(ESlateVisibility::Visible);
}



void UItemAscensionDashboardUI::HandleLevelUp()
{
	const UCatalogEntry* Entry = PrimaryCollection->GetSelectedEntry();
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!IsValid(AscensionSubsystem) || !IsValid(InventoryEntry))
	{
		LOG_ERROR(LogItemAscension, TEXT("AscensionSubsystem, InventoryEntry is not valid"));
		return;
	}

	const FInventoryItem* Item = InventoryEntry->Item;
	if (!Item)
	{
		LOG_ERROR(LogItemAscension, TEXT("Item is not valid"));
		return;
	}

	FGuid MaterialId = Item->ItemId;
	FPrimaryAssetId MaterialAssetId = InventoryEntry->AssetId;

	FGuid TaskId = FGuid::NewGuid();
	AscensionSubsystem->AddExperiencePoints(TaskId, CatalogId, ActiveAssetId, ActiveItemId, MaterialAssetId, MaterialId,
		FTaskCallback::CreateWeakLambda(this,
			[](const FTaskResult& Result)
			{
				if (Result.State == ETaskState::Pending)
				{
					UE_LOG(LogAvatar, Log, TEXT("Task Started"));
				}
				else
				{
					UE_LOG(LogAvatar, Log, TEXT("Task Finished, Message: %s"), *Result.Message);
				}
			}
		)
	);
}

void UItemAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogItemAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	FGuid TaskId = FGuid::NewGuid();
	AscensionSubsystem->AddRankPoints(TaskId, CatalogId, ActiveAssetId, ActiveItemId,
		FTaskCallback::CreateWeakLambda(this,
			[](const FTaskResult& Result)
			{
				if (Result.State == ETaskState::Pending)
				{
					UE_LOG(LogAvatar, Log, TEXT("Task Started"));
				}
				else
				{
					UE_LOG(LogAvatar, Log, TEXT("Task Finished, Message: %s"), *Result.Message);
				}
			}
		)
	);
}

void UItemAscensionDashboardUI::HandleOnItemUpdated(const FGuid& InventoryId)
{
	if (CatalogId == InventoryId)
	{
		RefreshDetails();
	}
}



void UItemAscensionDashboardUI::SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
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

	UFilterAssetCriterion* AssetCriterion = PrimaryCollection->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();

		const FAscensionData& Ascension = Item->Ascension;
		const UAssetCollection* CollectionRule = AscensionProvider->GetExperienceItems(Ascension);
		if (IsValid(CollectionRule))
		{
			TArray<FPrimaryAssetId> AssetIds;
			CollectionRule->AssetIds.GetKeys(AssetIds);

			AssetCriterion->Included.Append(AssetIds);
		}
	}

	PrimaryCollection->DisplayEntries();

	ToggleAscension(Item);
}

void UItemAscensionDashboardUI::GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutCatalogUI) const
{
	OutCatalogUI.Add(PrimaryDetail);
}



void UItemAscensionDashboardUI::NativeConstruct()
{
	if (IsValid(RankUpButton)) RankUpButton->OnClicked.AddDynamic(this, &UItemAscensionDashboardUI::HandleRankUp);
	if (IsValid(LevelUpButton)) LevelUpButton->OnClicked.AddDynamic(this, &UItemAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UItemAscensionSubsystem::Get(GetGameInstance());
	InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (IsValid(InventorySubsystem))
	{
		if (bAutoRefresh)
		{
			InventorySubsystem->OnInventoryRefreshed.AddUObject(this, &UItemAscensionDashboardUI::HandleOnItemUpdated);
		}
	}

	Super::NativeConstruct();
}

void UItemAscensionDashboardUI::NativeDestruct()
{
	if (IsValid(RankUpButton)) RankUpButton->OnClicked.RemoveAll(this);
	if (IsValid(LevelUpButton)) LevelUpButton->OnClicked.RemoveAll(this);

	if (IsValid(InventorySubsystem))
	{
		InventorySubsystem->OnItemAdded.RemoveAll(this);
	}
	AscensionSubsystem = nullptr;
	InventorySubsystem = nullptr;
	ActiveItemAscension = nullptr;

	Super::NativeDestruct();
}

