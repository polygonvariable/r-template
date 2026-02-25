// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "FilterGroup.generated.h"

// Module Macros
#define RCORE_API RCOREFILTER_API

// Forward Declarations
class UFilterCriterion;



/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UFilterGroup : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterCriterion> CriterionRoot;


	RCORE_API UFilterCriterion* GetCriterionByName(FName PropertyName) const;

	template<typename T>
	T* GetCriterionByName(FName PropertyName) const
	{
		return Cast<T>(GetCriterionByName(PropertyName));
	}

protected:

	UFilterCriterion* FindCriterionByName(UFilterCriterion* Criterion, const FName& PropertyName) const;

};



// Module Macros
#undef RCORE_API

