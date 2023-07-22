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
		enhanced_input_component->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATDSPlayerController::OnInputStarted);
		enhanced_input_component->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATDSPlayerController::OnSetDestinationTriggered);
		enhanced_input_component->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATDSPlayerController::OnSetDestinationReleased);
		enhanced_input_component->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATDSPlayerController::OnSetDestinationReleased);
		//enhanced_input_component->BindAction(Scroll_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Scroll);
		enhanced_input_component->BindAction(Attack_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Attack_Pressed);
			
		//Setup keyboard input events
		enhanced_input_component->BindAction(Move_Action, ETriggerEvent::Triggered, this, &ATDSPlayerController::Move);
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
	APawn *controlled_pawn = GetPawn();

	if (controlled_pawn == nullptr)
		return;

	FVector2D movement_vector = Value.Get<FVector2D>(); // input is a Vector2D
	FRotator rotation = controlled_pawn->GetControlRotation(); // find out which way is forward
	FRotator yaw_rotation(0, rotation.Yaw, 0);
	FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X); // get forward vector
	FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y); // get right vector 

	controlled_pawn->AddMovementInput(forward_direction, movement_vector.Y);
	controlled_pawn->AddMovementInput(right_direction, movement_vector.X);
}
//-------------------------------------------------------------------------------------------------------------
void ATDSPlayerController::Attack_Pressed()
{
	AWeapon_Default *weapon;
	weapon = ATDSCharacter::Get_Weapon();

	if (weapon != 0)
		weapon->Set_Weapon_State_Fire(true);
	else
		UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::Attack_Pressed - Curr_Weapon - NULL"));
}
//-------------------------------------------------------------------------------------------------------------
