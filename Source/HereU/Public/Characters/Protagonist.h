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
	virtual void MoveForward(float AxisValue) override;
	
};

