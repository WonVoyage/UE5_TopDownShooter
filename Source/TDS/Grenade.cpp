// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Kismet/GameplayStatics.h"

void AGrenade::BeginPlay()
{
	Super::BeginPlay();

}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
}

void AGrenade::TimerExplose(float DeltaTime)
{
	if (TimerEnabled)
		if (TimerToExplose > TimeToExplose)
			Explose();
		else
			TimerToExplose += DeltaTime;
}

void AGrenade::Collision_Hit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::Collision_Hit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AGrenade::Impact_Projectile()
{
	//Init Grenade
	TimerEnabled = true;
}

void AGrenade::Explose()
{
	TimerEnabled = false;
	if (Projectile_Settings.Explose_FX)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Projectile_Settings.Explose_FX, GetActorLocation(), GetActorRotation(), FVector(1.0f));

	if (Projectile_Settings.Explose_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Projectile_Settings.Explose_Sound, GetActorLocation());

	TArray<AActor*> IgnoredActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
		Projectile_Settings.Explose_Max_Damage,
		Projectile_Settings.Explose_Max_Damage*0.2,
		GetActorLocation(),
		1000.0,
		2000.0,
		5,
		NULL, IgnoredActor,nullptr,nullptr);

	this->Destroy();
}
