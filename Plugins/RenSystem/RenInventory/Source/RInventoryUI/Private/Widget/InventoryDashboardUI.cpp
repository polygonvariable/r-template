// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Widget/InventoryCollectionUI.h"
#include "Widget/InventoryDetailUI.h"
#include "Widget/InventoryFilterUI.h"



void UInventoryDashboardUI::SetContainerId(FGuid Id)
{
	ContainerId = Id;
}

void UInventoryDashboardUI::GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutComponent) const
{
	OutComponent.Add(CatalogDetail);
}

void UInventoryDashboardUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(CatalogCollection)) CatalogCollection->SetContainerId(ContainerId);
	if (IsValid(CatalogDetail)) CatalogDetail->SetContainerId(ContainerId);
}

void UInventoryDashboardUI::NativeConstruct()
{
	if (IsValid(CatalogCollection)) CatalogCollection->OnEntrySelected.BindUObject(this, &UInventoryDashboardUI::InitializeDetails);
	
	Super::NativeConstruct();
}

void UInventoryDashboardUI::NativeDestruct()
{
	if (IsValid(CatalogCollection)) CatalogCollection->OnEntrySelected.Unbind();

	Super::NativeDestruct();
}

