// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "PartySlotUI.generated.h"

// Forward Declarations
class UImage;
class UButton;
class UTextBlock;
class URPrimaryDataAsset;
class UPartyStorage;



/**
 *
 */
UCLASS(Abstract)
class UPartySlotUI : public UAssetUI
{

	GENERATED_BODY()

public:

	// ~ UAssetUI
	virtual void ResetDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetUI

protected:

	UPROPERTY(EditAnywhere)
	int CharacterSlot = 0;

	UPROPERTY(EditAnywhere)
	bool bAllowSlotEdit = false;

	UPROPERTY(EditAnywhere)
	bool bHideOnEmpty = false;

	UPROPERTY(EditAnywhere)
	FText EmptyText = FText::FromString("Empty");

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> EmptyIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> AssetIcon = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> AssetDisplayName = nullptr;

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> ClearSlotButton = nullptr;

	UPROPERTY()
	TObjectPtr<UPartyStorage> PartyStorage = nullptr;


	UFUNCTION(BlueprintCallable)
	void ClearSlot();

	// ~ UAssetUI
	virtual void SetPrimaryDetail(const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~ End of UUserWidget

};

