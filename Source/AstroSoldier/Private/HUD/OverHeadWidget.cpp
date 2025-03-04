// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.


#include "HUD/OverHeadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadWidget::SetDisplayText(FString TextToDisplay)
{
	DisplayText->SetText(FText::FromString(TextToDisplay));
}

void UOverHeadWidget::ShowPlayerNetRole(const APawn* InPawn)
{
	if(APlayerState* PlayerState = GetOwningPlayerState())
	{
		SetDisplayText(FString::Printf(TEXT("%s"), *PlayerState->GetPlayerName()));
	}
}

void UOverHeadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
