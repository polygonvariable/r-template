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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	FName AssetSourceId = NAME_None;


	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void InitializeDetail();

	RCORE_API virtual void InitializeDetail(const UAssetEntry* Entry);
	RCORE_API virtual void InitializeDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset);

	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void RefreshDetail();

	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void ResetDetail();

	UFUNCTION(BlueprintCallable)
	RCORE_API virtual void CloseWidget();

protected:

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	RCORE_API virtual const FPrimaryAssetId& GetActiveAssetId() const;

	RCORE_API virtual void SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset);
	RCORE_API virtual void SetSecondaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset);

	RCORE_API virtual void CancelInitialization();
	RCORE_API virtual void SwitchDetail(bool bPrimary);

	// ~ UUserWidget
	RCORE_API virtual void NativeConstruct() override;
	RCORE_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	TWeakObjectPtr<const URPrimaryDataAsset> _ActiveAsset;
	FPrimaryAssetId _ActiveAssetId;
	FGuid _ActiveLoadId;

};



// Module Macros
#undef RCORE_API

