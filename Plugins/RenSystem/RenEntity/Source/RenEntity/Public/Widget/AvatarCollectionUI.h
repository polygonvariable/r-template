// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "AvatarCollectionUI.generated.h"

// Forward Declarations
class UListView;

class UPoolSubsystem;
class UAvatarSubsystem;
class UAvatarEntry;

struct FAvatarRecord;



/**
 *
 */
UCLASS(Abstract)
class UAvatarCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAvatarEntry> EntryClass;

	UFUNCTION(BlueprintCallable)
	void DisplayEntries();

	UFUNCTION(BlueprintCallable)
	void ClearEntries();

	UFUNCTION(BlueprintCallable)
	void RefreshEntries();

	UFUNCTION(BlueprintCallable)
	UAvatarEntry* GetSelectedEntry();


	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEntityEntrySelected, const UAvatarEntry* /* Entry */);
	FOnEntityEntrySelected OnEntrySelected;

protected:

	UPROPERTY()
	TWeakObjectPtr<UPoolSubsystem> PoolSubsystem;

	UPROPERTY()
	TWeakObjectPtr<UAvatarSubsystem> AvatarSubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> EntityList = nullptr;


	virtual void HandleSelectedEntry(UObject* Object);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

