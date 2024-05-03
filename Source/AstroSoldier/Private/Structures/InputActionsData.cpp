// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.

#include "Structures/InputActionsData.h"

#if WITH_EDITOR

void UInputActionsData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	TArray<UInputAction*> InputActions;
	DataActions.GenerateValueArray(InputActions);
	DataActions.Reset();

	for(UInputAction* Action : InputActions)
	{
		if(IsValid(Action))
		{
			DataActions.Emplace(Action->GetName(), Action);
		}
		else
		{
			DataActions.Emplace(TEXT(""), nullptr);
		}
	}
}

#endif