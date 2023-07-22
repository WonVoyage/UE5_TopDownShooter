#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile_Settings") float Damage = 20.0;
};
//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FWeapon_Info
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Settings") float Damage = 20.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Settings") float Rate_Of_Fire = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Settings") FProjectile_Info Projectile_Settings;
};
//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API UMy_Types : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()
};
//-------------------------------------------------------------------------------------------------------------
