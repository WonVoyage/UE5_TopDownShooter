// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile_Default.h"
#include "Grenade.generated.h"

/**
* 
*/
UCLASS()
class TDS_API AGrenade : public AProjectile_Default
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TimerExplose(float DeltaTime);

	virtual void Collision_Hit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void Impact_Projectile();

	void Explose();

	bool TimerEnabled = false;
	float TimerToExplose = 0.0;
	float TimeToExplose = 5.0;
};
