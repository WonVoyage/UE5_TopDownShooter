#include "Grenade.h"
#include "Kismet/GameplayStatics.h"

//-------------------------------------------------------------------------------------------------------------
AGrenade::AGrenade()
: Timer_To_Explose(0.0), Time_To_Explose(5.0), Timer_Enabled(false)
{
}
//-------------------------------------------------------------------------------------------------------------
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}
//-------------------------------------------------------------------------------------------------------------
void AGrenade::Tick(float delta_time)
{
	Super::Tick(delta_time);
	Timer_Explose(delta_time);
}
//-------------------------------------------------------------------------------------------------------------
void AGrenade::Timer_Explose(float delta_time)
{
	if (!Timer_Enabled)
		return;

	if (Timer_To_Explose > Time_To_Explose)
		Explose();
	else
		Timer_To_Explose += delta_time;
}
//-------------------------------------------------------------------------------------------------------------
void AGrenade::Collision_Hit(class UPrimitiveComponent *hit_component, AActor *other_actor, UPrimitiveComponent *other_component, FVector normal_impulse, const FHitResult &hit)
{
	Super::Collision_Hit(hit_component, other_actor, other_component, normal_impulse, hit);
}
//-------------------------------------------------------------------------------------------------------------
void AGrenade::Impact_Projectile()
{
	Timer_Enabled = true;
}
//-------------------------------------------------------------------------------------------------------------
void AGrenade::Explose()
{
	TArray<AActor*> ignored_actor;

	Timer_Enabled = false;

	if (Projectile_Settings.Explose_FX)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Projectile_Settings.Explose_FX, GetActorLocation(), GetActorRotation(), FVector(1.0f));

	if (Projectile_Settings.Explose_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Projectile_Settings.Explose_Sound, GetActorLocation());

	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Projectile_Settings.Explose_Max_Damage, Projectile_Settings.Explose_Max_Damage * 0.2, GetActorLocation(), 1000.0, 2000.0, 5, NULL, ignored_actor, 0, 0);

	Destroy();
}
//-------------------------------------------------------------------------------------------------------------
