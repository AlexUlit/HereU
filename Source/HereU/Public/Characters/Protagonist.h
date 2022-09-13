#pragma once

#include "CoreMinimal.h"
#include "Characters/HereUCharacter_Base.h"
#include "Interfaces/MyInputInterface.h"
#include "Protagonist.generated.h"


UCLASS()
class HEREU_API AProtagonist : public AHereUCharacter_Base, public IMyInputInterface
{
	GENERATED_BODY()

public:
	virtual void InputMoveX(float AxisValue) override;
	virtual void InputMoveY(float AxisValue) override;
	virtual void InputMouseX(float AxisValue) override;
	virtual void InputMouseY(float AxisValue) override;
	virtual void InputSpacePress() override;
};

