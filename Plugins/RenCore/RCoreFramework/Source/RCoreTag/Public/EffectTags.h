// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// Module Macros
#define RCORE_API RCORETAG_API



// Gameplay Tags
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Physical);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Magical);

RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Effect_Visible);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Effect_Stackable);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Effect_Duration);



// Module Macros
#undef RCORE_API

