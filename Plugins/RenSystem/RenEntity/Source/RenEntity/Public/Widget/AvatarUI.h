// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "AvatarUI.generated.h"

// Forward Declarations
class URAssetManager;
class UAvatarAsset;
class UAvatarEntry;

struct FAvatarRecord;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAvatarUI : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual void InitializeDetails(const UAvatarEntry* Entry);
	virtual void InitializeDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record);

	UFUNCTION(BlueprintCallable)
	virtual void RefreshDetails();

	UFUNCTION(BlueprintCallable)
	virtual void ResetDetails();

protected:

	UPROPERTY()
	FGuid LatentId;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	virtual void SetPrimaryDetails(const UAvatarAsset* Asset);
	virtual void SetSecondaryDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record);

	virtual void CancelLatentFetch();

	UFUNCTION(BlueprintCallable)
	virtual void CloseWidget();

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

