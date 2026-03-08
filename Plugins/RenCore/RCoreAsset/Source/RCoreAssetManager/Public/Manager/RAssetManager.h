// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Definition/LatentResult.h"

// Generated Headers
#include "RAssetManager.generated.h"

// Module Macros
#define RCORE_API RCOREASSETMANAGER_API

// Forward Declarations
struct FLatentHandle;



/**
 *
 */
UCLASS(MinimalAPI, Config = Game)
class URAssetManager : public UAssetManager
{

	GENERATED_BODY()

public:

	RCORE_API void CancelFetch(const FGuid& LatentId);

	RCORE_API TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FSoftClassPath& Path, UClass* Type);
	RCORE_API TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const TArray<FSoftClassPath>& Paths, UClass* Type);
	RCORE_API TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path, UClass* Type);
	RCORE_API TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths, UClass* Type);

	RCORE_API TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FSoftObjectPath& Path, UClass* Type);
	RCORE_API TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const TArray<FSoftObjectPath>& Paths, UClass* Type);
	RCORE_API TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftObjectPath& Path, UClass* Type);
	RCORE_API TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths, UClass* Type);


	template<typename T>
	TFuture<FLatentLoadedAsset<T>> FetchPrimaryAsset(const FPrimaryAssetId& AssetId);

	template<typename T>
	TFuture<FLatentLoadedAssets<T>> FetchPrimaryAssets(const TArray<FPrimaryAssetId>& AssetIds);

	template<typename T>
	TFuture<FLatentLoadedAsset<T>> FetchPrimaryAsset(const FGuid& LatentId, const FPrimaryAssetId& AssetId);

	template<typename T>
	TFuture<FLatentLoadedAssets<T>> FetchPrimaryAssets(const FGuid& LatentId, const TArray<FPrimaryAssetId>& AssetIds);


	template<typename T>
	TFuture<FLatentLoadedAsset<T>> FetchSecondaryAsset(const FSoftObjectPath& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<T>> FetchSecondaryAssets(const TArray<FSoftObjectPath>& Paths);

	template<typename T>
	TFuture<FLatentLoadedAsset<T>> FetchSecondaryAsset(const FGuid& LatentId, const FSoftObjectPath& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<T>> FetchSecondaryAssets(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths);
	

	template<typename T>
	TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FSoftClassPath& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const TArray<FSoftClassPath>& Paths);

	template<typename T>
	TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths);


	template<typename T>
	TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const TSoftClassPtr<T>& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const TArray<TSoftClassPtr<T>>& Paths);

	template<typename T>
	TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const TSoftClassPtr<T>& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<TSoftClassPtr<T>>& Paths);

protected:

	TMap<FGuid, TSharedPtr<FLatentHandle>> LatentHandles;
	FCriticalSection LatentHandleLock;

	TWeakPtr<FLatentHandle> CreateHandle(const FGuid& LatentId);
	void RemoveHandle(const FGuid& LatentId);

};



// Module Macros
#undef RCORE_API

