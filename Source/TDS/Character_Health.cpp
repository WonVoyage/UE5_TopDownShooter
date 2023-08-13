#include "Character_Health.h"

UCharacter_Health::UCharacter_Health()
: Coef_Damage(1.0), Shield(100.0), Cool_Down_Shield_Time(5.0), Shield_Recovery_Value(1.0), Shield_Recovery_Rate(0.1)
{
}
//-------------------------------------------------------------------------------------------------------------
void UCharacter_Health::Change_Health(float value)
{
	value *= Coef_Damage;

	if (Shield > 0.0 && value > 0.0)
		Change_Shield(value);
	else
		Super::Change_Health(value);
}
//-------------------------------------------------------------------------------------------------------------
void UCharacter_Health::Change_Shield(float value)
{
	Shield -= value;

	On_Shield_Change.Broadcast(Shield, value);

	if (Shield > 100.0)
		Shield = 100.0;
	else
	{
		if (Shield < 0.0)
			Shield = 0.0;
	}

	if (!GetWorld())
		throw 23;

	GetWorld()->GetTimerManager().SetTimer(Cool_Down_Shield_Timer, this, &UCharacter_Health::Cool_Down_Shield_End, Cool_Down_Shield_Time, false);
	GetWorld()->GetTimerManager().ClearTimer(Shield_Recovery_Rate_Timer);
}
//-------------------------------------------------------------------------------------------------------------
void UCharacter_Health::Cool_Down_Shield_End()
{
	if (!GetWorld())
		throw 23;

	GetWorld()->GetTimerManager().SetTimer(Shield_Recovery_Rate_Timer, this, &UCharacter_Health::Recovery_Shield, Shield_Recovery_Rate, true);
}
//-------------------------------------------------------------------------------------------------------------
void UCharacter_Health::Recovery_Shield()
{
	if (!GetWorld())
		throw 23;

	Shield += Shield_Recovery_Value;

	if (Shield > 100.0)
	{
		Shield = 100.0;

		GetWorld()->GetTimerManager().ClearTimer(Shield_Recovery_Rate_Timer);
	}

	On_Shield_Change.Broadcast(Shield, Shield_Recovery_Value);
}
//-------------------------------------------------------------------------------------------------------------
float UCharacter_Health::Get_Shield()
{
	return Shield;
}
//-------------------------------------------------------------------------------------------------------------
