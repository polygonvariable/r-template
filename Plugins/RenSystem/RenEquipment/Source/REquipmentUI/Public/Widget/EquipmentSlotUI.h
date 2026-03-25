// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "EquipmentSlotUI.generated.h"

// Forward Declarations
class UImage;
class UButton;
class UTextBlock;
class URPrimaryDataAsset;
class UEquipmentStorage;



/**
 *
 */
UCLASS(Abstract)
class UEquipmentSlotUI : public UAssetUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FGameplayTag SlotTag;

	// ~ UAssetUI
	virtual void ResetDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetUI

protected:

	UPROPERTY(VisibleAnywhere)
	FGuid OwnerId;

	UPROPERTY(VisibleAnywhere)
	FPrimaryAssetId OwnerAssetId;

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> EquipmentStorage = nullptr;

	UPROPERTY(EditAnywhere)
	bool bAllowEdit = false;

	UPROPERTY(EditAnywhere)
	bool bCollapseOnEmpty = false;

	UPROPERTY(EditAnywhere)
	FText EmptyText = FText::FromString("Empty");

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> EmptyIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> AssetIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> AssetDisplayName = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> ClearButton = nullptr;


	UFUNCTION(BlueprintCallable)
	void ClearSlot();

	// ~ UAssetUI
	virtual void SetPrimaryDetail(const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~ End of UUserWidget

};

