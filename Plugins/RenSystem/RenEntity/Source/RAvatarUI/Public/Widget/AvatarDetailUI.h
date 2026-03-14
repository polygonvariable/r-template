// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "AvatarDetailUI.generated.h"

// Forward Declarations
class UAscensionDetailUI;
class UAvatarStorage;
class URPrimaryDataAsset;

struct FAvatarData;



/**
 *
 */
UCLASS(Abstract)
class UAvatarDetailUI : public UAssetDetailUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDetailUI
	virtual void InitializeDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetDetailUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAscensionDetailUI> AscensionDetail = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAvatarStorage> AvatarCollection = nullptr;


	virtual void SetCustomDetails(const FAvatarData* Item);

	// ~ UAssetDetailUI
	virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset) override;
	// ~ End of UAssetDetailUI

	// ~ UUserWidget
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

