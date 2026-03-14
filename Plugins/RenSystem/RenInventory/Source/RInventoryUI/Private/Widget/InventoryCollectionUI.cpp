// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionUI.h"

// Engine Headers

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/InventoryEntry.h"



void UInventoryCollectionUI::InitializeCollection()
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(AssetSourceId);
	if (IsValid(InventoryStorage) && bAutoRefresh)
	{
		InventoryStorage->OnInventoryRefreshed.AddUObject(this, &UInventoryCollectionUI::RefreshEntries);
	}

	Inventory = TWeakObjectPtr<UInventoryStorage>(InventoryStorage);
}

void UInventoryCollectionUI::DisplayEntries()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (!IsValid(InventoryStorage))
	{
		LOG_ERROR(LogInventory, TEXT("InventoryStorage is invalid"));
		return;
	}

	InventoryStorage->QueryItems(GetFilterRoot(), QueryRule,
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

void UInventoryCollectionUI::NativeDestruct()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (IsValid(InventoryStorage))
	{
		InventoryStorage->OnInventoryRefreshed.RemoveAll(this);
	}
	Inventory.Reset();

	Super::NativeDestruct();
}

