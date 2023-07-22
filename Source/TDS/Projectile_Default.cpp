#include "Projectile_Default.h"

//-------------------------------------------------------------------------------------------------------------
AProjectile_Default::AProjectile_Default()
{
	PrimaryActorTick.bCanEverTick = true;

	Bullet_Collision_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	Bullet_Collision_Sphere->SetSphereRadius(16.0);
	Bullet_Collision_Sphere->OnComponentHit.AddDynamic(this, &AProjectile_Default::Collision_Hit);
	Bullet_Collision_Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Default::Collision_Begin_Overlap);
	Bullet_Collision_Sphere->OnComponentEndOverlap.AddDynamic(this, &AProjectile_Default::Collision_End_Overlap);
	Bullet_Collision_Sphere->bReturnMaterialOnMove = true;
	Bullet_Collision_Sphere->SetCanEverAffectNavigation(false);

	RootComponent = Bullet_Collision_Sphere;

	Bullet_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_Projectile_Mesh"));
	Bullet_Mesh->SetupAttachment(RootComponent);
	Bullet_Mesh->SetCanEverAffectNavigation(false);

	Bullet_FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet_FX"));
	Bullet_FX->SetupAttachment(RootComponent);

	Bullet_Projectile_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet_Movement"));
	Bullet_Projectile_Movement->UpdatedComponent = RootComponent;
	Bullet_Projectile_Movement->InitialSpeed = 1.0;
	Bullet_Projectile_Movement->MaxSpeed = 0;
	Bullet_Projectile_Movement->bRotationFollowsVelocity = true;
	Bullet_Projectile_Movement->bShouldBounce = true;
}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Collision_Hit(class UPrimitiveComponent *hit_component, AActor *other_actor, UPrimitiveComponent *other_component, FVector normal_impulse, const FHitResult &hit)
{

}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Collision_Begin_Overlap(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_component, int32 other_body_index, bool from_sweep, const FHitResult &sweep_result)
{

}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Collision_End_Overlap(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_component, int32 other_body_index)
{

}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::BeginPlay()
{
	Super::BeginPlay();
}
//-------------------------------------------------------------------------------------------------------------
