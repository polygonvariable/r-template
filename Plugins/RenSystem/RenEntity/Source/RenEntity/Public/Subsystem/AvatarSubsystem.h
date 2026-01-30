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
	TWeakObjectPtr<UAvatarStorage> AvatarStorage;

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

