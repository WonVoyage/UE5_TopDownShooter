#include "Inventory.h"

//-------------------------------------------------------------------------------------------------------------
UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::TickComponent(float delta_time, ELevelTick tick_type, FActorComponentTickFunction* this_tick_function)
{
	Super::TickComponent(delta_time, tick_type, this_tick_function);
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::Switch_Weapon_To_Index(int index_destination, int old_index, FAdditional_Weapon_Info old_info)
{
	FName new_id_weapon;
	FAdditional_Weapon_Info new_additional_info;

	int i;
	int correct_index;
	int new_current_index;

	correct_index = index_destination;
	i = 0;

	if (index_destination > Weapon_Slot.Num() - 1)
		correct_index = 0;
	else
		if (index_destination < 0)
			correct_index = Weapon_Slot.Num() - 1;

	while(i < Weapon_Slot.Num())
	{
		if (i == correct_index)
		{
			if(!Weapon_Slot[i].Name.IsNone())
			{
				new_id_weapon = Weapon_Slot[i].Name;
				new_additional_info = Weapon_Slot[i].Info;
			}
		}

		i++;
	}

	new_current_index = correct_index;

	Set_Additional_Weapon_Info(old_index, old_info);
	On_Switch_Weapon.Broadcast(new_id_weapon, new_additional_info, new_current_index);

	//if (Weapon_Slot[index_destination].Name.IsNone())
	//	return;

	//FName new_weapon_name;
	//FAdditional_Weapon_Info new_weapon_info;
	//int i = 0;

	//if (index_destination > Weapon_Slot.Num() - 1)
	//	index_destination = 0;

	//if (index_destination < 0)
	//	index_destination = Weapon_Slot.Num() - 1;

	//Weapon_Slot[Curr_Slot_Index].Info = Weapon_Slot[index_destination].Info;
	//Weapon_Slot[Curr_Slot_Index] = Weapon_Slot[index_destination];
	//new_weapon_name = Weapon_Slot[Curr_Slot_Index].Name;
	//new_weapon_info = Weapon_Slot[Curr_Slot_Index].Info;


	//while (i < Weapon_Slot.Num())
	//{
	//	if (i == index_destination)
	//	{
	//		Weapon_Slot
	//	}
	//}
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::Set_Additional_Weapon_Info(int old_index, FAdditional_Weapon_Info old_info)
{
	if (Weapon_Slot.IsValidIndex(old_index))
	{
		bool is_find;
		int i;

		is_find = false;
		i = 0;

		while (i < Weapon_Slot.Num() && !is_find)
		{
			if (i == old_index)
			{
				Weapon_Slot[i].Info = old_info;
				is_find = true;
				On_Weapon_Additional_Info_Change.Broadcast(old_index, old_info);
			}

			i++;
		}

		if(!is_find)
			UE_LOG(LogTemp, Warning, TEXT("UTPSInventoryComponent::SetAdditionalInfoWeapon - Not Found Weapon with index - %d"), old_index);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("UTPSInventoryComponent::SetAdditionalInfoWeapon - Not Correct index Weapon - %d"), old_index);
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::Save_Item_To_Inventory(FWeapon_Slot new_slot, int index_slot, int curr_index)
{
	return;
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::Clear_Weapon_Slot(int curr_slot)
{
	//FName drop_item_name;

	//drop_item_name = Get_Weapon_Name_By_Slot_Index(curr_slot);

	//if (UGame_Instance* game_instance = Cast<UGame_Instance>(GetWorld()->GetGameInstance()))
	//	if (game_instance->Get_Drop_Item_Info_By_Name(drop_item_name, drop_item))
	//	{
	//		drop_item.Slot.Info = Weapon_Slot[curr_slot].Info;
	//		drop_item.Slot.Name = TEXT("None");
	//	}
	//	else
	//		throw 23;
	//else
	//	throw 23;

	Weapon_Slot[curr_slot].Info.Round = 0;
	Weapon_Slot[curr_slot].Name = TEXT("None");
}
//-------------------------------------------------------------------------------------------------------------
bool UInventory::Get_Drop_Item_From_Inventory(int index_slot, FDrop_Item &drop_item)
{
	bool result;
	FName drop_item_name;

	result = false;
	drop_item_name = Get_Weapon_Name_By_Slot_Index(index_slot);

	if (UGame_Instance *game_instance = Cast<UGame_Instance>(GetWorld()->GetGameInstance()))
		result = game_instance->Get_Drop_Item_Info_By_Name(drop_item_name, drop_item);

	if (Weapon_Slot.IsValidIndex(index_slot))
		drop_item.Slot.Info = Weapon_Slot[index_slot].Info;

	return result;
}
//-------------------------------------------------------------------------------------------------------------
bool UInventory::Check_Ammo_For_Weapon(EWeapon_Type weapon_type, int &available_ammo)
{
	int i = 0;

	while (i < Ammo_Slot.Num())
	{
		if (Ammo_Slot[i].Weapon_Type == weapon_type)
		{
			if (Ammo_Slot[i].Count > 0)
			{
				available_ammo = Ammo_Slot[i].Count;
				return true;
			}
		}

		i++;
	}

	On_Ammo_Empty.Broadcast(weapon_type);

	return false;
}
//-------------------------------------------------------------------------------------------------------------
int UInventory::Get_Weapon_Index_Slot_By_Name(FName weapon_name)
{
	int i = 0;
		
	while (i < Weapon_Slot.Num())
	{
		if (Weapon_Slot[i].Name == weapon_name)
			return i;

		i++;
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::Ammo_Slot_Change_Value(EWeapon_Type type_weapon, int ammo_taken)
{
	bool is_find;
	int i;

	is_find = false;
	i = 0;

	while (i < Ammo_Slot.Num() && !is_find)
	{
		if (Ammo_Slot[i].Weapon_Type == type_weapon)
		{
			Ammo_Slot[i].Count += ammo_taken;

			if (Ammo_Slot[i].Count > Ammo_Slot[i].Max_Count)
				Ammo_Slot[i].Count = Ammo_Slot[i].Max_Count;

			On_Ammo_Change.Broadcast(Ammo_Slot[i].Weapon_Type, Ammo_Slot[i].Count);

			is_find = true;
		}

		i++;
	}
}
//-------------------------------------------------------------------------------------------------------------
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	int i;

	for (i = 0; i < Weapon_Slot.Num(); i++)
	{
		UGame_Instance *my_gi = Cast<UGame_Instance>(GetWorld()->GetGameInstance());
		if (my_gi)
		{
			if (!Weapon_Slot[i].Name.IsNone())
			{
				FWeapon_Info info;

				if (my_gi->Get_Weapon_Info_By_Name(Weapon_Slot[i].Name, info))
					Weapon_Slot[i].Info.Round = info.Max_Round;
				else
				{
					//Weapon_Slot.RemoveAt(i);
					//i--;
				}
			}
		}
	}

	if (Weapon_Slot.IsValidIndex(0))
	{
		if(!Weapon_Slot[0].Name.IsNone())
		On_Switch_Weapon.Broadcast(Weapon_Slot[0].Name, Weapon_Slot[0].Info, 0);
	}
}
//-------------------------------------------------------------------------------------------------------------
bool UInventory::Can_Pickup_Ammo(EWeapon_Type weapon_type)
{
	int i = 0;

	while (i < Ammo_Slot.Num())
	{
		if (Ammo_Slot[i].Weapon_Type == weapon_type)
			return true;

		i++;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------
bool UInventory::Can_Pickup_Weapon(int &slot_number)
{
	int i = 0;

	while (i < Weapon_Slot.Num())
	{
		if (Weapon_Slot[i].Name.IsNone())
		{
			slot_number = i;
			return true;
		}

		i++;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------
bool UInventory::Get_Weapon_To_Inventory(FWeapon_Slot new_weapon)
{
	int i = 0;
	int slot_number = -1;

	while (i < Weapon_Slot.Num())
	{
		if (Can_Pickup_Weapon(slot_number))
		{
			Weapon_Slot[slot_number] = new_weapon;

			On_Update_Weapon_Slots.Broadcast(slot_number, new_weapon);
			return true;
		}

		i++;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------
FName UInventory::Get_Weapon_Name_By_Slot_Index(int index)
{
	if (Weapon_Slot.IsValidIndex(index))
		return Weapon_Slot[index].Name;

	return "";
}
//-------------------------------------------------------------------------------------------------------------
