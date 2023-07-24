#include "Weapon_Default.h"

//-------------------------------------------------------------------------------------------------------------
AWeapon_Default::AWeapon_Default()
:  Fire_Time(0), Scene_Component(0), Skeletal_Mesh_Weapon(0), Static_Mesh_Weapon(0), Shoot_Location(0), Weapon_Fire(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene_Component = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Component"));
	RootComponent = Scene_Component;

	Skeletal_Mesh_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal_Mesh"));
	Skeletal_Mesh_Weapon->SetGenerateOverlapEvents(false);
	Skeletal_Mesh_Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	Skeletal_Mesh_Weapon->SetupAttachment(RootComponent);

	Static_Mesh_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static_Mesh"));
	Static_Mesh_Weapon->SetGenerateOverlapEvents(false);
	Static_Mesh_Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	Static_Mesh_Weapon->SetupAttachment(RootComponent);

	Shoot_Location = CreateDefaultSubobject<UArrowComponent>(TEXT("Shoot_Location"));
	Shoot_Location->SetupAttachment(RootComponent);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Fire_Tick(DeltaTime);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Fire_Tick(float delta_time)
{
	if (!Weapon_Fire)
		return;

	if (Fire_Time < 0.0)
		Fire();
	else
		Fire_Time -= delta_time;
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Fire()
{
	if (!Shoot_Location || !Weapon_Settings.Projectile_Settings.Projectile)
		throw 23;
	
	FVector spawn_location;
	FRotator spawn_rotation;
	FActorSpawnParameters spawn_params;

	spawn_location = Shoot_Location->GetComponentLocation();
	spawn_rotation = Shoot_Location->GetComponentRotation();
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawn_params.Owner = GetOwner();
	spawn_params.Instigator = GetInstigator();
	Fire_Time = Weapon_Settings.Rate_Of_Fire;

	if(AProjectile_Default *projectile = Cast<AProjectile_Default>(GetWorld()->SpawnActor(Weapon_Settings.Projectile_Settings.Projectile, &spawn_location, &spawn_rotation, spawn_params)))
		projectile->InitialLifeSpan = 20.0;
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::BeginPlay()
{
	Super::BeginPlay();

	Weapon_Init();
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Weapon_Init()
{
	if (Skeletal_Mesh_Weapon && !Skeletal_Mesh_Weapon->SkeletalMesh)
		Skeletal_Mesh_Weapon->DestroyComponent(true);

	if (Static_Mesh_Weapon && Static_Mesh_Weapon->GetStaticMesh())
		Static_Mesh_Weapon->DestroyComponent(true);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Set_Weapon_State_Fire(bool is_fire)
{
	if (Weapon_Can_Fire())
		Weapon_Fire = is_fire;
	else
		Weapon_Fire = false;
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Update_State_Weapon(EMovement_State movement_state)
{
	Change_Dispersion();
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Change_Dispersion()
{

}
//-------------------------------------------------------------------------------------------------------------
bool AWeapon_Default::Weapon_Can_Fire()
{
	return true;
}
//-------------------------------------------------------------------------------------------------------------
