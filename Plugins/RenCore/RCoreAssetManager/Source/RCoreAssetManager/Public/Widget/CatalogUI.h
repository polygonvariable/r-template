// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "CatalogUI.generated.h"

// Forward Declarations
class URAssetManager;
class UCatalogEntry;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCatalogUI : public UUserWidget
{

	GENERATED_BODY()

public:

	RCOREASSETMANAGER_API virtual void InitializeDetails(const UCatalogEntry* Entry);
	RCOREASSETMANAGER_API virtual void InitializeDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset);

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API virtual void RefreshDetails();

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API virtual void ResetDetails();

	UFUNCTION(BlueprintCallable)
	RCOREASSETMANAGER_API virtual void CloseWidget();

protected:

	UPROPERTY()
	FGuid LatentId;

	UPROPERTY()
	FPrimaryAssetId ActiveAssetId;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	RCOREASSETMANAGER_API virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const;

	RCOREASSETMANAGER_API virtual void SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset);
	RCOREASSETMANAGER_API virtual void SetSecondaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset);

	RCOREASSETMANAGER_API virtual void CancelAssetLoading();
	RCOREASSETMANAGER_API virtual void SwitchDetails(bool bPrimary);

	// ~ UUserWidget
	RCOREASSETMANAGER_API virtual void NativeConstruct() override;
	RCOREASSETMANAGER_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

