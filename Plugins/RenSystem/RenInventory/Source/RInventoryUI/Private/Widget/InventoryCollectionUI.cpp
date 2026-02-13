// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "FilterGroup.h"
#include "FilterLeafCriterion.h"
#include "InventoryEntry.h"
#include "InventoryRecord.h"
#include "InventorySubsystem.h"
#include "LogCategory.h"
#include "LogMacro.h"



void UInventoryCollectionUI::SetContainerId(FGuid Id)
{
	ContainerId = Id;
}

void UInventoryCollectionUI::DisplayEntries()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
		return;
	}

	const UFilterCriterion* FilterRoot = GetFilterRoot();

	Inventory->QueryItems(ContainerId, FilterRoot, QueryRule,
		[this](const FInventorySortEntry& SortEntry)
		{
			UInventoryEntry* Entry = GetEntryFromPool<UInventoryEntry>();
			if (IsValid(Entry))
			{
				Entry->AssetId = SortEntry.AssetId;
				Entry->Item = SortEntry.Item;
				Entry->Quantity = SortEntry.Quantity;

				AddEntry(Entry);
			}
		}
	);
}

void UInventoryCollectionUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (IsValid(Inventory))
		{
			if (bAutoRefresh)
			{
				Inventory->OnItemAdded.AddWeakLambda(this, [this](FGuid InContainerId, const FPrimaryAssetId&, FGuid) { if (ContainerId == InContainerId) RefreshEntries(); });
				Inventory->OnItemRemoved.AddWeakLambda(this, [this](FGuid InContainerId, const FPrimaryAssetId&, FGuid) { if (ContainerId == InContainerId) RefreshEntries(); });
				Inventory->OnItemUpdated.AddWeakLambda(this, [this](FGuid InContainerId, const FPrimaryAssetId&, FGuid) { if (ContainerId == InContainerId) RefreshEntries(); });
			}
			InventorySubsystem = TWeakObjectPtr<UInventorySubsystem>(Inventory);
		}
	}

	Super::NativeConstruct();
}

void UInventoryCollectionUI::NativeDestruct()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}

