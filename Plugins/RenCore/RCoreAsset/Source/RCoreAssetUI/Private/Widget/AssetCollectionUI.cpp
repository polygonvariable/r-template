// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "Filter/FilterGroup.h"
#include "Widget/AssetEntry.h"



void UAssetCollectionUI::SetContainerId(const FGuid& Id)
{
	ContainerId = Id;
}

void UAssetCollectionUI::DisplayEntries()
{
}

void UAssetCollectionUI::ClearEntries(bool bRegenerate)
{
	if (!IsValid(EntryList))
	{
		return;
	}

	const TArray<UObject*>& Items = EntryList->GetListItems();
	for (UObject* Item : Items)
	{
		UAssetEntry* Entry = Cast<UAssetEntry>(Item);
		if (!IsValid(Entry))
		{
			continue;
		}
		Entry->ResetData();
		ReturnEntryToPool(Entry);
	}

	EntryList->ClearListItems();

	if (bRegenerate)
	{
		EntryList->RegenerateAllEntries();
	}
}

void UAssetCollectionUI::RefreshEntries()
{
	ClearEntries(true);
	DisplayEntries();
}

UAssetEntry* UAssetCollectionUI::GetSelectedEntry()
{
	if (!IsValid(EntryList))
	{
		return nullptr;
	}
	return EntryList->GetSelectedItem<UAssetEntry>();
}



void UAssetCollectionUI::AddSubDetails(const FPrimaryAssetId& Id, const FInstancedStruct& Detail)
{
	SubDetails.Add(Id, Detail);
}

void UAssetCollectionUI::RemoveSubDetails(const FPrimaryAssetId& Id)
{
	SubDetails.Remove(Id);
}

void UAssetCollectionUI::ClearSubDetails()
{
	SubDetails.Empty();
}



UFilterCriterion* UAssetCollectionUI::GetCriterionByName(FName Name) const
{
	if (!IsValid(FilterRule))
	{
		return nullptr;
	}
	return FilterRule->GetCriterionByName(Name);
}

const UFilterCriterion* UAssetCollectionUI::GetFilterRoot() const
{
	UFilterCriterion* CriterionRoot = nullptr;
	if (IsValid(FilterRule))
	{
		return FilterRule->CriterionRoot;
	}
	return CriterionRoot;
}



void UAssetCollectionUI::AddEntry(const FPrimaryAssetId& AssetId, UAssetEntry* Entry)
{
	if (!IsValid(Entry) || !IsValid(EntryList))
	{
		return;
	}

	FInstancedStruct* Detail = SubDetails.Find(AssetId);
	if (Detail != nullptr)
	{
		Entry->AssetSubDetail = *Detail;
	}
	Entry->AssetId = AssetId;

	EntryList->AddItem(Entry);
}

void UAssetCollectionUI::ReturnEntryToPool(UAssetEntry* Item)
{
	if (IsValid(Item))
	{
		EntryPool.Add(Item);
	}
}

UAssetEntry* UAssetCollectionUI::GetEntryFromPool(const TSubclassOf<UAssetEntry>& EntryClass)
{
	if (EntryPool.Num() > 0)
	{
		return EntryPool.Pop();
	}
	return NewObject<UAssetEntry>(this, EntryClass);
}



void UAssetCollectionUI::HandleOnItemSelectionChanged(UObject* Object)
{
	const UAssetEntry* Entry = Cast<UAssetEntry>(Object);
	if (IsValid(Entry))
	{
		OnEntrySelected.ExecuteIfBound(Entry);
	}
}

void UAssetCollectionUI::NativeConstruct()
{
	if (IsValid(EntryList))
	{
		EntryList->OnItemSelectionChanged().AddUObject(this, &UAssetCollectionUI::HandleOnItemSelectionChanged);
	}

	Super::NativeConstruct();
}

void UAssetCollectionUI::NativeDestruct()
{
	ClearEntries(false);

	if (IsValid(EntryList))
	{
		EntryList->OnItemSelectionChanged().RemoveAll(this);
	}

	Super::NativeDestruct();
}

