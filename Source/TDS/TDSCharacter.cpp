#include "TDSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"


AWeapon_Default *ATDSCharacter::Curr_Weapon = 0;
//-------------------------------------------------------------------------------------------------------------
ATDSCharacter::ATDSCharacter()
	: Movement_State(EMovement_State::Run), Sprint_Run_Enabled(false), Walk_Enabled(false), Aim_Enabled(false)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

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
	Init_Weapon(Init_Weapon_Name);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSCharacter::Init_Weapon(FName id_weapon)
{
	if (!Weapon_Class)
		return;

	UGame_Instance *game_instance = Cast<UGame_Instance>(GetGameInstance());
	if (!game_instance)
		return;

	FWeapon_Info weapon_info;
	if(!game_instance->Get_Weapon_Info_By_Name(id_weapon, weapon_info))
	{
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::Init_Weapon - weapon not found in table - NULL"));
		return;
	}

	FVector spawn_location;
	FRotator spawn_rotation;
	FActorSpawnParameters spawn_params;

	spawn_location = FVector(ForceInitToZero);
	spawn_rotation = FRotator(ForceInitToZero);
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawn_params.Owner = GetOwner();
	spawn_params.Instigator = GetInstigator();

	if (AWeapon_Default *weapon = Cast<AWeapon_Default>(GetWorld()->SpawnActor(weapon_info.Weapon_Class, &spawn_location, &spawn_rotation, spawn_params)))
	{
		FAttachmentTransformRules rule(EAttachmentRule::SnapToTarget, false);

		weapon->AttachToComponent(GetMesh(), rule, FName("Weapon_Socket_Right_Hand"));

		Curr_Weapon = weapon;

		weapon->Weapon_Settings = weapon_info;
		weapon->Update_State_Weapon(Movement_State);
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
		weapon->Update_State_Weapon(Movement_State);
}
//-------------------------------------------------------------------------------------------------------------
AWeapon_Default *ATDSCharacter::Get_Weapon()
{
	return Curr_Weapon;
}
//-------------------------------------------------------------------------------------------------------------
ATDSCharacter *ATDSCharacter::Get_Character()
{
	return this;
}
//-------------------------------------------------------------------------------------------------------------
