#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "My_Types.generated.h"

//-------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EMovement_State : uint8
{
	Aim		UMETA(Display_Name = "Aim"),
	Aim_Walk UMETA(Display_Name = "Aim_Walk"),
	Walk		UMETA(Display_Name = "Walk"),
	Run		UMETA(Display_Name = "Run"),
	Sprint	UMETA(Display_Name = "Sprint")
};
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FCharacter_Speed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float Aim = 300.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float Aim_Walk = 100.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float Walk = 200.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float Run = 600.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") float Sprint = 800.0;
};
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FProjectile_Info
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") TSubclassOf<class AProjectile_Default> Projectile = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") USoundBase *Hit_Sound = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") USoundBase *Explose_Sound = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") UParticleSystem *Explose_FX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Hit_Decal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem  *> Hit_FXs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") float Explose_Max_Damage = 40.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") float Damage = 20.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") float Life_Time = 20.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") float Init_Speed = 2000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") float Max_Radius_Damage = 200.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") bool Is_Like_Bomp = false;
};
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FWeapon_Dispersion
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion") float Aim_Start = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion") float Aim_Min = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion") float Aim_Max = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion") float Aim_Shoot_Coef = 1.0;
};
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FWeapon_Info : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class") TSubclassOf<class AWeapon_Default> Weapon_Class = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile") FProjectile_Info Projectile_Settings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound") USoundBase *Sound_Fire = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound") USoundBase *Sound_Reload = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX") UParticleSystem *Effect_Fire = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Effect") UDecalComponent *Decal_On_Hit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation") UAnimMontage *Animation_Character_Fire = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation") UAnimMontage *Animation_Character_Reload = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh") UStaticMesh *Magazine_Drop = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh") UStaticMesh *Sleeve_Bullets = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace") float Damage = 20.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace") float Distance_Trace = 2000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State") float Rate_Of_Fire = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State") float Reload_Time = 2.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State") int Max_Round = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State") int Projectile_By_Shot = 1;
};
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FAdditional_Weapon_Info
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Stats") int Round = 10;
};
//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API UMy_Types : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()
};
//-------------------------------------------------------------------------------------------------------------
