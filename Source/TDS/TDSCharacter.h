#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TDSCharacter.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDSCharacter();

	//virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	//virtual void SetupInputComponent(class UInputComponent *PlayerInputComponent);


	//UFUNCTION() void Input_Axis_X(float value);
	//UFUNCTION() void Input_Axis_Y(float value);
	//UFUNCTION() void Movement_Tick(float delta_seconds);

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//float Axis_X = 0.0f;
	//float Axis_Y = 0.0f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) class UCameraComponent* TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) class USpringArmComponent* CameraBoom;
};
//-------------------------------------------------------------------------------------------------------------
