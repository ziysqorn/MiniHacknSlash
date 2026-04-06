// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraAdjustmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIHACKNSLASH_API UCameraAdjustmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraAdjustmentComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | Normal Socket Offset")
	FVector NormalSocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Normal Target Arm Length")
	float NormalTargetArmLength;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | In Combat Socket Offset")
	FVector InCombatSocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | In Combat Target Arm Length")
	float InCombatTargetArmLength;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | In Combat Camera Rotation")
	FRotator InCombatCameraRot;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Camera Transition Speed")
	float CameraTransitionSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | End In Combat Duration")
	float EndInCombatDuration;

	FTimerHandle InCombatTimerHandle;



	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartCombatState();
	
	void EndCombatState();
};
