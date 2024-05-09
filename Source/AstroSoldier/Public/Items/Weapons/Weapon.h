// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/InteractuableItem.h"
#include "Weapon.generated.h"

UCLASS()
class ASTROSOLDIER_API AWeapon : public AInteractuableItem
{
	GENERATED_BODY()
	
public:

	AWeapon();

	virtual void BeginPlay() override;

protected:
	

private:
		
	
};
