// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HereUPlayerController.generated.h"

class IMyInputInterface;
UCLASS()
class HEREU_API AHereUPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	
	void MyInputMoveX(float AxisValue);
	void MyInputMoveY(float AxisValue);
	void MyInputMouseX(float AxisValue);
	void MyInputMouseY(float AxisValue);

	void MyInputSprintPress(){};
	void MyInputSprintReleased(){};

	void MyInputCrouchPress(){};
	void MyInputCrouchReleased(){};

	void MyInputLMBPress();
	void MyInputLMBReleased();

	void MyInputRMBPress();
	void MyInputRMBReleased();

	void MyInputSpacePress();
	void MyInputSpaceReleased();

	bool bCanUseMovement = true;
	bool bCanUseMouse = true;
	
	IMyInputInterface* InputInterface;

	//InputComponent->GetAxisValue("A")
};







