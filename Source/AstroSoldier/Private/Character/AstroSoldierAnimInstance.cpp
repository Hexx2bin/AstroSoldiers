// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.


#include "Character/AstroSoldierAnimInstance.h"

#include "AstroComponents/CombatComponent.h"
#include "Character/AstroSoldierCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAstroSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AstroCharacter = Cast<AAstroSoldierCharacter>(TryGetPawnOwner());
}

void UAstroSoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AstroCharacter == nullptr)
	{
		AstroCharacter = Cast<AAstroSoldierCharacter>(TryGetPawnOwner());
	}
	if (AstroCharacter == nullptr) return;
		
	FVector Velocity = AstroCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = AstroCharacter->GetCharacterMovement()->IsFalling();
	bIsAcelerating = AstroCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsWeaponEquipped = AstroCharacter->GetCombatComponent()->GetEquippedWeapon() != nullptr;

#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, bIsWeaponEquipped ? FString::Printf(TEXT("bIsWeaponEquipped: true")) : FString::Printf(TEXT("bIsWeaponEquipped: false")));
	}
#endif
}
