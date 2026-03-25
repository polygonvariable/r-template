// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/PartySlotUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Manager/RAssetManager.inl"
#include "Storage/PartyStorage.h"
#include "Subsystem/PartySubsystem.h"
#include "Widget/Drag/AssetDragOperation.h"




void UPartySlotUI::ResetDetail()
{
	AssetDisplayName->SetText(EmptyText);
	AssetIcon->SetBrushFromSoftTexture(EmptyIcon);

	if (bHideOnEmpty)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPartySlotUI::RefreshDetail()
{
	if (!IsValid(PartyStorage))
	{
		ResetDetail();
		return;
	}

	FPrimaryAssetId AssetId = PartyStorage->GetCharacterAtSlot(CharacterSlot);
	if (!AssetId.IsValid())
	{
		ResetDetail();
		return;
	}

	InitializeAssetById(AssetId);
}

void UPartySlotUI::ClearSlot()
{
	if (bAllowSlotEdit && IsValid(PartyStorage))
	{
		PartyStorage->ClearSlot(CharacterSlot);
	}
}

void UPartySlotUI::SetPrimaryDetail(const URPrimaryDataAsset* Asset)
{
	AssetDisplayName->SetText(Asset->DisplayName);
	AssetIcon->SetBrushFromSoftTexture(Asset->Icon);

	if (bHideOnEmpty)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UPartySlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAllowSlotEdit && IsValid(ClearSlotButton))
	{
		ClearSlotButton->OnClicked.AddDynamic(this, &UPartySlotUI::ClearSlot);
	}

	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		PartyStorage = PartySubsystem->GetPartyCollection();
		if (IsValid(PartyStorage))
		{
			PartyStorage->OnStorageUpdated.AddUObject(this, &UPartySlotUI::RefreshDetail);
			RefreshDetail();
		}
	}
}

void UPartySlotUI::NativeDestruct()
{
	if (IsValid(ClearSlotButton)) ClearSlotButton->OnClicked.Clear();

	if (IsValid(PartyStorage))
	{
		PartyStorage->OnStorageUpdated.RemoveAll(this);
	}
	PartyStorage = nullptr;

	Super::NativeDestruct();
}

bool UPartySlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!bAllowSlotEdit)
	{
		return false;
	}

	UAssetDragOperation* DragOperation = Cast<UAssetDragOperation>(InOperation);
	if (!IsValid(DragOperation))
	{
		return false;
	}

	const FPrimaryAssetId& AssetId = DragOperation->AssetId;
	if (!AssetId.IsValid())
	{
		return false;
	}

	return PartyStorage->SetCharacterAtSlot(CharacterSlot, AssetId);
}

