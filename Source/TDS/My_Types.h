#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "My_Types.generated.h"

//-------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EMovement_State : uint8
{
	Aim UMETA(Display_Name = "Aim"),
	Aim_Walk UMETA(Display_Name = "Aim_Walk"),
	Walk UMETA(Display_Name = "Walk"),
	Run UMETA(Display_Name = "Run"),
	Sprint UMETA(Display_Name = "Sprint")
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
UCLASS()
class TDS_API UMy_Types : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()
};
//-------------------------------------------------------------------------------------------------------------
