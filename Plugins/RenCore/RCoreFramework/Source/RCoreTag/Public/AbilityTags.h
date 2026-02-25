// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// Module Macros
#define RCORE_API RCORETAG_API


// Gameplay Tags
// ~ Deprecated
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack01_End);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack02_End);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack03_End);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack04_End);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack05_End);

RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_State_Ready);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_State_Aim);

RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_01);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_02);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_03);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_04);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_05);


RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_01);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_02);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_03);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_04);
RCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_05);
// ~ End of Deprecated


// ~ Equipment Ability
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Uses_Equipment_Slot_01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Uses_Equipment_Slot_02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Uses_Equipment_Slot_03);
// ~ End of Equipment Ability

// ~ Equipment Event
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Equipment_Slot_01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Equipment_Slot_02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Equipment_Slot_03);
// ~ End of Equipment Event

// ~ Equipment State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Slot_01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Slot_02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Slot_03);
// ~ End of Equipment State


// ~ Camera
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Aim);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Camera_Aim);
// ~ End of Camera



// Module Macros
#undef RCORE_API

