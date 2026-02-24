// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Filter/FilterParentCriterion.h"

// Engine Headers

// Project Headers
#include "Definition/FilterContext.h"



bool UFilterGroupCriterion::Evaluate(const FFilterContext& Context) const
{
	if (Criteria.Num() == 0)
	{
		return true;
	}

	if (Operator == EFilterOperator::And)
	{
		for (const UFilterCriterion* Criterion : Criteria)
		{
			if (Criterion && !Criterion->Evaluate(Context))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (const UFilterCriterion* Criterion : Criteria)
		{
			if (Criterion && Criterion->Evaluate(Context))
			{
				return true;
			}
		}
		return false;
	}
}

bool UFilterNotCriterion::Evaluate(const FFilterContext& Context) const
{
	if (Negate)
	{
		return !Negate->Evaluate(Context);
	}
	return true;
}

