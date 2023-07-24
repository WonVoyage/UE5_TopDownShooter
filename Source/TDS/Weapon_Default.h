#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "My_Types.h"
#include "Projectile_Default.h"
#include "Weapon_Default.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API AWeapon_Default : public AActor
{
	GENERATED_BODY()
	
public:
	// Functions
	AWeapon_Default();

	virtual void Tick(float DeltaTime) override;
	
	void Fire_Tick(float delta_time);
	void Fire();
	void Weapon_Init();
	void Set_Weapon_State_Fire(bool is_fire);
	void Update_State_Weapon(EMovement_State movement_state);
	void Change_Dispersion();
	bool Weapon_Can_Fire();

	// Variables
	float Fire_Time;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USceneComponent *Scene_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USkeletalMeshComponent *Skeletal_Mesh_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UStaticMeshComponent *Static_Mesh_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UArrowComponent *Shoot_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Info") FAdditional_Weapon_Info Weapon_Info;
	UPROPERTY() FWeapon_Info Weapon_Settings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire_Logic") bool Weapon_Fire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire_Logic") bool Weapon_Reloading;
protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
