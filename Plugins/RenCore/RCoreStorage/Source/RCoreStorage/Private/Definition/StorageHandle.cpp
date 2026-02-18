// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Definition/StorageHandle.h"

// Engine Headers

// Project Headers
#include "SaveGame/Storage.h"



bool FStorageHandle::IsValid() const
{
	return StorageClass != nullptr && StorageId.IsValid() && !Url.IsEmpty();
}

