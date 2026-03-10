// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers


// Project Headers
#include "Definition/TaskType.h"

// Generated Headers

// Module Macros
#define RCORE_API RCORESTORAGE_API

// Forward Declarations
class UStorage;



/**
 *
 * 
 *
 */
struct FStorageHandle
{

public:

    TSubclassOf<UStorage> StorageClass;
    FGuid StorageId;
    FString Url;
    FTaskCallback Callback;

    RCORE_API bool IsValid() const;
    RCORE_API bool IsUrlValid() const;

};



// Module Macros
#undef RCORE_API

