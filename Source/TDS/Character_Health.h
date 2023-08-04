#pragma once

#include "CoreMinimal.h"
#include "Health.h"
#include "Character_Health.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API UCharacter_Health : public UHealth
{
	GENERATED_BODY()

public:
	// Functions
	virtual void Receive_Damage(float damage);
};
//-------------------------------------------------------------------------------------------------------------
