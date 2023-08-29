#include "Projectile_Default.h"

//-------------------------------------------------------------------------------------------------------------
AProjectile_Default::AProjectile_Default()
{
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;

	Bullet_Collision_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	Bullet_Collision_Sphere->SetSphereRadius(4.0);
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
	Bullet_Projectile_Movement->InitialSpeed = 2500.0;
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
void AProjectile_Default::Init(FProjectile_Info init_param)
{
	Bullet_Projectile_Movement->InitialSpeed = init_param.Init_Speed;
	Bullet_Projectile_Movement->MaxSpeed = init_param.Init_Speed;
	SetLifeSpan(init_param.Life_Time);

	if (init_param.Mesh)
		Init_Visible_Projectile_Multicast(init_param.Mesh);
	else
		Bullet_Mesh->DestroyComponent();

	Projectile_Settings = init_param;
}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Impact_Projectile()
{
	Destroy();
}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Collision_Hit(class UPrimitiveComponent *hit_component, AActor *other_actor, UPrimitiveComponent *other_component, FVector normal_impulse, const FHitResult &hit)
{
	if (other_actor && hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface mySurfacetype = UGameplayStatics::GetSurfaceType(hit);

		if (Projectile_Settings.Hit_Decal.Contains(mySurfacetype))
		{
			UMaterialInterface* myMaterial = Projectile_Settings.Hit_Decal[mySurfacetype];

			if (myMaterial && other_component)
				UGameplayStatics::SpawnDecalAttached(myMaterial, FVector(20.0f), other_component, NAME_None, hit.ImpactPoint, hit.ImpactNormal.Rotation(),EAttachLocation::KeepWorldPosition,10.0f);
		}

		if (Projectile_Settings.Hit_FXs.Contains(mySurfacetype))
			if (UParticleSystem* myParticle = Projectile_Settings.Hit_FXs[mySurfacetype])
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myParticle, FTransform(hit.ImpactNormal.Rotation(), hit.ImpactPoint, FVector(1.0f)));

		if (Projectile_Settings.Hit_Sound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Projectile_Settings.Hit_Sound, hit.ImpactPoint);
	}

	UGameplayStatics::ApplyDamage(other_actor, Projectile_Settings.Damage, GetInstigatorController(), this, NULL);
	UAISense_Damage::ReportDamageEvent(GetWorld(), hit.GetActor(), GetInstigator(), Projectile_Settings.Damage, hit.Location, hit.Location);
	Impact_Projectile();	
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

	Bullet_Collision_Sphere->OnComponentHit.AddDynamic(this, &AProjectile_Default::Collision_Hit);
	Bullet_Collision_Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Default::Collision_Begin_Overlap);
	Bullet_Collision_Sphere->OnComponentEndOverlap.AddDynamic(this, &AProjectile_Default::Collision_End_Overlap);
}
//-------------------------------------------------------------------------------------------------------------
void AProjectile_Default::Init_Visible_Projectile_Multicast_Implementation(UStaticMesh *mesh)
{
	Bullet_Mesh->SetStaticMesh(mesh);
}
//-------------------------------------------------------------------------------------------------------------
