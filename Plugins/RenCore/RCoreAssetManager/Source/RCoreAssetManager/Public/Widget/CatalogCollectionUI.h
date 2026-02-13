// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "CatalogCollectionUI.generated.h"

// Forward Declarations
class UListView;

class UCatalogEntry;

class UFilterGroup;
class UFilterCriterion;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCatalogCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API virtual void DisplayEntries();

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API void ClearEntries(bool bRegenerate);

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API virtual void RefreshEntries();

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API UCatalogEntry* GetSelectedEntry();

	RCOREASSETMANAGER_API UFilterCriterion* GetCriterionByName(FName Name) const;

	template<typename T>
	T* GetCriterionByName(FName Name)
	{
		return Cast<T>(GetCriterionByName(Name));
	}

	DECLARE_DELEGATE_OneParam(FOnEntityEntrySelected, const UCatalogEntry* /* Entry */);
	FOnEntityEntrySelected OnEntrySelected;

protected:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> EntryList = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UCatalogEntry>> EntryPool;

	RCOREASSETMANAGER_API const UFilterCriterion* GetFilterRoot() const;

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API void AddEntry(UCatalogEntry* Entry);


	RCOREASSETMANAGER_API UCatalogEntry* GetEntryFromPool(const TSubclassOf<UCatalogEntry>& EntryClass);
	RCOREASSETMANAGER_API void ReturnEntryToPool(UCatalogEntry* Item);


	template<typename T>
	T* GetEntryFromPool()
	{
		return Cast<T>(GetEntryFromPool(T::StaticClass()));
	}

	void HandleSelectedEntry(UObject* Object);

	// ~ UUserWidget
	RCOREASSETMANAGER_API virtual void NativeConstruct() override;
	RCOREASSETMANAGER_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

