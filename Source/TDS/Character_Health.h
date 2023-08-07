#pragma once

#include "CoreMinimal.h"
#include "Health.h"
#include "Character_Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Shield_Change, float, shield, float, damage);
//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API UCharacter_Health : public UHealth
{
	GENERATED_BODY()

public:
	// Functions
	UCharacter_Health();
	virtual void Change_Health(float value);

	void Change_Shield(float value);
	void Cool_Down_Shield_End();
	void Recovery_Shield();
	float Get_Shield();

	// Variables
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health") FOn_Shield_Change On_Shield_Change;
	FTimerHandle Cool_Down_Shield_Timer;
	FTimerHandle Shield_Recovery_Rate_Timer;
	float Coef_Damage;
	float Shield;
	float Cool_Down_Shield_Time;
	float Shield_Recovery_Value;
	float Shield_Recovery_Rate;
};
//-------------------------------------------------------------------------------------------------------------
