// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CatalogDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Widget/CatalogEntry.h"



void UCatalogDashboardUI::GetAllCatalogUI_Implementation(TArray<UCatalogUI*>& OutFilters) const
{

}

void UCatalogDashboardUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	TArray<UCatalogUI*> CatalogUI;
	GetAllCatalogUI(CatalogUI);

	for (UCatalogUI* Catalog : CatalogUI)
	{
		if (IsValid(Catalog) && Catalog != this)
		{
			Catalog->InitializeDetails(Entry, Asset);
		}
	}
}

void UCatalogDashboardUI::NativeConstruct()
{
	if (IsValid(CloseButton)) CloseButton->OnClicked.AddDynamic(this, &UCatalogDashboardUI::CloseWidget);

	Super::NativeConstruct();
}

void UCatalogDashboardUI::NativeDestruct()
{
	if (IsValid(CloseButton)) CloseButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

