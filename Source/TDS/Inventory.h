#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "My_Types.h"
#include "Game_Instance.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Switch_Weapon, FName, Weapon_Name, FAdditional_Weapon_Info, Weapon_Info);
//-------------------------------------------------------------------------------------------------------------
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Functions
	UInventory();

	virtual void TickComponent(float delta_time, ELevelTick tick_type, FActorComponentTickFunction* this_tick_function) override;

	void Switch_Weapon_To_Index(int index_destination, int old_index, FAdditional_Weapon_Info old_info);
	void Set_Additional_Weapon_Info(int old_index, FAdditional_Weapon_Info old_info);

	int GetWeaponIndexSlotByName(FName IdWeaponName);

	// Variables
	FOn_Switch_Weapon On_Switch_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FWeapon_Slot> Weapon_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FAmmo_Slot> Ammo_Slot;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") int Curr_Slot_Index;

protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
