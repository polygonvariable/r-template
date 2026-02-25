// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetEntryUI.h"

// Engine Headers

// Project Headers
#include "Widget/AssetEntry.h"



void UAssetEntryUI::ResetDetails()
{
	CancelInitialization();

	SetPrimaryDetails(nullptr, nullptr);
}

void UAssetEntryUI::GetAssetDetail(FInstancedStruct& AssetDetail) const
{
	const UAssetEntry* Entry = GetListItem<UAssetEntry>();
	if (IsValid(Entry))
	{
		AssetDetail = Entry->AssetDetail;
	}
}

void UAssetEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CancelInitialization();

	const UAssetEntry* Entry = Cast<UAssetEntry>(ListItemObject);
	InitializeDetails(Entry);

	Execute_OnListItemObjectSet(this, ListItemObject);
}

void UAssetEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{
	Execute_BP_OnItemSelectionChanged(this, bSelected);
}

