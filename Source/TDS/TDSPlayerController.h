#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "NiagaraSystem.h"
#include "TDSCharacter.h"
#include "TDSPlayerController.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class ATDSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Functions
	ATDSPlayerController();

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UNiagaraSystem* FXCursor;	// FX Class that we will spawn when clicking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UInputAction* SetDestinationClickAction;	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UInputAction* SetDestinationTouchAction;	// Jump Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UInputAction* Move_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") UInputAction* Attack_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") float ShortPressThreshold;	// Time Threshold to know if it was a short press

protected:

	// Functions
	virtual void SetupInputComponent() override;
	virtual void BeginPlay();

	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void Move(const FInputActionValue& Value);
	void Attack_Pressed();

	// Variables
	uint32 bMoveToMouseCursor : 1;	//* True if the controlled character should navigate to the mouse cursor

private:
	// Variables
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};
//-------------------------------------------------------------------------------------------------------------
