// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/CraftQuery.h"
#include "Definition/TaskType.h"

// Generated Headers
#include "CraftSubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RCRAFT_API

// Forward Declarations
class UAssetCollection;
class UStorage;
class IStorageProviderInterface;
class UCraftStorage;
class UTradeAsset;
class URPrimaryDataAsset;

struct FAssetDetail_Trade;
struct FInstancedStruct;
struct FCraftData;



/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UCraftSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	RSYSTEM_API UCraftStorage* GetCraftStorage();

	RSYSTEM_API void ClaimCraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);
	RSYSTEM_API void CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);

	RSYSTEM_API const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FInstancedStruct& Context) const;
	RSYSTEM_API const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FGuid& CollectionId) const;

	RSYSTEM_API void QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)> Callback);

protected:

	TWeakInterfacePtr<IStorageProviderInterface> StorageProvider;


	void HandleGlossaryItems(const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback);
	void HandleStorageItems(const TMap<URPrimaryDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback);

	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	RSYSTEM_API static UCraftSubsystem* Get(UWorld* World);
	RSYSTEM_API static UCraftSubsystem* Get(UGameInstance* GameInstance);

	static FGuid GetStorageId();
	static FString GetStorageUrl();
	static TSubclassOf<UStorage> GetStorageClass();

};



// Module Macros
#undef RSYSTEM_API

