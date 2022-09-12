// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HereUPlayerController_Base.generated.h"

/**
 * 
 */
UCLASS()
class HEREU_API AHereUPlayerController_Base : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	virtual void MoveForward(float AxisValue);
	virtual void MoveRight(float AxisValue);
};



