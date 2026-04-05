// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CanBindHealthUI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanBindHealthUI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIHACKNSLASH_API ICanBindHealthUI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BindHealthBar(UObject* inObj, const FName& inFunc) = 0;
	virtual void SetHealthPercentage(float Percentage) = 0;
};
