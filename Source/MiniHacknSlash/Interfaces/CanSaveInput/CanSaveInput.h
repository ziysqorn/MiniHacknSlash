// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CanSaveInput.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCanSaveInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIHACKNSLASH_API ICanSaveInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetSavedInputBuffer(TArray<FString>& OutBuffer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearSavedInputBuffer();
};
