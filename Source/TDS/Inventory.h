#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game_Instance.h"
#include "Inventory.generated.h"

//-------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FWeapon_Slot_Test
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon_Slot") TArray<FWeapon_Slot> Weapon_Slot;
};
//-------------------------------------------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOn_Switch_Weapon, FName, weapon_name, FAdditional_Weapon_Info, weapon_info, int, new_current_index_weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Ammo_Change, EWeapon_Type, type_ammo, int, cout);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Weapon_Additional_Info_Change, int, index, FAdditional_Weapon_Info, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Ammo_Empty, EWeapon_Type, weapon_type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Ammo_Available, EWeapon_Type, weapon_type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Update_Weapon_Slots, int, index_slot_change, FWeapon_Slot, new_info);
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
	void Save_Item_To_Inventory(FWeapon_Slot new_slot, int index_slot, int curr_index);
	void Drop_Weapon();
	bool Check_Ammo_For_Weapon(EWeapon_Type weapon_type, int &available_ammo);
	int Get_Weapon_Index_Slot_By_Name(FName weapon_name);

	UFUNCTION(BlueprintCallable) void Ammo_Slot_Change_Value(EWeapon_Type type_weapon, int ammo_taken);
	UFUNCTION(BlueprintCallable) bool Can_Pickup_Ammo(EWeapon_Type weapon_type);
	UFUNCTION(BlueprintCallable) bool Can_Pickup_Weapon(int &slot_number);
	UFUNCTION(BlueprintCallable) bool Get_Weapon_To_Inventory(FWeapon_Slot new_weapon);


	// Variables
	FOn_Switch_Weapon On_Switch_Weapon;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Ammo_Change On_Ammo_Change;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Ammo_Empty On_Ammo_Empty;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Ammo_Empty On_Ammo_Available;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Weapon_Additional_Info_Change On_Weapon_Additional_Info_Change;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Update_Weapon_Slots On_Update_Weapon_Slots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FWeapon_Slot> Weapon_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FAmmo_Slot> Ammo_Slot;

protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
