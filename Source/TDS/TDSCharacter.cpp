#include "TDSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

AWeapon_Default *ATDSCharacter::Curr_Weapon = 0;
//-------------------------------------------------------------------------------------------------------------
ATDSCharacter::ATDSCharacter()
:  Curr_Slot_Index(0), Inventory(0), Movement_State(EMovement_State::Run), Sprint_Run_Enabled(false), Walk_Enabled(false), Aim_Enabled(false)
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

	UInventory *inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory_Component"));

	if (inventory)
		inventory->On_Switch_Weapon.AddDynamic(this, &ATDSCharacter::Init);

	Inventory = inventory;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Init(Init_Weapon_Name, Weapon_Info, Curr_Slot_Index);
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
		return;

	UGame_Instance *game_instance = Cast<UGame_Instance>(GetGameInstance());
	if (!game_instance)
		return;

	FWeapon_Info weapon_info;
	if(!game_instance->Get_Weapon_Info_By_Name(id_weapon, weapon_info))
	{
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::Init - weapon not found in table - NULL"));
		return;
	}

	if (Curr_Weapon)
	{
		//Curr_Weapon->Destroy();
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
		weapon->Update_State(Movement_State);
		//weapon->Info.Round = weapon->Settings.Max_Round;
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
	if(!Walk_Enabled && !Sprint_Run_Enabled && !Aim_Enabled)
		Movement_State = EMovement_State::Run;
	else
	{
		if(Sprint_Run_Enabled)
		{
			Walk_Enabled = false;
			Aim_Enabled = false;
			Movement_State = EMovement_State::Sprint;
		}
		if(Walk_Enabled && !Sprint_Run_Enabled && Aim_Enabled)
			Movement_State = EMovement_State::Aim_Walk;
		else
			if(Walk_Enabled && !Sprint_Run_Enabled && !Aim_Enabled)
				Movement_State = EMovement_State::Walk;
			else
				if(!Walk_Enabled && !Sprint_Run_Enabled && Aim_Enabled)
					Movement_State = EMovement_State::Aim;
	}

	Update();

	if (AWeapon_Default *weapon = Get_Weapon())
		weapon->Update_State(Movement_State);
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

	//int old_index = Inventory->Curr_Slot_Index;
	//FAdditional_Weapon_Info old_info = Curr_Weapon->Info;

	//Inventory->Switch_Weapon_To_Index(Inventory->Curr_Slot_Index + 1, old_index, old_info);
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
	// in bp
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BP_Weapon_Reload_Start_Implementation(UAnimMontage *anim)
{
	// in bp
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::BP_Weapon_Reload_End_Implementation(bool is_success)
{
	// in bp
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
