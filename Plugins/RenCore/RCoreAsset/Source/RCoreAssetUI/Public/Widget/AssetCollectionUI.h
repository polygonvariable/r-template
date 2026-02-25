// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "InstancedStruct.h"

// Project Headers

// Generated Headers
#include "AssetCollectionUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API

// Forward Declarations
class UListView;
class UAssetEntry;
class UFilterGroup;
class UFilterCriterion;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FOnEntrySelected, const UAssetEntry* /* Entry */);
	FOnEntrySelected OnEntrySelected;


	RCORE_API virtual void SetCatalogId(const FGuid& Id);
	RCORE_API virtual void DisplayEntries();
	RCORE_API virtual void ClearEntries(bool bRegenerate);
	RCORE_API virtual void RefreshEntries();

	RCORE_API UAssetEntry* GetSelectedEntry();
	template<typename T>
	T* GetSelectedEntry()
	{
		return Cast<T>(GetSelectedEntry());
	}


	RCORE_API void AddSubDetails(const FPrimaryAssetId& Id, const FInstancedStruct& Detail);
	RCORE_API void RemoveSubDetails(const FPrimaryAssetId& Id);
	RCORE_API void ClearSubDetails();


	RCORE_API UFilterCriterion* GetCriterionByName(FName Name) const;

	template<typename T>
	T* GetCriterionByName(FName Name)
	{
		return Cast<T>(GetCriterionByName(Name));
	}

protected:

	UPROPERTY(EditAnywhere)
	FGuid CatalogId;

	UPROPERTY()
	TMap<FPrimaryAssetId, FInstancedStruct> SubDetails;

	UPROPERTY(EditAnywhere)
	bool bAutoSelectAfterRefresh = false;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> EntryList = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UAssetEntry>> EntryPool;


	RCORE_API const UFilterCriterion* GetFilterRoot() const;


	RCORE_API void AddEntry(const FPrimaryAssetId& AssetId, UAssetEntry* Entry);
	RCORE_API void ReturnEntryToPool(UAssetEntry* Item);
	RCORE_API UAssetEntry* GetEntryFromPool(const TSubclassOf<UAssetEntry>& EntryClass);

	template<typename T>
	T* GetEntryFromPool()
	{
		return Cast<T>(GetEntryFromPool(T::StaticClass()));
	}


	void HandleOnItemSelectionChanged(UObject* Object);


	// ~ UUserWidget
	RCORE_API virtual void NativeConstruct() override;
	RCORE_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



// Module Macros
#undef RCORE_API

