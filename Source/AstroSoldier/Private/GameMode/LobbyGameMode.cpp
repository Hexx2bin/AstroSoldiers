// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.


#include "GameMode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if(NumberOfPlayers == 2)
	{
		TObjectPtr<UWorld> World = GetWorld();
		if(World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/_Game/Maps/AstroMap?listen"));
		}
	}
}
