// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

// Engine Headers
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "InventoryAsset.h"
#include "InventoryRecord.h"
#include "Widget/AscensionDetailUI.h"

#include "AssetManagerUtils.h"
#include "LogCategory.h"
#include "LogMacro.h"

#include "InventoryEntry.h"
#include "InventoryPrimaryAsset.h"
#include "InventorySubsystem.h"




void UInventoryDetailUI::SetContainerId(FGuid Id)
{
	ContainerId = Id;
}

void UInventoryDetailUI::RefreshDetails()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
		return;
	}

	const FInventoryItem* Item = Inventory->GetItemById(ContainerId, ActiveAssetId, ActiveItemId);
	int Quantity = Inventory->GetTotalQuantity(ContainerId, ActiveAssetId);
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
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (IsValid(Inventory))
		{
			if (bAutoRefresh)
			{
				Inventory->OnItemAdded.AddWeakLambda(this, [this](FGuid InContainerId, const FPrimaryAssetId&, FGuid) { if (ContainerId == InContainerId) RefreshDetails(); });
				Inventory->OnItemRemoved.AddWeakLambda(this, [this](FGuid InContainerId, const FPrimaryAssetId&, FGuid) { if (ContainerId == InContainerId) RefreshDetails(); });
				Inventory->OnItemUpdated.AddWeakLambda(this, [this](FGuid InContainerId, const FPrimaryAssetId&, FGuid) { if (ContainerId == InContainerId) RefreshDetails(); });
			}
			InventorySubsystem = TWeakObjectPtr<UInventorySubsystem>(Inventory);
		}
	}

	Super::NativeConstruct();
}

void UInventoryDetailUI::NativeDestruct()
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

