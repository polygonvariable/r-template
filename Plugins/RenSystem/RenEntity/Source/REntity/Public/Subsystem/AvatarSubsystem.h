// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/Runtime/AvatarData.h"
#include "Definition/TaskType.h"
#include "Interface/IAssetInstance.h"

// Generated Headers
#include "AvatarSubsystem.generated.h"

// Module Macros
#define RSYSTEM_API RENTITY_API

// Forward Declarations
class IStorageProvider;
class UAvatarStorage;
class UAvatarAsset;
class URAssetManager;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UAvatarSubsystem : public UGameInstanceSubsystem, public IAssetInstanceCollectionProvider
{

	GENERATED_BODY()

public:

	RSYSTEM_API UAvatarStorage* GetAvatarCollection(const FName& CollectionId) const;

	// ~ IAssetInstanceCollectionProvider
	RSYSTEM_API virtual IAssetInstanceCollection* GetInstanceCollection(const FName& CollectionId) const override;
	RSYSTEM_API virtual FPrimaryAssetType GetSupportedAssetType() const override;
	RSYSTEM_API virtual FName GetDefaultCollectionId() const override;
	// ~ End of IAssetInstanceCollectionProvider

protected:

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static RSYSTEM_API UAvatarSubsystem* Get(UWorld* World);
	static RSYSTEM_API UAvatarSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef RSYSTEM_API

