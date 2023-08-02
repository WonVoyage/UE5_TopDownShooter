#include "Weapon_Default.h"

//-------------------------------------------------------------------------------------------------------------
AWeapon_Default::AWeapon_Default()
:  Shoot_End_Location(FVector(ForceInitToZero)), Scene_Component(0), Skeletal_Mesh(0), Static_Mesh(0), Shoot_Location(0), Reload_Timer(0.0),Reloading(false), Fire_Timer(0.0), Is_Fire(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene_Component = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Component"));
	RootComponent = Scene_Component;

	Skeletal_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal_Mesh"));
	Skeletal_Mesh->SetGenerateOverlapEvents(false);
	Skeletal_Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Skeletal_Mesh->SetupAttachment(RootComponent);

	Static_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static_Mesh"));
	Static_Mesh->SetGenerateOverlapEvents(false);
	Static_Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Static_Mesh->SetupAttachment(RootComponent);

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
	if (!Is_Fire)
		return;

	if (Is_Fire && Get_Round() > 0 && !Reloading)
	{
		if (Fire_Timer <= 0.0)
		{
			Fire();
		}
		else
			Fire_Timer -= delta_time;
	}
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Fire()
{
	if (!Shoot_Location || !Settings.Projectile_Settings.Projectile)
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
	Fire_Timer = Settings.Rate_Of_Fire;

	for (i = 0; i < projectile_count; i++)
		if(AProjectile_Default *projectile = Cast<AProjectile_Default>(GetWorld()->SpawnActor(Settings.Projectile_Settings.Projectile, &spawn_location, &spawn_rotation, spawn_params)))
			projectile->Init(Settings.Projectile_Settings);

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Settings.Sound_Fire, Shoot_Location->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Settings.Effect_Fire, Shoot_Location->GetComponentLocation());

	Info.Round--;
	On_Fire.Broadcast();

	if (Get_Round() <= 0 && !Reloading)
		if (Can_Reload())
			Init_Reload();

}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::BeginPlay()
{
	Super::BeginPlay();

	Init();
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Init()
{
	if (Skeletal_Mesh && !Skeletal_Mesh->GetSkeletalMeshAsset())
		Skeletal_Mesh->DestroyComponent(true);

	if (Static_Mesh && Static_Mesh->GetStaticMesh())
		Static_Mesh->DestroyComponent(true);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Init_Reload()
{
	Reloading = true;
	Reload_Timer = Settings.Reload_Time;

	// Add Animation
	if(Settings.Animation_Character_Reload)
		On_Reload_Start.Broadcast(Settings.Animation_Character_Reload);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Set_State_Fire(bool is_fire)
{
	if (Can_Fire())
		Is_Fire = is_fire;
	else
		Is_Fire = false;
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Update_State(EMovement_State movement_state)
{

}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Finish_Reload()
{
	int ammo_need_take;
	int aviable_ammo;
	int need_to_reload;

	Reloading = false;
	aviable_ammo = Get_Aviable_Ammo_For_Reload();

	ammo_need_take = Info.Round - aviable_ammo;
	need_to_reload = Settings.Max_Round - Info.Round;

	if (need_to_reload > aviable_ammo)
	{
		Info.Round = aviable_ammo;
		ammo_need_take = aviable_ammo;
	}
	else
	{
		Info.Round += need_to_reload;
		ammo_need_take = need_to_reload;
	}

	On_Reload_End.Broadcast(true, ammo_need_take);
}
//-------------------------------------------------------------------------------------------------------------
void AWeapon_Default::Cancel_Reload()
{
	Reloading = false;

	if (Skeletal_Mesh && Skeletal_Mesh->GetAnimInstance())
		Skeletal_Mesh->GetAnimInstance()->StopAllMontages(0.15);

	On_Reload_End.Broadcast(false, 0);
}
//-------------------------------------------------------------------------------------------------------------
bool AWeapon_Default::Can_Fire()
{
	return true;
}
//-------------------------------------------------------------------------------------------------------------
bool AWeapon_Default::Can_Reload()
{
	int ammo;

	if (!GetOwner())
		return false;

	UInventory *inventory = Cast<UInventory>(GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		return false;

	if (!inventory->Check_Ammo_For_Weapon(Settings.Type, ammo))
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------------------
int AWeapon_Default::Get_Aviable_Ammo_For_Reload()
{
	int aviable_ammo;
	int result_ammo;

	aviable_ammo = Settings.Max_Round;
	result_ammo = 0;

	if (!GetOwner())
		return aviable_ammo;

	UInventory *inventory = Cast<UInventory>(GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		return aviable_ammo;

	if (!inventory->Check_Ammo_For_Weapon(Settings.Type, aviable_ammo))
		return aviable_ammo;

	result_ammo = aviable_ammo;

	return result_ammo;
}
//-------------------------------------------------------------------------------------------------------------
int AWeapon_Default::Get_Round()
{
	return Info.Round;
}
//-------------------------------------------------------------------------------------------------------------
int AWeapon_Default::Get_Number_Projectile_By_Shot()
{
	return Settings.Projectile_By_Shot;
}
//-------------------------------------------------------------------------------------------------------------
