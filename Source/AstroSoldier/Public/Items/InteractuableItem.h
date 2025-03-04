// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractuableItem.generated.h"

class USkeletalMeshComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Initial UMETA(DisplayName = "Initial State"),
	EIS_ReadyToInteract UMETA(DisplayName = "Ready To Interact"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Dropped UMETA(DisplayName = "Dropped"),

	EIS_MAX UMETA(DisplayName = "Default Max")
};

UENUM(BlueprintType)
enum class EItemInteraction : uint8
{
	EII_Equip UMETA(DisplayName = "Equip"),
	EII_Save UMETA(DisplayName = "Save"),
	
	EII_NONE UMETA(DisplayName = "NONE")
};

UCLASS()
class ASTROSOLDIER_API AInteractuableItem : public AActor
{
	GENERATED_BODY()

public:

	AInteractuableItem();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE EItemInteraction GetItemInteraction() const {	return ItemInteraction;	}

	UFUNCTION()
	void SetPickUpWidgetVisibility(bool bWidgetIsVisible);

	UFUNCTION()
	void SetItemState(EItemState NewState);

	UFUNCTION()
	void OnRep_SetItemState(EItemState NewState);

protected:

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EItemInteraction ItemInteraction;	

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ItemMesh;	

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_SetItemState)
	EItemState ItemState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PickUpWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float InteractionRange = 15.f;

	UFUNCTION()
	void CheckForPlayerThatCanInteractWith();

	UFUNCTION()
	void SetItemBehaviour(EItemState BehaviorState);

};
