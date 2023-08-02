#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game_Instance.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Switch_Weapon, FName, weapon_name, FAdditional_Weapon_Info, weapon_info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Ammo_Change, EWeapon_Type, type_ammo, int, cout);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Weapon_Additional_Info_Change, int, index, FAdditional_Weapon_Info, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Ammo_Empty, EWeapon_Type, weapon_type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Ammo_Available, EWeapon_Type, weapon_type);
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
	void Ammo_Slot_Change_Value(EWeapon_Type type_weapon, int ammo_taken);
	void Save_Item_To_Inventory();
	bool Check_Ammo_For_Weapon(EWeapon_Type weapon_type, int &available_ammo);
	int GetWeaponIndexSlotByName(FName IdWeaponName);

	UFUNCTION(BlueprintCallable) bool Can_Pickup_Ammo();
	UFUNCTION(BlueprintCallable) bool Can_Pickup_Weapon();


	// Variables
	FOn_Switch_Weapon On_Switch_Weapon;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Ammo_Change On_Ammo_Change;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Ammo_Empty On_Ammo_Empty;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Ammo_Empty On_Ammo_Available;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory") FOn_Weapon_Additional_Info_Change On_Weapon_Additional_Info_Change;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FWeapon_Slot> Weapon_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons") TArray<FAmmo_Slot> Ammo_Slot;

protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
