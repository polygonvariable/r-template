// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "InstancedStruct.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/Runtime/InventoryItem.h"
#include "Filter/FilterLeafCriterion.h"
#include "Interface/IAscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventoryAscensionSubsystem.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/InventoryEntry.h"



void UInventoryAscensionDashboardUI::InitializeDetail()
{
	InventoryDetail->AssetSourceId = AssetSourceId;
	InventoryDetail->InitializeDetail();

	LevelItemCollection->AssetSourceId = AssetSourceId;
	LevelItemCollection->InitializeCollection();

	RankItemCollection->AssetSourceId = AssetSourceId;
	RankItemCollection->InitializeCollection();

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(AssetSourceId);
	if (IsValid(InventoryStorage) && bAutoRefresh)
	{
		InventoryStorage->OnInventoryRefreshed.AddUObject(this, &UInventoryAscensionDashboardUI::HandleOnItemUpdated);
	}

	Inventory = TWeakObjectPtr<UInventoryStorage>(InventoryStorage);
}

void UInventoryAscensionDashboardUI::RefreshDetail()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (!IsValid(InventoryStorage))
	{
		return;
	}

	const FInventoryItem* Item = InventoryStorage->GetItemById(GetActiveAssetId(), ActiveItemId);
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

	RankItemCollection->ClearSubDetails();

	UFilterAssetCriterion* AssetFilter = RankItemCollection->GetCriterionByName<UFilterAssetCriterion>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const FAscensionData& AscensionData = Item->Ascension;
		const UAssetCollection* ItemCollection = ActiveItemAscension->GetRankAssets(AscensionData);
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



void UInventoryAscensionDashboardUI::HandleTaskCallback(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Pending)
	{
		DisableControls();
		LOG_WARNING(LogInventoryAscension, TEXT("Task Started"));
	}
	else
	{
		EnableControls();
		LOG_WARNING(LogInventoryAscension, TEXT("Task Finished, Message: %s"), *Result.Message);
	}
}



void UInventoryAscensionDashboardUI::HandleLevelUp()
{
	const UAssetEntry* Entry = LevelItemCollection->GetSelectedEntry();
	if (!IsValid(AscensionSubsystem) || !IsValid(Entry))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("AscensionSubsystem, Entry is not valid"));
		return;
	}

	FGuid MaterialId = Entry->GetAssetInstanceId();
	FPrimaryAssetId MaterialAssetId = Entry->AssetId;
	
	AscensionSubsystem->AddExperiencePoints(AssetSourceId, GetActiveAssetId(), ActiveItemId, MaterialAssetId, MaterialId, FTaskCallback::CreateUObject(this, &UInventoryAscensionDashboardUI::HandleTaskCallback));
}

void UInventoryAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	AscensionSubsystem->AddRankPoints(AssetSourceId, GetActiveAssetId(), ActiveItemId, FTaskCallback::CreateUObject(this, &UInventoryAscensionDashboardUI::HandleTaskCallback));
}

void UInventoryAscensionDashboardUI::HandleOnItemUpdated()
{
	RefreshDetail();
}



void UInventoryAscensionDashboardUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	InventoryDetail->InitializeDetail(Entry, Asset);
}

void UInventoryAscensionDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(Asset);
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

	UFilterAssetCriterion* AssetIdFilter = LevelItemCollection->GetCriterionByName<UFilterAssetCriterion>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetIdFilter))
	{
		AssetIdFilter->Included.Empty();

		const FAscensionData& AscensionData = Item->Ascension;
		const UAssetCollection* ItemCollection = AscensionProvider->GetExperienceAssets(AscensionData);
		if (IsValid(ItemCollection))
		{
			TArray<FPrimaryAssetId> AssetList;
			ItemCollection->GetAssetIds(AssetList);

			AssetIdFilter->Included.Append(AssetList);
		}
	}

	LevelItemCollection->DisplayEntries();

	ToggleAscension(Item);
}



void UInventoryAscensionDashboardUI::NativeConstruct()
{
	RankUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UInventoryAscensionSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UInventoryAscensionDashboardUI::NativeDestruct()
{
	RankUpButton->OnClicked.RemoveAll(this);
	LevelUpButton->OnClicked.RemoveAll(this);

	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (IsValid(InventoryStorage))
	{
		InventoryStorage->OnInventoryRefreshed.RemoveAll(this);
	}
	Inventory.Reset();

	AscensionSubsystem = nullptr;
	ActiveItemAscension = nullptr;

	Super::NativeDestruct();
}

