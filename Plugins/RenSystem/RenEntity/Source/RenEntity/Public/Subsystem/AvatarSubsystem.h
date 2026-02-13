// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "AvatarRecord.h"
#include "TaskDefinition.h"

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

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAvatarUpdated, const FPrimaryAssetId& /* AvatarId */);
	FOnAvatarUpdated OnAvatarUpdated;
	FOnAvatarUpdated OnAvatarAdded;
	FOnAvatarUpdated OnAvatarRemoved;

	const TMap<FPrimaryAssetId, FAvatarData>* GetAvatarCollection() const;
	const FAvatarData* GetAvatarData(const FPrimaryAssetId& AvatarId) const;

	UFUNCTION(BlueprintCallable)
	bool AddAvatar(const FPrimaryAssetId& AvatarId);

	UFUNCTION(BlueprintCallable)
	bool RemoveAvatar(const FPrimaryAssetId& AvatarId);

	bool UpdateAvatar(const FPrimaryAssetId& AvatarId, TFunctionRef<bool(FAvatarData*)> Callback);

protected:

	UPROPERTY()
	TObjectPtr<UAvatarStorage> AvatarStorage;

	TWeakInterfacePtr<IStorageProviderInterface> StorageProviderInterface;


	TMap<FPrimaryAssetId, FAvatarData>* GetMutableAvatarCollection() const;
	FAvatarData* GetMutableAvatarData(const FPrimaryAssetId& AvatarId) const;

	void LoadAvatarStorage();
	void SaveAvatarStorage();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

