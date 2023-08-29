#include "TDSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

//-------------------------------------------------------------------------------------------------------------
ATDSCharacter::ATDSCharacter()
: Is_Alive(true), Lives(1), Curr_Slot_Index(0), Inventory(0), Sprint_Run_Enabled(false), Aim_Enabled(false), Walk_Enabled(false), Curr_Weapon(0), Movement_State(EMovement_State::Run) 
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate Character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate Character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when Character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory_Component"));
	Health = CreateDefaultSubobject<UCharacter_Health>(TEXT("Health"));

	Inventory->On_Switch_Weapon.AddDynamic(this, &ATDSCharacter::Init);
	Health->On_Dead.AddDynamic(this, &ATDSCharacter::Dead);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bReplicates = true;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
	Fire_Tick(delta_seconds);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Init(Init_Weapon_Name, Weapon_Info, Curr_Slot_Index);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Enable_Ragdoll()
{
	if (!GetMesh())
		throw 23;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Fire_Tick(float delta_seconds)
{
	if (!Curr_Weapon)
		return;

	if (!Curr_Weapon->Is_Fire)
		return;

	if (Curr_Weapon->Reloading)
		return;

	if (!Is_Alive)
		return;

	if (Curr_Weapon->Get_Round() > 0)
	{
		if (Curr_Weapon->Fire_Timer <= 0.0)
			Curr_Weapon->Fire_Server();
		else
			Curr_Weapon->Fire_Timer -= delta_seconds;
	}
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Drop_Weapon(int32 ByIndex, FDrop_Item &DropItemInfo)
{
	FWeapon_Slot emty_weapon_slot;
	int i = 1;

	FVector spawn_location;
	FRotator spawn_rotation;
	FActorSpawnParameters spawn_params;

	spawn_location = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 100.0);
	spawn_rotation = FRotator(ForceInitToZero);
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Inventory->Weapon_Slot.Num() < 1)
		throw 23;

	Inventory->Get_Drop_Item_From_Inventory(ByIndex, DropItemInfo);

	GetWorld()->SpawnActor(Weapon_Class, &spawn_location, &spawn_rotation, spawn_params);

	//switch weapon to valid slot weapon from start weapon slots array
	//while (i < Weapon_Slot.Num())
	//{
	//	if (!Weapon_Slot[i].Name.IsNone())
	//		On_Switch_Weapon.Broadcast(Weapon_Slot[j].Name, Weapon_Slot[j].Info, j);

	//	i++;
	//}

	Inventory->Weapon_Slot[ByIndex] = emty_weapon_slot;
	//On_Update_Weapon_Slots.Broadcast(ByIndex, EmtyWeaponSlot);

}
//-------------------------------------------------------------------------------------------------------------
AWeapon_Default *ATDSCharacter::Get_Weapon()
{
	return Curr_Weapon;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Init(FName id_weapon, FAdditional_Weapon_Info new_weapon_additional_info, int new_index_weapon)
{
	if (!Weapon_Class)
		throw 23;

	UGame_Instance *game_instance = Cast<UGame_Instance>(GetGameInstance());
	if (!game_instance)
		throw 23;

	FWeapon_Info weapon_info;
	if (!game_instance->Get_Weapon_Info_By_Name(id_weapon, weapon_info))
	{
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::Init - weapon not found in table - NULL"));
		return;
	}

	if (Curr_Weapon)
	{
		Curr_Weapon = 0;
	}

	FVector spawn_location;
	FRotator spawn_rotation;
	FActorSpawnParameters spawn_params;

	spawn_location = FVector(ForceInitToZero);
	spawn_rotation = FRotator(ForceInitToZero);
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawn_params.Owner = this;
	spawn_params.Instigator = GetInstigator();

	if (AWeapon_Default *weapon = Cast<AWeapon_Default>(GetWorld()->SpawnActor(weapon_info.Weapon_Class, &spawn_location, &spawn_rotation, spawn_params)))
	{
		FAttachmentTransformRules rule(EAttachmentRule::SnapToTarget, false);

		weapon->AttachToComponent(GetMesh(), rule, FName("Weapon_Socket_Right_Hand"));
		weapon->Settings = weapon_info;
		weapon->Info = new_weapon_additional_info;

		Curr_Slot_Index = new_index_weapon;

		weapon->On_Fire.AddDynamic(this, &ATDSCharacter::Weapon_Fire);
 		weapon->On_Reload_Start.AddDynamic(this, &ATDSCharacter::Weapon_Reload_Start);
		weapon->On_Reload_End.AddDynamic(this, &ATDSCharacter::Weapon_Reload_End);

		Curr_Weapon = weapon;

		if (Curr_Weapon->Get_Round() <= 0 && Curr_Weapon->Can_Reload())
			Curr_Weapon->Init_Reload();

		Inventory->On_Ammo_Available.Broadcast(weapon->Settings.Type);
	}
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Update()
{
	float result_speed;

	result_speed = 600.0;

	switch (Movement_State)
	{
	case EMovement_State::Aim:
		result_speed = Movement_Info.Aim;
		break;

	case EMovement_State::Aim_Walk:
		result_speed = Movement_Info.Aim_Walk;
		break;

	case EMovement_State::Walk:
		result_speed = Movement_Info.Walk;
		break;

	case EMovement_State::Run:
		result_speed = Movement_Info.Run;
		break;

	case EMovement_State::Sprint:
		result_speed = Movement_Info.Sprint;
		break;
	
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = result_speed;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Change_Movement_State()
{
	EMovement_State new_state = EMovement_State::Run;

	if (!Is_Alive)
		return;

	if (!Walk_Enabled && !Sprint_Run_Enabled && !Aim_Enabled )
		new_state = EMovement_State::Run;
	else
	{
		if (Sprint_Run_Enabled)
		{
			Walk_Enabled = false;
			Aim_Enabled = false;
			new_state = EMovement_State::Sprint;
		}

		if (Walk_Enabled && !Sprint_Run_Enabled && Aim_Enabled)
			new_state = EMovement_State::Aim_Walk;
		else
			if (Walk_Enabled && !Sprint_Run_Enabled && !Aim_Enabled)
				new_state = EMovement_State::Walk;
			else
				if (!Walk_Enabled && !Sprint_Run_Enabled && Aim_Enabled)
					new_state = EMovement_State::Aim;
	}

	Set_Movement_State_On_Server(new_state);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Switch_Next_Weapon()
{
	if (Curr_Weapon && Inventory->Weapon_Slot.Num() > 1)
	{
		FAdditional_Weapon_Info old_info;
		int old_index;

		old_info = Curr_Weapon->Info;
		old_index = Curr_Slot_Index;

		Inventory->Switch_Weapon_To_Index(Curr_Slot_Index + 1, old_index, old_info);
	}	
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Switch_Prev_Weapon()
{
	if (Curr_Weapon && Inventory->Weapon_Slot.Num() > 1)
	{
		FAdditional_Weapon_Info old_info;
		int old_index;

		old_info = Curr_Weapon->Info;
		old_index = Curr_Slot_Index;

		Inventory->Switch_Weapon_To_Index(Curr_Slot_Index - 1, old_index, old_info);
	}
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BP_Weapon_Fire_Implementation()
{
	// In BP
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BP_Dead_Implementation()
{
	// In BP
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BP_Weapon_Reload_Start_Implementation(UAnimMontage *anim)
{
	// In BP
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BP_Weapon_Reload_End_Implementation(bool is_success)
{
	// In BP
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Set_Actor_Rotation_By_Yaw_On_Server_Implementation(float yaw)
{
	Set_Actor_Rotation_By_Yaw_Multicast(yaw);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Set_Movement_State_On_Server_Implementation(EMovement_State new_state)
{
	Set_Movement_State_Multicast(new_state);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Set_Actor_Rotation_By_Yaw_Multicast_Implementation(float yaw)
{
	if (Controller && !Controller->IsLocalPlayerController())
		SetActorRotation(FQuat(FRotator(0.0, yaw, 0.0)));
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Set_Movement_State_Multicast_Implementation(EMovement_State new_state)
{
	Movement_State = new_state;
	Update();
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Weapon_Fire()
{
	if (Inventory && Curr_Weapon)
		Inventory->Set_Additional_Weapon_Info(Curr_Slot_Index, Curr_Weapon->Info);

	BP_Weapon_Fire();
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Weapon_Reload_Start(UAnimMontage *anim)
{
	BP_Weapon_Reload_Start(anim);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Weapon_Reload_End(bool is_success, int ammo_safe)
{
	if (Inventory && Curr_Weapon)
		Inventory->Ammo_Slot_Change_Value(Curr_Weapon->Settings.Type, -ammo_safe);

	BP_Weapon_Reload_End(is_success);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Dead()
{
	int rand;
	float time_anim = 0.0;
	ATDSGameMode* game_mode = Cast<ATDSGameMode>(GetWorld()->GetAuthGameMode());


	rand = FMath::RandHelper(Dead_Animations.Num());

	if (!Dead_Animations.IsValidIndex(rand))
		throw 23;

	if (!GetMesh())
		throw 23;

	if (!GetMesh()->GetAnimInstance())
		throw 23;

	time_anim = Dead_Animations[rand]->GetPlayLength();
	GetMesh()->GetAnimInstance()->Montage_Play(Dead_Animations[rand]);

	Is_Alive = false;

	GetWorldTimerManager().SetTimer(Ragdoll_Timer, this, &ATDSCharacter::Enable_Ragdoll, time_anim, false);
	game_mode->BP_Dead();
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATDSCharacter, Movement_State);
	DOREPLIFETIME(ATDSCharacter, Curr_Weapon);
}
//-------------------------------------------------------------------------------------------------------------
