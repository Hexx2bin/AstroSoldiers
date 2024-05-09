// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASTROSOLDIER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	friend class AAstroSoldierCharacter;

	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipWeapon(const AWeapon* WeaponToEquip);

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapons, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AAstroSoldierCharacter> AstroCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapons, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AWeapon> EquippedWeapon;

		
};
