// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HereUPlayerController_Base.h"
#include "ControllerProtagonist.generated.h"

/**
 * 
 */
UCLASS()
class HEREU_API AControllerProtagonist : public AHereUPlayerController_Base
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void MoveForward(float AxisValue) override;
	virtual void MoveRight(float AxisValue) override;
};
