// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "ShopSubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RSHOP_API

// Forward Declarations
class UAssetCollection;
class UStorage;
class UShopStorage;
class UTradeAsset;
class URPrimaryDataAsset;
class IStorageProvider;

struct FAssetDetail_Trade;
struct FInstancedStruct;



/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UShopSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	RSYSTEM_API UShopStorage* GetShop(const FName& ShopId);

	RSYSTEM_API void PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);

	RSYSTEM_API const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FInstancedStruct& Context) const;
	RSYSTEM_API const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FGuid& CollectionId) const;

	RSYSTEM_API void QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&)> Callback);

protected:

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	RSYSTEM_API static UShopSubsystem* Get(UWorld* World);
	RSYSTEM_API static UShopSubsystem* Get(UGameInstance* GameInstance);

};



// Module Macros
#undef RSYSTEM_API

