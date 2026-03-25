// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "EquipmentDashboardUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UAssetDetailUI;
class UAssetEntry;
class URPrimaryDataAsset;
class UEquipmentSlotUI;
class UPanelWidget;



/**
 *
 */
UCLASS(Abstract)
class UEquipmentDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	virtual void ResetDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	bool bRebuild = false;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> EquipmentCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> OwnerDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UPanelWidget> SlotBox = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEquipmentSlotUI> SlotClass = nullptr;

	UPROPERTY(EditAnywhere)
	FMargin SlotSpacing;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SyncButton = nullptr;

	FGuid OwnerId;


	UFUNCTION()
	void SyncEquipment();

	// ~ UAssetDashboardUI
	virtual void SetPrimaryDetail(const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

