#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Game_Instance.h"
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
	
	virtual void Tick(float delta_seconds);
	virtual void BeginPlay();

	static AWeapon_Default *Get_Weapon();
	
	UFUNCTION(BlueprintCallable) void Init_Weapon(FName id_weapon, FAdditional_Weapon_Info new_weapon_additional_info);
	UFUNCTION(BlueprintCallable) void Update();
	UFUNCTION(BlueprintCallable) void Change_Movement_State();
	UFUNCTION(BlueprintCallable) void Switch_Next_Weapon();
	UFUNCTION(BlueprintCallable) void Switch_Prev_Weapon();
	UFUNCTION(BlueprintNativeEvent) void BP_Weapon_Fire();
	UFUNCTION(BlueprintNativeEvent) void BP_Weapon_Reload_Start(UAnimMontage *anim);
	UFUNCTION(BlueprintNativeEvent) void BP_Weapon_Reload_End(bool is_success);
	UFUNCTION() void Weapon_Fire();
	UFUNCTION() void Weapon_Reload_Start(UAnimMontage *anim);
	UFUNCTION() void Weapon_Reload_End(bool is_success, int ammo_safe);

	// Variables
	static AWeapon_Default *Curr_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory") int Curr_Slot_Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory") UInventory *Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") EMovement_State Movement_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") FCharacter_Speed Movement_Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool Sprint_Run_Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool Walk_Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool Aim_Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo") FName Init_Weapon_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo") FAdditional_Weapon_Info Weapon_Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo") TSubclassOf<AWeapon_Default> Weapon_Class;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") UCameraComponent* TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") USpringArmComponent* CameraBoom;
};
//-------------------------------------------------------------------------------------------------------------
