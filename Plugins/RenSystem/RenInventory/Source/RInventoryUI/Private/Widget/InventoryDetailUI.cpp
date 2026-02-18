// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Definition/InventoryItem.h"
#include "Definition/InventoryPrimaryAsset.h"
#include "InventoryEntry.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AscensionDetailUI.h"



void UInventoryDetailUI::RefreshDetails()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
		return;
	}

	const FInventoryItem* Item = Inventory->GetItemById(CatalogId, ActiveAssetId, ActiveItemId);
	if (!Item)
	{
		LOG_ERROR(LogInventory, TEXT("Item is invalid"));
		return;
	}

	int Quantity = Item->Quantity;
	SetCustomDetails(Item, Quantity);
}

bool UInventoryDetailUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return InventoryPrimaryAsset::IsValid(AssetId);
}

void UInventoryDetailUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	const UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(Asset);
	if (!IsValid(InventoryAsset))
	{
		return;
	}

	if (IsValid(EntryName)) EntryName->SetText(InventoryAsset->DisplayName);
	if (IsValid(EntryDescription)) EntryDescription->SetText(InventoryAsset->Description);
	if (IsValid(EntryIcon)) EntryIcon->SetBrushFromSoftTexture(InventoryAsset->Icon);
}

void UInventoryDetailUI::SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
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
	if (IsValid(EntryQuantity)) EntryQuantity->SetText(FText::AsNumber(Quantity));
	if (IsValid(AscensionDetail)) AscensionDetail->InitializeDetails(Item->Ascension);
}

void UInventoryDetailUI::NativeConstruct()
{
	UInventorySubsystem* Inventory = UInventorySubsystem::Get(GetGameInstance());
	if (IsValid(Inventory))
	{
		if (bAutoRefresh)
		{
			Inventory->OnInventoryRefreshed.AddWeakLambda(this, [this](const FGuid& InventoryId) { if (CatalogId == InventoryId) RefreshDetails(); });
		}
		InventorySubsystem = TWeakObjectPtr<UInventorySubsystem>(Inventory);
	}

	Super::NativeConstruct();
}

void UInventoryDetailUI::NativeDestruct()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnInventoryRefreshed.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}

