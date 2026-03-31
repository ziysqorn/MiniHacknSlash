// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "CanLockTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanLockTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIHACKNSLASH_API ICanLockTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void LockTarget() = 0;
	virtual TWeakObjectPtr<AActor> GetLockedOnTarget() = 0;
};
