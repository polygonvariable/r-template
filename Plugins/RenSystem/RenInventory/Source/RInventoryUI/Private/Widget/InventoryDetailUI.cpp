// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Asset/RPrimaryDataAsset.h"
#include "Definition/Runtime/InventoryItem.h"
#include "Library/InventoryPrimaryAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AscensionDetailUI.h"
#include "Widget/InventoryEntry.h"



void UInventoryDetailUI::RefreshDetails()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
		return;
	}

	const FInventoryItem* Item = Inventory->GetItemById(ContainerId, ActiveAssetId, ActiveItemId);
	if (!Item)
	{
		LOG_ERROR(LogInventory, TEXT("Item is invalid"));
		return;
	}

	int Quantity = Item->Quantity;
	SetCustomDetails(Item, Quantity);
}

void UInventoryDetailUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}
	EntryName->SetText(Asset->DisplayName);
	EntryDescription->SetText(Asset->Description);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UInventoryDetailUI::SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
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

	if (IsValid(AscensionDetail)) AscensionDetail->InitializeDetails(Item->Ascension);
}

void UInventoryDetailUI::NativeConstruct()
{
	UInventorySubsystem* Inventory = UInventorySubsystem::Get(GetGameInstance());
	if (IsValid(Inventory))
	{
		if (bAutoRefresh)
		{
			Inventory->OnInventoryRefreshed.AddWeakLambda(this, [this](const FGuid& InventoryId) { if (ContainerId == InventoryId) RefreshDetails(); });
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

