// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEntity/Public/Widget/AvatarUI.h"

// Generated Headers
#include "AvatarViewUI.generated.h"

// Forward Declarations
class UButton;

class UAvatarCollectionUI;
class UAvatarDetailUI;



/**
 *
 */
UCLASS(Abstract)
class UAvatarViewUI : public UAvatarUI
{

	GENERATED_BODY()

public:

	// ~ UAvatarViewUI
	virtual void InitializeDetails(const UAvatarEntry* Entry);
	// ~ End of UAvatarViewUI

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAvatarCollectionUI> EntryCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UAvatarDetailUI> EntryDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;


	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

