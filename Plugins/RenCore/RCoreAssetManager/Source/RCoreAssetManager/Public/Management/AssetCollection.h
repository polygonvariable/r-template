// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers

// Generated Headers
#include "AssetCollection.generated.h"

// Module Macros
#define RCORE_API RCOREASSETMANAGER_API

// Forward Declarations
class URPrimaryDataAsset;



/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail
{

	GENERATED_BODY()

public:

	FAssetDetail() {}
	FAssetDetail(int InQuantity) : Quantity(InQuantity) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Quantity = 0;

};



#if WITH_EDITORONLY_DATA

USTRUCT(BlueprintType)
struct FAssetDetail_SimpleEd : public FAssetDetail
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<URPrimaryDataAsset> DataAsset;

};

#endif




/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail_Unique : public FAssetDetail
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGuid ItemId;

};

#if WITH_EDITORONLY_DATA

USTRUCT(BlueprintType)
struct FAssetDetail_UniqueEd : public FAssetDetail_Unique
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<URPrimaryDataAsset> DataAsset;

};

#endif


/**
 *
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetCollection : public UObject
{

	GENERATED_BODY()

public:

	RCORE_API const FGameplayTagContainer& GetCollectionTags() const;

protected:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tags;

};



/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Simple)")
class UAssetCollection_Simple : public UAssetCollection
{

	GENERATED_BODY()

public:

	RCORE_API virtual const TMap<FPrimaryAssetId, int>& GetAssetList() const;
	RCORE_API virtual bool GetAsset(const FPrimaryAssetId& AssetId, int& OutQuantity) const;
	RCORE_API virtual bool GetAnyAsset(TPair<FPrimaryAssetId, int>& OutPair) const;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_SimpleEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<FPrimaryAssetId, int> AssetList;

};


/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Unique Referenced)")
class UAssetCollection_UniqueReferenced : public UAssetCollection
{

	GENERATED_BODY()

public:

	RCORE_API virtual const TMap<URPrimaryDataAsset*, FAssetDetail_Unique>& GetAssetList() const;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_UniqueEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<URPrimaryDataAsset*, FAssetDetail_Unique> AssetList;

	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};

/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Unique)")
class UAssetCollection_Unique : public UAssetCollection
{

	GENERATED_BODY()

public:

	RCORE_API virtual const TMap<FPrimaryAssetId, FAssetDetail_Unique>& GetAssetList() const;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Meta = (DisplayName = "Asset List"))
	TArray<FAssetDetail_UniqueEd> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Asset List (Debug)"))
	TMap<FPrimaryAssetId, FAssetDetail_Unique> AssetList;

	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};



// Module Macros
#undef RCORE_API

