// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Weapons/Weapon.h"
#include "AstroSoldierCharacter.generated.h"

class UInputActionsData;
class UInputMappingContext;
class UWidgetComponent;
class UCombatComponent;

UCLASS()
class ASTROSOLDIER_API AAstroSoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AAstroSoldierCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	FORCEINLINE TObjectPtr<AInteractuableItem> GetInteractuableItem() const { return this->InteractuableItem; }
	void SetInteractuableItem(AInteractuableItem* NewInteractuableItem);
	FORCEINLINE TObjectPtr<UCombatComponent> GetCombatComponent() const { return Combat; } 

	UFUNCTION()
	void OnRep_SetInteractuableItem(AInteractuableItem* NewInteractuableItem);

protected:
	
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& ActionValue);
	void Rotate(const FInputActionValue& ActionValue);
	void Interact(const FInputActionValue& ActionValue);
	void Crouch(const FInputActionValue& ActionValue);

private:

	/* Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	/*Camera that follows the player*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	/*Data list with all data actions used in ShooterGameplayContext*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Elements|EnhancedInput", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputActionsData> IADataList;

	/*Context that contains actions to control character*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Elements|EnhancedInput", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> GameplayMappingContext;

	/*Velocity of normal horizontal turn rate*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Elements|Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = 0.f), meta = (ClampMax = 1.f))
	float HipTurnRate = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Elements|Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = 0.f), meta = (ClampMax = 1.f))
    float LookUpTurnRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Elements|HUD", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> OverHeadWidget;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Elements|Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> Combat;

private:
	////////////RPC FUNCTIONS////////////////

	UFUNCTION(Server, Reliable)
	void ServerInteract();
	
	////////////REPLICATED MEMBERS//////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Elements|Interactuables", meta=(AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_SetInteractuableItem)
	AInteractuableItem* InteractuableItem;
};
