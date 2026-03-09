// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/CraftQuery.h"
#include "Definition/TaskType.h"

// Generated Headers
#include "CraftSubsystem.generated.h"

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
UCLASS(NotBlueprintType)
class UCraftSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void ClaimCraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);
	void CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);



	UCraftStorage* GetCraftStorage();

	void QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)> Callback);

	const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FInstancedStruct& Context) const;
	const UAssetCollection* GetMaterialCollection(const URPrimaryDataAsset* Asset, const FGuid& CollectionId) const;

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

	static UCraftSubsystem* Get(UWorld* World);
	static UCraftSubsystem* Get(UGameInstance* GameInstance);

	static FGuid GetStorageId();
	static FString GetStorageUrl();
	static TSubclassOf<UStorage> GetStorageClass();

};

