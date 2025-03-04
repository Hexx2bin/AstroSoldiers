// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.


#include "AstroComponents/CombatComponent.h"
#include "Character/AstroSoldierCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Items/Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, EquippedWeapon != nullptr ? FString::Printf(TEXT("bIsWeaponEquipped: true")) : FString::Printf(TEXT("bIsWeaponEquipped: false")));
	}
#endif
	
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(AstroCharacter == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	const USkeletalMeshSocket* HandSocket = AstroCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if(HandSocket != nullptr)
	{
		HandSocket->AttachActor(EquippedWeapon, AstroCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(AstroCharacter);
	EquippedWeapon->SetPickUpWidgetVisibility(false);
}
