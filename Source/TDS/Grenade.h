#pragma once

#include "CoreMinimal.h"
#include "Projectile_Default.h"
#include "Grenade.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API AGrenade : public AProjectile_Default
{
	GENERATED_BODY()

public:
	// Functions
	AGrenade();

	virtual void Tick(float delta_time);
	virtual void Collision_Hit(class UPrimitiveComponent *hit_component, AActor *other_actor, UPrimitiveComponent *other_component, FVector normal_impulse, const FHitResult &hit);
	virtual void Impact_Projectile();

	void Timer_Explose(float delta_time);
	void Explose();

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade") float Timer_To_Explose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade") float Time_To_Explose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade") bool Timer_Enabled;

protected:
	// Functions
	virtual void BeginPlay() override;


};
//-------------------------------------------------------------------------------------------------------------
