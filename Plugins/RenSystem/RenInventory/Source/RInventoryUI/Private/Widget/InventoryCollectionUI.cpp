// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionUI.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/InventoryEntry.h"



void UInventoryCollectionUI::DisplayEntries()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
		return;
	}

	const UFilterCriterion* FilterRoot = GetFilterRoot();

	Inventory->QueryItems(CatalogId, FilterRoot, QueryRule,
		[this](const FInventorySortEntry& SortEntry)
		{
			UInventoryEntry* Entry = GetEntryFromPool<UInventoryEntry>();
			if (IsValid(Entry))
			{
				Entry->Item = SortEntry.Item;
				Entry->Quantity = SortEntry.Quantity;
				AddEntry(SortEntry.AssetId, Entry);
			}
		}
	);
}

void UInventoryCollectionUI::OnInventoryRefreshed(const FGuid& InventoryId)
{
	if (CatalogId == InventoryId)
	{
		RefreshEntries();
	}
}

void UInventoryCollectionUI::NativeConstruct()
{
	UInventorySubsystem* Inventory = UInventorySubsystem::Get(GetGameInstance());
	if (IsValid(Inventory))
	{
		if (bAutoRefresh)
		{
			Inventory->OnInventoryRefreshed.AddUObject(this, &UInventoryCollectionUI::OnInventoryRefreshed);
		}
		InventorySubsystem = TWeakObjectPtr<UInventorySubsystem>(Inventory);
	}

	Super::NativeConstruct();
}

void UInventoryCollectionUI::NativeDestruct()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnInventoryRefreshed.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}

