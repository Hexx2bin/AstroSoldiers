// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#include "Character/AstroSoldierCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AstroComponents/CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"
#include "Structures/InputActionsData.h"

#if WITH_EDITOR
#include "PropertyEditorModule.h"
#endif

AAstroSoldierCharacter::AAstroSoldierCharacter()
{
#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "Custom Detail"
	static const FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);

	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Character", "AstroSoldier", LOCTEXT("AstroSoldier", "AstroSoldier"));
	Section->AddCategory("Elements");
#undef LOCTEXT_NAMESPACE
#endif	
	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(GetMesh());

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
}

void AAstroSoldierCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AAstroSoldierCharacter, InteractuableItem, COND_OwnerOnly);
}

void AAstroSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(IsValid(Combat))
	{
		Combat->AstroCharacter = this;
	}
}

void AAstroSoldierCharacter::SetInteractuableItem(AInteractuableItem* NewInteractuableItem)
{
	if(IsLocallyControlled())
	{
		if(NewInteractuableItem != nullptr)
		{
			NewInteractuableItem->SetPickUpWidgetVisibility(true);
		}
		if(InteractuableItem != nullptr)
		{
			InteractuableItem->SetPickUpWidgetVisibility(false);
		}
	}	
	InteractuableItem = NewInteractuableItem;
}

void AAstroSoldierCharacter::OnRep_SetInteractuableItem(AInteractuableItem* NewInteractuableItem)
{
/*#if WITH_EDITOR
	FString Message = NewInteractuableWeapon != nullptr ? FString::Printf(TEXT("NewInteractuableWeapon no null")) : FString::Printf(TEXT("NewInteractuableWeapon is null"));
	FString Message1 = InteractuableWeapon != nullptr ? FString::Printf(TEXT("InteractuableWeapon no null")) : FString::Printf(TEXT("InteractuableWeapon is null"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, Message);
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, Message1);
	}
#endif*/
	if(NewInteractuableItem != nullptr)
	{
		NewInteractuableItem->SetPickUpWidgetVisibility(false);
	}
	if(InteractuableItem != nullptr)
	{
		InteractuableItem->SetPickUpWidgetVisibility(true);
	}
}

void AAstroSoldierCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if(const TObjectPtr<APlayerController> PC = Cast<APlayerController>(GetController()))
	{
		if(const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(GameplayMappingContext, 0);
		}
	}
}

void AAstroSoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAstroSoldierCharacter::Move(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Vector x: %f, y: %f"), ActionValue.Get<FVector2D>().X, ActionValue.Get<FVector2D>().Y);

	if(IsValid(Controller))
	{
		//Find out wich way is forward
		const FVector2d MoveValue { ActionValue.Get<FVector2d>() };
		const FRotator MovementRotator { 0.f, Controller->GetControlRotation().Yaw, 0.f};

		//Forward/Backward direction
		if(MoveValue.Y != 0.f)
		{
			const FVector Direction { MovementRotator.RotateVector(FVector::ForwardVector)};
			AddMovementInput(Direction, MoveValue.Y);
		}

		//Right/Left direction
		if(MoveValue.X != 0.f)
		{
			const FVector Direction { MovementRotator.RotateVector(FVector::RightVector)};
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void AAstroSoldierCharacter::Rotate(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Rotate"));
	if(IsValid(Controller))
	{
		const FVector2D RotateValue { ActionValue.Get<FVector2D>()};
		if(RotateValue.X != 0.f)
		{
			AddControllerYawInput(RotateValue.X * HipTurnRate);
		}
		if(RotateValue.Y != 0.f)
		{
			AddControllerPitchInput(RotateValue.Y * LookUpTurnRate);
		}
	}
}

void AAstroSoldierCharacter::Interact(const FInputActionValue& ActionValue)
{
	if(IsValid(InteractuableItem) && IsValid(Combat))
	{
		if(HasAuthority())
		{
			switch (InteractuableItem->GetItemInteraction())
			{
			case EItemInteraction::EII_Equip:
				Combat->EquipWeapon(Cast<AWeapon>(InteractuableItem));
				break;
			case EItemInteraction::EII_Save:
				break;
			case EItemInteraction::EII_NONE:
				break;
			default: ;
			}
		}
		else
		{
			ServerInteract();
		}
	}
}

void AAstroSoldierCharacter::Crouch(const FInputActionValue& ActionValue)
{
	
}

void AAstroSoldierCharacter::ServerInteract_Implementation()
{
	if(IsValid(InteractuableItem) && IsValid(Combat))
	{
		switch (InteractuableItem->GetItemInteraction())
		{
		case EItemInteraction::EII_Equip:
			Combat->EquipWeapon(Cast<AWeapon>(InteractuableItem));
			break;
		case EItemInteraction::EII_Save:
			break;
		case EItemInteraction::EII_NONE:
			break;
		default: ;
		}
	}
}

void AAstroSoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAstroSoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	if(const TSoftObjectPtr<UEnhancedInputComponent> PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnhancedInputComponent->BindAction(IADataList->DataActions[TEXT("IA_Move")],ETriggerEvent::Triggered, this, &ThisClass::Move);
		PlayerEnhancedInputComponent->BindAction(IADataList->DataActions[TEXT("IA_Rotate")],ETriggerEvent::Triggered, this, &ThisClass::Rotate);
		PlayerEnhancedInputComponent->BindAction(IADataList->DataActions[TEXT("IA_Interact")],ETriggerEvent::Triggered, this, &ThisClass::Interact);
		PlayerEnhancedInputComponent->BindAction(IADataList->DataActions[TEXT("IA_Crouch")],ETriggerEvent::Triggered, this, &ThisClass::Crouch);
	}
}