#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Weapon_Default.h"
#include "Game_Instance.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API UGame_Instance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Functions
	UFUNCTION(BlueprintCallable) bool Get_Weapon_Info_By_Name(FName name_weapon, FWeapon_Info &out_info);
	
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Settings") UDataTable *Weapon_Info_Table = 0;
};
//-------------------------------------------------------------------------------------------------------------
