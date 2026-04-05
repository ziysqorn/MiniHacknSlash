// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "../../UI/CustomHUD/CustomHUD.h"
#include "HUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIHACKNSLASH_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHUDComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | HUD Subclass")
	TSubclassOf<UUserWidget> HUDSubclass;

	UPROPERTY()
	TObjectPtr<UCustomHUD> CustomHUD;

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	float GetOwnerHealthPercentage() const;

	UFUNCTION()
	float GetOwnerStunPercentage() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitHUD();
};
