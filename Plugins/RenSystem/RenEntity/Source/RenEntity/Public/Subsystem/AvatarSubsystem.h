// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "AvatarRecord.h"

// Generated Headers
#include "AvatarSubsystem.generated.h"

// Forward Declarations
class IStorageProviderInterface;
class UAvatarStorage;
class UAvatarAsset;
class URAssetManager;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UAvatarSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAvatarUpdated, FPrimaryAssetId /* AvatarId */);
	FOnAvatarUpdated OnAvatarUpdated;

	const TMap<FPrimaryAssetId, FAvatarRecord>* GetAvatarRecords() const;
	const FAvatarRecord* GetAvatarRecord(const FPrimaryAssetId& AvatarId) const;


	UFUNCTION(BlueprintCallable)
	bool AddAvatar(const FPrimaryAssetId& AvatarId);

	UFUNCTION(BlueprintCallable)
	bool RemoveAvatar(const FPrimaryAssetId& AvatarId);

	UFUNCTION(BlueprintCallable)
	bool UpdateAvatar(const FPrimaryAssetId& AvatarId, const FAvatarRecord& AvatarRecord);

	bool UpdateAvatar(const FPrimaryAssetId& AvatarId, TFunctionRef<bool(FAvatarRecord*)> Callback);

protected:

	UPROPERTY()
	TObjectPtr<UAvatarStorage> AvatarStorage;

	TWeakInterfacePtr<IStorageProviderInterface> StorageProviderInterface;


	TMap<FPrimaryAssetId, FAvatarRecord>* GetMutableAvatarRecords() const;
	FAvatarRecord* GetMutableAvatarRecord(const FPrimaryAssetId& AvatarId) const;

	void LoadAvatarStorage();
	void SaveAvatarStorage();


	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};


struct FLatentTaskDelegate
{

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLatentTaskStarted, FGuid);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLatentTaskFinished, FGuid, bool /* bSuccess */, const FString&);

	FOnLatentTaskStarted OnStarted;
	FOnLatentTaskFinished OnFinished;
	
	void ClearAll()
	{
		OnStarted.Clear();
		OnFinished.Clear();
	}

};


UCLASS(MinimalAPI)
class ULatentTask : public UObject
{

	GENERATED_BODY()

public:

	FLatentTaskDelegate LatentDelegate;
	FGuid LatentId;

	virtual void StartTask();
	virtual void StopTask();

protected:


	void Succeed();
	void Fail(const FString& Reason);

	virtual void Cleanup();

};

UCLASS(MinimalAPI)
class UAddExperienceTask : public ULatentTask
{

	GENERATED_BODY()

public:

	virtual void StartTask() override;
	virtual void StopTask() override;

	FPrimaryAssetId AvatarId;
	FPrimaryAssetId ItemId;

protected:

	UPROPERTY()
	UAvatarAsset* AvatarAsset = nullptr;

	UPROPERTY()
	UPrimaryDataAsset* ItemAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarSubsystem> AvatarSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	void Step_ValidatePlayer();
	void Step_LoadCharacter();
	void Step_CheckRules();
	void Step_RemoveItem();
	void Step_ApplyLevelUp();

};








/**
 *
 *
 *
 */
UCLASS(MinimalAPI)
class UTaskSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	virtual ULatentTask* CreateTask(TSubclassOf<ULatentTask> TaskClass, const FGuid& LatentId);

	template<class T>
	T* CreateTask(const FGuid& LatentId)
	{
		return Cast<T>(CreateTask(T::StaticClass(), LatentId));
	}

	virtual void StartTask(const FGuid& LatentId);
	virtual void StopTask(const FGuid& LatentId);

protected:

	UPROPERTY()
	TMap<FGuid, TObjectPtr<ULatentTask>> LatentTasks;

	virtual void HandleOnTaskStarted(FGuid LatentId);
	virtual void HandleOnTaskFinished(FGuid LatentId, bool bSuccess, const FString& Reason);

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};




















USTRUCT()
struct FTaskDelegate
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTaskFinished, FGuid /* LatentId */, bool /* bSuccess */, const FString& /* Reason */);
	FOnTaskFinished OnTaskFinished;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTaskStarted, FGuid /* LatentId */);
	FOnTaskStarted OnTaskStarted;

};




/**
 *
 *
 *
 */
UCLASS(MinimalAPI)
class UAvatarEnhanceSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	ULatentTask* AddExperiencePoints(const FGuid& LatentId, const FPrimaryAssetId& AvatarId, const FPrimaryAssetId& ItemId);
	ULatentTask* AddRankPoints(const FGuid& LatentId, const FPrimaryAssetId& AvatarId);

protected:

	UPROPERTY()
	UTaskSubsystem* TaskSubsystem = nullptr;

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

