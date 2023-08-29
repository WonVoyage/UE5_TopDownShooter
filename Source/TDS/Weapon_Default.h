#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Net/UnrealNetwork.h"

#include "Projectile_Default.h"
#include "Inventory.h"
#include "Weapon_Default.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn_Fire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Reload_Start, UAnimMontage*, anim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Reload_End, bool, is_success, int, ammo_safe);
//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API AWeapon_Default : public AActor
{
	GENERATED_BODY()
	
public:
	// Functions
	AWeapon_Default();

	virtual void Tick(float delta_time);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	void Reload_Tick(float delta_time);
	UFUNCTION(Server, Reliable) void Fire_Server();
	UFUNCTION(NetMulticast, Reliable) void Fire_Multicast();
	void Init();
	void Init_Reload();
	void Finish_Reload();
	void Cancel_Reload();
	bool Can_Reload();
	int Get_Aviable_Ammo_For_Reload();
	int Get_Round();
	int Get_Number_Projectile_By_Shot();

	// Variables
	FVector Shoot_End_Location;

	UPROPERTY(BlueprintAssignable) FOn_Fire On_Fire;
	UPROPERTY(BlueprintAssignable) FOn_Reload_Start On_Reload_Start;
	UPROPERTY(BlueprintAssignable) FOn_Reload_End On_Reload_End;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USceneComponent *Scene_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USkeletalMeshComponent *Skeletal_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UStaticMeshComponent *Static_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UArrowComponent *Shoot_Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Info") FWeapon_Info Settings;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reload_Logic") float Reload_Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload_Logic") bool Reloading;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire_Logic") float Fire_Timer;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite) bool Is_Fire;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite) FAdditional_Weapon_Info Info;

protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
