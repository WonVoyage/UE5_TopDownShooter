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

	int correct_index;
	int i;

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

	Set_Additional_Weapon_Info(old_index, old_info);
	On_Switch_Weapon.Broadcast(new_id_weapon, new_additional_info);

	//if (Weapon_Slot[index_destination].Name.IsNone())
	//	return;

	//FName new_weapon_name;
	//FAdditional_Weapon_Info new_weapon_info;
	//int i = 0;

	//if (index_destination > Weapon_Slot.Num() - 1)
	//	index_destination = 0;

	//if (index_destination < 0)
	//	index_destination = Weapon_Slot.Num() - 1;

	//Weapon_Slot[Curr_Slot_Index].Index = index_destination;
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
			Ammo_Slot[i].Cout += ammo_taken;

			if (Ammo_Slot[i].Cout > Ammo_Slot[i].Max_Cout)
				Ammo_Slot[i].Cout = Ammo_Slot[i].Max_Cout;

			On_Ammo_Change.Broadcast(Ammo_Slot[i].Weapon_Type, Ammo_Slot[i].Cout);

			is_find = true;
		}

		i++;
	}
}
//-------------------------------------------------------------------------------------------------------------
bool UInventory::Check_Ammo_For_Weapon(int index)
{
	return true;
}
//-------------------------------------------------------------------------------------------------------------
int UInventory::GetWeaponIndexSlotByName(FName IdWeaponName)
{
	bool is_find;
	int result;
 	int i;

	is_find = false;
	result = -1;
	i = 0;
	
	while (i < Weapon_Slot.Num() && !is_find)
	{
		if (Weapon_Slot[i].Name == IdWeaponName)
		{
			is_find = true;
			result = i;
		}

		i++;
	}

	return result;
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
					Weapon_Slot.RemoveAt(i);
					i--;
				}
			}
		}
	}

	if (Weapon_Slot.IsValidIndex(0))
	{
		if(!Weapon_Slot[0].Name.IsNone())
		On_Switch_Weapon.Broadcast(Weapon_Slot[0].Name, Weapon_Slot[0].Info);
	}
}
//-------------------------------------------------------------------------------------------------------------
