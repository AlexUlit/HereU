// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "MyInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HEREU_API IMyInputInterface
{
	GENERATED_BODY()

public:
	virtual void InputMoveX(float AxisValue){}
	virtual void InputMoveY(float AxisValue){}
	virtual void InputMouseX(float AxisValue){}
	virtual void InputMouseY(float AxisValue){}
	virtual void InputSpacePress(){}
};
