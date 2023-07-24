#include "Game_Instance.h"

//-------------------------------------------------------------------------------------------------------------
bool UGame_Instance::Get_Weapon_Info_By_Name(FName name_weapon, FWeapon_Info &out_info)
{
	bool is_find;
	FWeapon_Info *weapon_info_row;

	is_find = false;

	if (!Weapon_Info_Table)
		UE_LOG(LogTemp, Warning, TEXT("UGame_Instance::Get_Weapon_Info_By_Name - Weapon_Table - NULL"));

	if(weapon_info_row = Weapon_Info_Table->FindRow<FWeapon_Info>(name_weapon, "", false))
	{
		is_find = true;
		out_info = *weapon_info_row;
	}
	
	return is_find;
}
//-------------------------------------------------------------------------------------------------------------
