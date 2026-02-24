// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "ShopCollectionUI.generated.h"

// Forward Declarations
class UShopAsset;
class URAssetManager;



/**
 *
 */
UCLASS(Abstract)
class UShopCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	void SetShopAssetId(FPrimaryAssetId AssetId);

	// ~ UAssetCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY()
	URAssetManager* AssetManager = nullptr;

	FGuid ShopLoadId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId ShopAssetId;

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	void StartShopLoad();
	void CancelShopLoad();
	void OnShopLoaded(UShopAsset* ShopAsset);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

