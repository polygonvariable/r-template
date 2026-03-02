// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Module Macros
#define RCORE_API RCORELIBRARY_API

// Forward Declarations



// ~ Task Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTask, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogStorage, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogAsset, Log, All);
// ~ End of Task Categories

// ~ Library Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTimer, Log, All);
// ~ End of Library Categories

// ~ Priority Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogPriorityList, Log, All);
// ~ End of Priority Categories

// ~ Environment Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogEnvironment, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogWeather, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogSeason, Log, All);
// ~ End of Environment Categories

// ~ Pool Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogPool, Log, All);
// ~ End of Pool Categories

// ~ Counter Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogCounter, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTimestamp, Log, All);
// ~ End of Counter Categories

// ~ Inventory Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTrade, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogCraft, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogShop, Log, All);

RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogInventoryAscension, Log, All);
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogInventoryInstance, Log, All);
// ~ End of Inventory Categories

// ~ Avatar Categories
RCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogAvatar, Log, All);
// ~ End of Avatar Categories




// Module Macros
#undef RCORE_API

