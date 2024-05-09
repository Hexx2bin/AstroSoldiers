// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#include "Items/Weapons/Weapon.h"

AWeapon::AWeapon()
{
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	ItemInteraction = EItemInteraction::EII_Equip;
}
