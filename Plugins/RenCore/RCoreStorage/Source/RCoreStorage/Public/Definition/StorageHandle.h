// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers


// Project Headers
#include "TaskDefinition.h"

// Generated Headers

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

    RCORESTORAGE_API bool IsValid() const;

};

