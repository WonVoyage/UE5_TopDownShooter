#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon_Default.h"
#include "TDSCharacter.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Functions

	ATDSCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void Weapon_Init();

	UFUNCTION(BlueprintCallable) void Update();
	UFUNCTION(BlueprintCallable) void Change_Movement_State();
	UFUNCTION(BlueprintCallable) static AWeapon_Default *Get_Weapon();
	UFUNCTION(BlueprintCallable) ATDSCharacter *Get_Character();


	// Variables
	static AWeapon_Default *Curr_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") EMovement_State Movement_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") FCharacter_Speed Movement_Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool Sprint_Run_Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool Walk_Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool Aim_Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo") TSubclassOf<AWeapon_Default> Weapon_Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") UCameraComponent* TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") USpringArmComponent* CameraBoom;
};
//-------------------------------------------------------------------------------------------------------------
