#include "Weapon_Default.h"

//-------------------------------------------------------------------------------------------------------------
AWeapon_Default::AWeapon_Default()
:  Shoot_End_Location(FVector(ForceInitToZero)), Scene_Component(0), Skeletal_Mesh_Weapon(0), Static_Mesh_Weapon(0), Shoot_Location(0), Reload_Timer(0.0),Reloading(false), Fire_Timer(0.0), Weapon_Fire(false)
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
void AWeapon_Default::Tick(float delta_time)
{
	Super::Tick(delta_time);
	Reload_Tick(delta_time);
	Fire_Tick(delta_time);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Reload_Tick(float delta_time)
{
	if (!Reloading)
		return;

	if (Reload_Timer < 0.0)
		Finish_Reload();
	else
		Reload_Timer -= delta_time;
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Fire_Tick(float delta_time)
{
	if (!Weapon_Fire)
		return;

	if (Get_Weapon_Round())
	{
		if (Fire_Timer <= 0.0)
		{
			if (!Reloading)
				Fire();
		}
		else
			Fire_Timer -= delta_time;
	}
	else
		if (!Reloading)
			Init_Reload();
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Fire()
{
	if (!Shoot_Location || !Weapon_Settings.Projectile_Settings.Projectile)
		throw 23;
	
	FVector spawn_location;
	FRotator spawn_rotation;
	FActorSpawnParameters spawn_params;
	int i;
	int projectile_count;

	spawn_location = Shoot_Location->GetComponentLocation();
	spawn_rotation = Shoot_Location->GetComponentRotation();
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawn_params.Owner = GetOwner();
	spawn_params.Instigator = GetInstigator();
	projectile_count = Get_Number_Projectile_By_Shot();
	Fire_Timer = Weapon_Settings.Rate_Of_Fire;

	for (i = 0; i < projectile_count; i++)
		if(AProjectile_Default *projectile = Cast<AProjectile_Default>(GetWorld()->SpawnActor(Weapon_Settings.Projectile_Settings.Projectile, &spawn_location, &spawn_rotation, spawn_params)))
			projectile->Init(Weapon_Settings.Projectile_Settings);

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Weapon_Settings.Sound_Fire, Shoot_Location->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon_Settings.Effect_Fire, Shoot_Location->GetComponentLocation());

	Weapon_Info.Round--;
	On_Weapon_Fire.Broadcast();
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::BeginPlay()
{
	Super::BeginPlay();

	Init_Weapon();
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Init_Weapon()
{
	if (Skeletal_Mesh_Weapon && !Skeletal_Mesh_Weapon->GetSkeletalMeshAsset())
		Skeletal_Mesh_Weapon->DestroyComponent(true);

	if (Static_Mesh_Weapon && Static_Mesh_Weapon->GetStaticMesh())
		Static_Mesh_Weapon->DestroyComponent(true);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Init_Reload()
{
	Reloading = true;
	Reload_Timer = Weapon_Settings.Reload_Time;

	// Add Animation
	if(Weapon_Settings.Animation_Character_Reload)
		On_Weapon_Reload_Start.Broadcast(Weapon_Settings.Animation_Character_Reload);
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

}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Finish_Reload()
{
	int ammo_need_take;

	Reloading = false;
	ammo_need_take = Weapon_Info.Round - Weapon_Settings.Max_Round;
	Weapon_Info.Round = Weapon_Settings.Max_Round;

	On_Weapon_Reload_End.Broadcast(true, ammo_need_take);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Cancel_Reload()
{
	Reloading = false;

	if (Skeletal_Mesh_Weapon && Skeletal_Mesh_Weapon->GetAnimInstance())
		Skeletal_Mesh_Weapon->GetAnimInstance()->StopAllMontages(0.15);

	On_Weapon_Reload_End.Broadcast(false, 0);
}
//-------------------------------------------------------------------------------------------------------------
bool AWeapon_Default::Can_Weapon_Reload()
{
	bool result;

	result = true;

	if (!GetOwner())
		result = false;

	UInventory *inventory = Cast<UInventory>(GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		result = false;

	if (!inventory->Check_Ammo_For_Weapon(inventory->GetWeaponIndexSlotByName(Curr_Weapon_Name)))
		result = false;

	return result;
}
//-------------------------------------------------------------------------------------------------------------
bool AWeapon_Default::Weapon_Can_Fire()
{
	return true;
}
//-------------------------------------------------------------------------------------------------------------
int AWeapon_Default::Get_Weapon_Round()
{
	return Weapon_Info.Round;
}
//-------------------------------------------------------------------------------------------------------------
int AWeapon_Default::Get_Number_Projectile_By_Shot()
{
	return Weapon_Settings.Projectile_By_Shot;
}
//-------------------------------------------------------------------------------------------------------------
