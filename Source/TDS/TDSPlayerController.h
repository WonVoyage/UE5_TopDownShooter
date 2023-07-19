#pragma once

//#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TDSPlayerController.generated.h"

//-------------------------------------------------------------------------------------------------------------
class UNiagaraSystem;
//-------------------------------------------------------------------------------------------------------------
UCLASS()
class ATDSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDSPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) float ShortPressThreshold;	// Time Threshold to know if it was a short press
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input) UNiagaraSystem* FXCursor;	// FX Class that we will spawn when clicking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")) class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")) class UInputAction* SetDestinationClickAction;	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")) class UInputAction* SetDestinationTouchAction;	// Jump Input Action 
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) class UInputAction* MoveAction;

protected:
	uint32 bMoveToMouseCursor : 1;	//* True if the controlled character should navigate to the mouse cursor

	virtual void SetupInputComponent() override;
	virtual void BeginPlay();

	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	void Move(const FInputActionValue& Value);

private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};
//-------------------------------------------------------------------------------------------------------------
