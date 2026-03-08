// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/Runtime/InventoryItem.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AscensionDetailUI.h"
#include "Widget/InventoryEntry.h"



void UInventoryDetailUI::InitializeDetail()
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(ContainerId);
	if (IsValid(InventoryStorage) && bAutoRefresh)
	{
		InventoryStorage->OnInventoryRefreshed.AddUObject(this, &UInventoryDetailUI::RefreshDetail);
	}

	Inventory = TWeakObjectPtr<UInventoryStorage>(InventoryStorage);
}

void UInventoryDetailUI::RefreshDetail()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (!IsValid(InventoryStorage))
	{
		LOG_ERROR(LogInventory, TEXT("InventoryStorage is invalid"));
		return;
	}

	const FInventoryItem* Item = InventoryStorage->GetItemById(GetActiveAssetId(), ActiveItemId);
	if (!Item)
	{
		LOG_ERROR(LogInventory, TEXT("Item is invalid"));
		return;
	}

	int Quantity = Item->Quantity;
	SetCustomDetails(Item, Quantity);
}

void UInventoryDetailUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}
	EntryName->SetText(Asset->DisplayName);
	EntryDescription->SetText(Asset->Description);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UInventoryDetailUI::SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!IsValid(InventoryEntry))
	{
		return;
	}

	const FInventoryItem* Item = InventoryEntry->Item;
	int Quantity = InventoryEntry->Quantity;
	SetCustomDetails(Item, Quantity);
}

void UInventoryDetailUI::SetCustomDetails(const FInventoryItem* Item, int Quantity)
{
	if (!Item)
	{
		return;
	}

	ActiveItemId = Item->ItemId;

	EntryQuantity->SetText(FText::AsNumber(Quantity));

	if (IsValid(AscensionDetail)) AscensionDetail->InitializeDetail(Item->Ascension);
}

void UInventoryDetailUI::NativeDestruct()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (IsValid(InventoryStorage))
	{
		InventoryStorage->OnInventoryRefreshed.RemoveAll(this);
	}
	Inventory.Reset();

	Super::NativeDestruct();
}

