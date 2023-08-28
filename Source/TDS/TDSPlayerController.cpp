#include "TDSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//-------------------------------------------------------------------------------------------------------------
ATDSPlayerController::ATDSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* sub_system = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		sub_system->AddMappingContext(DefaultMappingContext, 0);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent *enhanced_input_component = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		enhanced_input_component->BindAction(Attack_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Attack_Pressed);
		enhanced_input_component->BindAction(Attack_Action, ETriggerEvent::Completed, this, &ATDSPlayerController::Attack_Released);
			
		//Setup keyboard input events
		enhanced_input_component->BindAction(Move_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Move);
		enhanced_input_component->BindAction(Reload_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Try_Reload_Weapon);
		enhanced_input_component->BindAction(Change_Weapon_Slot_To_1_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Change_Weapon_Slot_To_1);
		enhanced_input_component->BindAction(Change_Weapon_Slot_To_2_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Change_Weapon_Slot_To_2);
		enhanced_input_component->BindAction(Change_Weapon_Slot_To_3_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Change_Weapon_Slot_To_3);
		enhanced_input_component->BindAction(Change_Weapon_Slot_To_4_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Change_Weapon_Slot_To_4);
	}
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::OnInputStarted()
{
	StopMovement();
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::OnSetDestinationTriggered()
{// Triggered every frame when the input is held down

	APawn *controlled_pawn = GetPawn();
	FHitResult hit;
	bool hit_successful = false;

	FollowTime += GetWorld()->GetDeltaSeconds();	// We flag that the input is being pressed
	
	// We look for the location in the world where the player has pressed the input
	if (bIsTouch)
		hit_successful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, hit);
	else
		hit_successful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, hit);

	// If we hit a surface, cache the location
	if (hit_successful)
		CachedDestination = hit.Location;
	
	// Move towards mouse pointer or touch
	if (controlled_pawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - controlled_pawn->GetActorLocation()).GetSafeNormal();
		controlled_pawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::OnSetDestinationReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination); // We move there and spawn some particles
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D movement_vector;
	FRotator rotation;
	FRotator yaw_rotation;
	FRotator rotation_to_look;
	FVector forward_direction;
	FVector right_direction;
	
	ATDSCharacter* character = Cast<ATDSCharacter>(GetPawn());

	if (character == nullptr)
		return;

	if (!IsLocalPlayerController())
		return;

	movement_vector = Value.Get<FVector2D>(); // input is a Vector2D
	rotation = character->GetControlRotation(); // find out which way is forward
	yaw_rotation = FRotator(0, rotation.Yaw, 0);
	rotation_to_look = FVector(movement_vector.X, movement_vector.Y, 0.0).ToOrientationRotator();
	forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X); // get forward vector
	right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y); // get right vector 

	character->AddMovementInput(forward_direction, movement_vector.Y);
	character->AddMovementInput(right_direction, movement_vector.X);

	character->Set_Actor_Rotation_By_Yaw_On_Server(rotation_to_look.Yaw);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Attack_Pressed()
{
	AWeapon_Default *weapon;
	weapon = ATDSCharacter::Get_Weapon();

	if (weapon != 0)
		weapon->Set_State_Fire(true);
	else
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::Attack_Pressed - Curr_Weapon - NULL"));
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Attack_Released()
{
	AWeapon_Default *weapon;
	weapon = ATDSCharacter::Get_Weapon();

	if (weapon != 0)
		weapon->Set_State_Fire(false);
	else
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::Attack_Pressed - Curr_Weapon - NULL"));
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Try_Reload_Weapon()
{
	if (AWeapon_Default *weapon = ATDSCharacter::Get_Weapon())
		if (weapon->Get_Round() < weapon->Settings.Max_Round && weapon->Can_Reload())
			weapon->Init_Reload();
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Change_Weapon_Slot_To_1()
{
	AWeapon_Default *weapon = ATDSCharacter::Get_Weapon();

	if (!weapon)
		throw 23;

	if (!weapon->GetOwner())
		throw 23;

	UInventory *inventory = Cast<UInventory>(weapon->GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		throw 23;

	ATDSCharacter* character;

	character = Cast<ATDSCharacter>(weapon->GetOwner());

	if (!character)
		throw 23;

	inventory->Switch_Weapon_To_Index(0, character->Curr_Slot_Index, weapon->Info);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Change_Weapon_Slot_To_2()
{
	AWeapon_Default *weapon = ATDSCharacter::Get_Weapon();

	if (!weapon)
		throw 23;

	if (!weapon->GetOwner())
		throw 23;

	UInventory *inventory = Cast<UInventory>(weapon->GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		throw 23;

	ATDSCharacter* character;

	character = Cast<ATDSCharacter>(weapon->GetOwner());

	if (!character)
		throw 23;

	inventory->Switch_Weapon_To_Index(1, character->Curr_Slot_Index, weapon->Info);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Change_Weapon_Slot_To_3()
{
	AWeapon_Default *weapon = ATDSCharacter::Get_Weapon();

	if (!weapon)
		throw 23;

	if (!weapon->GetOwner())
		throw 23;

	UInventory *inventory = Cast<UInventory>(weapon->GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		throw 23;

	ATDSCharacter* character;

	character = Cast<ATDSCharacter>(weapon->GetOwner());

	if (!character)
		throw 23;

	inventory->Switch_Weapon_To_Index(2, character->Curr_Slot_Index, weapon->Info);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Change_Weapon_Slot_To_4()
{
	AWeapon_Default *weapon = ATDSCharacter::Get_Weapon();

	if (!weapon)
		throw 23;

	if (!weapon->GetOwner())
		throw 23;

	UInventory *inventory = Cast<UInventory>(weapon->GetOwner()->GetComponentByClass(UInventory::StaticClass()));

	if (!inventory)
		throw 23;

	ATDSCharacter* character;

	character = Cast<ATDSCharacter>(weapon->GetOwner());

	if (!character)
		throw 23;

	inventory->Switch_Weapon_To_Index(3, character->Curr_Slot_Index, weapon->Info);
}
//-------------------------------------------------------------------------------------------------------------
