// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "OverHeadWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ASTROSOLDIER_API UOverHeadWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = HUD, meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(const APawn* InPawn);

	void SetDisplayText(FString TextToDisplay);	

protected:

	virtual void NativeDestruct() override;
	
	
};
