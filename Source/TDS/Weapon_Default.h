#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Projectile_Default.h"
#include "Inventory.h"
#include "Weapon_Default.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn_Weapon_Fire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Weapon_Reload_Start, UAnimMontage*, anim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Weapon_Reload_End, bool, is_success, int, ammo_safe);
//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API AWeapon_Default : public AActor
{
	GENERATED_BODY()
	
public:
	// Functions
	AWeapon_Default();

	virtual void Tick(float delta_time);
	
	void Reload_Tick(float delta_time);
	void Fire_Tick(float delta_time);
	void Fire();
	void Init_Weapon();
	void Init_Reload();
	void Set_Weapon_State_Fire(bool is_fire);
	void Update_State_Weapon(EMovement_State movement_state);
	void Finish_Reload();
	void Cancel_Reload();
	bool Can_Weapon_Reload();
	bool Weapon_Can_Fire();
	int Get_Weapon_Round();
	int Get_Number_Projectile_By_Shot();

	// Variables
	FVector Shoot_End_Location;
	FName Curr_Weapon_Name;

	UPROPERTY(BlueprintAssignable) FOn_Weapon_Fire On_Weapon_Fire;
	UPROPERTY(BlueprintAssignable) FOn_Weapon_Reload_Start On_Weapon_Reload_Start;
	UPROPERTY(BlueprintAssignable) FOn_Weapon_Reload_End On_Weapon_Reload_End;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USceneComponent *Scene_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USkeletalMeshComponent *Skeletal_Mesh_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UStaticMeshComponent *Static_Mesh_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UArrowComponent *Shoot_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Info") FAdditional_Weapon_Info Weapon_Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Info") FWeapon_Info Weapon_Settings;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reload_Logic") float Reload_Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload_Logic") bool Reloading;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire_Logic") float Fire_Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire_Logic") bool Weapon_Fire;

protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
