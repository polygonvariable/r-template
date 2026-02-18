// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CatalogEntryUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Widget/CatalogEntry.h"



void UCatalogEntryUI::ResetDetails()
{
	CancelAssetLoading();

	SetPrimaryDetails(nullptr, nullptr);
}

void UCatalogEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CancelAssetLoading();

	UCatalogEntry* Entry = Cast<UCatalogEntry>(ListItemObject);
	InitializeDetails(Entry);

	Execute_OnListItemObjectSet(this, ListItemObject);
}

void UCatalogEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{
	Execute_BP_OnItemSelectionChanged(this, bSelected);
}

