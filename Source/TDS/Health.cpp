#include "Health.h"

//-------------------------------------------------------------------------------------------------------------
UHealth::UHealth()
: Health(100.0)
{
	PrimaryComponentTick.bCanEverTick = true;
}
//-------------------------------------------------------------------------------------------------------------
void UHealth::BeginPlay()
{
	Super::BeginPlay();
}
//-------------------------------------------------------------------------------------------------------------
void UHealth::TickComponent(float delta_time, ELevelTick tick_type, FActorComponentTickFunction *this_tick_function)
{
	Super::TickComponent(delta_time, tick_type, this_tick_function);
}
//-------------------------------------------------------------------------------------------------------------
void UHealth::Change_Health(float damage)
{
	Health -= damage;
	On_Health_Change.Broadcast(Health, damage);

	if (Health > 100.0)
		Health = 100.0;

	if (Health <= 0.0)
		On_Dead.Broadcast();
}
//-------------------------------------------------------------------------------------------------------------
void UHealth::Set_New_Health(float new_health)
{
	Health = new_health;
}
//-------------------------------------------------------------------------------------------------------------
float UHealth::Get_Current_Health()
{
	return Health;
}
//-------------------------------------------------------------------------------------------------------------
void UHealth::BP_Dead_Event_Implementation()
{
	// In BP
}
//-------------------------------------------------------------------------------------------------------------
