// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentDashboardUI.h"

// Engine Headers
#include "Components/PanelWidget.h"
#include "Components/Button.h"

// Project Headers
#include "Settings/EquipmentSettings.h"
#include "Subsystem/EquipmentSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/EquipmentSlotUI.h"



void UEquipmentDashboardUI::InitializeDetail()
{
	//OwnerDetail->PrimarySourceId = PrimarySourceId;
	OwnerDetail->InitializeDetail();

	//EquipmentCollection->PrimarySourceId = PrimarySourceId;
	EquipmentCollection->InitializeCollection();
	EquipmentCollection->DisplayEntries();
}

void UEquipmentDashboardUI::ResetDetail()
{

}

void UEquipmentDashboardUI::SyncEquipment()
{
	UEquipmentSubsystem* EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
	if (!IsValid(EquipmentSubsystem))
	{
		return;
	}
	EquipmentSubsystem->SyncEquipment(OwnerId);
}

void UEquipmentDashboardUI::SetPrimaryDetail(const URPrimaryDataAsset* Asset)
{
	OwnerDetail->InitializeAssetDetail(Asset);
}

void UEquipmentDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	if (!IsValid(Entry))
	{
		return;
	}

	OwnerId = Entry->GetAssetInstanceId();

	OwnerDetail->InitializeEntryDetail(Entry);

	TArray<UWidget*> Children = SlotBox->GetAllChildren();
	for (UWidget* Child : Children)
	{
		UAssetUI* SlotUI = Cast<UAssetUI>(Child);
		if (IsValid(SlotUI))
		{
			SlotUI->InitializeEntryDetail(Entry);
		}
	}
}

void UEquipmentDashboardUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FGameplayTagContainer& EquipmentSlots = Settings->EquipmentSlots;

	int NumSlots = EquipmentSlots.Num();
	int NumChildren = SlotBox->GetChildrenCount();

	if (NumChildren != NumSlots)
	{
		SlotBox->ClearChildren();

		for (int i = 0; i < NumSlots; i++)
		{
			UEquipmentSlotUI* SlotUI = CreateWidget<UEquipmentSlotUI>(this, SlotClass);
			if (IsValid(SlotUI))
			{
				SlotUI->SlotTag = EquipmentSlots.GetByIndex(i);
				SlotUI->SetPadding(SlotSpacing);
				SlotBox->AddChild(SlotUI);
			}
		}
	}
}

void UEquipmentDashboardUI::NativeConstruct()
{
	SyncButton->OnClicked.AddDynamic(this, &UEquipmentDashboardUI::SyncEquipment);
	Super::NativeConstruct();
}

void UEquipmentDashboardUI::NativeDestruct()
{
	SyncButton->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}

