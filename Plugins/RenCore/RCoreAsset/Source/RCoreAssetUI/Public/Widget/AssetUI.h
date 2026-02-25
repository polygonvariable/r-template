// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "AssetUI.generated.h"

// Module Macros
#define RCORE_API RCOREASSETUI_API

// Forward Declarations
class URAssetManager;
class UAssetEntry;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetUI : public UUserWidget
{

	GENERATED_BODY()

public:

	RCORE_API virtual void SetCatalogId(const FGuid& Id);

	RCORE_API virtual void InitializeDetails(const UAssetEntry* Entry);
	RCORE_API virtual void InitializeDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset);

	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void RefreshDetails();

	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void ResetDetails();

	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void CloseWidget();

protected:

	UPROPERTY(EditAnywhere)
	FGuid CatalogId;

	UPROPERTY()
	FGuid LatentId;

	UPROPERTY()
	FPrimaryAssetId ActiveAssetId;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	RCORE_API virtual bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const;

	RCORE_API virtual void SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset);
	RCORE_API virtual void SetSecondaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset);

	RCORE_API virtual void CancelInitialization();
	RCORE_API virtual void SwitchDetails(bool bPrimary);

	// ~ UUserWidget
	RCORE_API virtual void NativeConstruct() override;
	RCORE_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



// Module Macros
#undef RCORE_API

