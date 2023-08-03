#include "Game_Instance.h"

//-------------------------------------------------------------------------------------------------------------
bool UGame_Instance::Get_Weapon_Info_By_Name(FName name_weapon, FWeapon_Info &out_info)
{
	if (!Weapon_Info_Table)
		throw 23;

	FWeapon_Info *weapon_info_row;

	if (weapon_info_row = Weapon_Info_Table->FindRow<FWeapon_Info>(name_weapon, "", false))
	{
		out_info = *weapon_info_row;
		return true;
	}
	
	return false;
}
//-------------------------------------------------------------------------------------------------------------
bool UGame_Instance::Get_Drop_Item_Info_By_Name(FName name_item, FDrop_Item &out_info)
{
	if (!Weapon_Info_Table)
		throw 23;

	FDrop_Item *drop_item_row;

	if (drop_item_row = Weapon_Info_Table->FindRow<FDrop_Item>(name_item, "", false))
	{
		out_info = *drop_item_row;
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------------------
