// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "ShopSubsystem.generated.h"

// Forward Declarations
class UAssetCollection;
class UStorage;
class IStorageProviderInterface;
class UShopStorage;
class UShopAsset;
class URPrimaryDataAsset;
struct FAssetDetail_Trade;
struct FInstancedStruct;



/**
 *
 */
UCLASS(NotBlueprintType)
class UShopSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);

	UShopStorage* GetShopStorage();

	void QueryItems(const UShopAsset* Asset, const FGuid& CollectionId, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&)> Callback);

	const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FInstancedStruct& Context) const;
	const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FGuid& CollectionId) const;

protected:

	TWeakInterfacePtr<IStorageProviderInterface> StorageProvider;

	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UShopSubsystem* Get(UWorld* World);
	static UShopSubsystem* Get(UGameInstance* GameInstance);


	static FGuid GetStorageId();
	static FString GetStorageUrl();
	static TSubclassOf<UStorage> GetStorageClass();

};

