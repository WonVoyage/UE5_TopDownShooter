#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn_Dead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Health_Change, float, health, float, damage);
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FStats_Param
{
	GENERATED_BODY()
};
//-------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Functions
	UHealth();

	virtual void TickComponent(float delta_time, ELevelTick tick_type, FActorComponentTickFunction *this_tick_function);


	UFUNCTION(BlueprintCallable, Category = "Health") virtual void Receive_Damage(float damage);
	UFUNCTION(BlueprintCallable, Category = "Health") float Get_Curent_Health();
	UFUNCTION(BlueprintNativeEvent) void BP_Dead_Event();
	UFUNCTION() void Dead_Event();

	// Variables
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health") FOn_Dead On_Dead;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health") FOn_Health_Change On_Health_Change;


private:
	// Variables
	float Health;

protected:
	// Functions
	virtual void BeginPlay();

};
//-------------------------------------------------------------------------------------------------------------
