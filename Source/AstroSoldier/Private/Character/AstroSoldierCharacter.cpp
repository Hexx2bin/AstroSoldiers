// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#include "Character/AstroSoldierCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
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
	}
}



