// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CatalogCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"
#include "RCoreFilter/Public/FilterGroup.h"

#include "Log/LogCategory.h"
#include "Log/LogMacro.h"

#include "Widget/CatalogEntry.h"



void UCatalogCollectionUI::SetCatalogId(const FGuid& Id)
{
	CatalogId = Id;
}

const UFilterCriterion* UCatalogCollectionUI::GetFilterRoot() const
{
	UFilterCriterion* CriterionRoot = nullptr;
	if (IsValid(FilterRule))
	{
		return FilterRule->CriterionRoot;
	}
	return CriterionRoot;
}

UFilterCriterion* UCatalogCollectionUI::GetCriterionByName(FName Name) const
{
	if (!IsValid(FilterRule))
	{
		return nullptr;
	}
	return FilterRule->GetCriterionByName(Name);
}




UCatalogEntry* UCatalogCollectionUI::GetEntryFromPool(const TSubclassOf<UCatalogEntry>& EntryClass)
{
	if (EntryPool.Num() > 0)
	{
		return EntryPool.Pop();
	}
	return NewObject<UCatalogEntry>(this, EntryClass);
}

void UCatalogCollectionUI::ReturnEntryToPool(UCatalogEntry* Item)
{
	if (IsValid(Item))
	{
		EntryPool.Add(Item);
	}
}




void UCatalogCollectionUI::AddEntry(UCatalogEntry* Entry)
{
	if (!IsValid(Entry) || !IsValid(EntryList))
	{
		return;
	}
	EntryList->AddItem(Entry);
}

void UCatalogCollectionUI::DisplayEntries()
{
}

void UCatalogCollectionUI::ClearEntries(bool bRegenerate)
{
	if (!IsValid(EntryList))
	{
		return;
	}

	const TArray<UObject*>& Items = EntryList->GetListItems();
	for (UObject* Item : Items)
	{
		UCatalogEntry* Entry = Cast<UCatalogEntry>(Item);
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

void UCatalogCollectionUI::RefreshEntries()
{
	ClearEntries(true);
	DisplayEntries();
}

UCatalogEntry* UCatalogCollectionUI::GetSelectedEntry()
{
	if (!IsValid(EntryList))
	{
		return nullptr;
	}
	return EntryList->GetSelectedItem<UCatalogEntry>();
}

void UCatalogCollectionUI::HandleSelectedEntry(UObject* Object)
{
	const UCatalogEntry* Entry = Cast<UCatalogEntry>(Object);
	if (IsValid(Entry))
	{
		OnEntrySelected.ExecuteIfBound(Entry);
	}
}

void UCatalogCollectionUI::NativeConstruct()
{
	if (IsValid(EntryList))
	{
		EntryList->OnItemSelectionChanged().AddUObject(this, &UCatalogCollectionUI::HandleSelectedEntry);
	}

	Super::NativeConstruct();
}

void UCatalogCollectionUI::NativeDestruct()
{
	ClearEntries(false);

	if (IsValid(EntryList))
	{
		EntryList->OnItemSelectionChanged().RemoveAll(this);
	}

	Super::NativeDestruct();
}

