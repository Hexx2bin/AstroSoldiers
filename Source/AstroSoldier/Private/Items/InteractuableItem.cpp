// © 2024 Anthony Caicedo. You can freely use the code of this project respecting the Terms and Conditions of the GNU V3 License.


#include "Items/InteractuableItem.h"
#include "Components/WidgetComponent.h"
#include "Character/AstroSoldierCharacter.h"
#include "Net/UnrealNetwork.h"

AInteractuableItem::AInteractuableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(GetRootComponent());

	ItemState = EItemState::EIS_Initial;
}

void AInteractuableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractuableItem, ItemState);
}

void AInteractuableItem::BeginPlay()
{
	Super::BeginPlay();

	PickUpWidget->SetVisibility(false);
	SetItemState(EItemState::EIS_Dropped);
}

void AInteractuableItem::SetPickUpWidgetVisibility(bool bWidgetIsVisible)
{
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(bWidgetIsVisible);
	}	
}

void AInteractuableItem::CheckForPlayerThatCanInteractWith()
{
	for(FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		TWeakObjectPtr<APlayerController> DummyController = *Iter;
		if(DummyController != nullptr)
		{
			if(AAstroSoldierCharacter* DummyCharacter = Cast<AAstroSoldierCharacter>(DummyController->GetCharacter()))
			{
				FVector Distance = DummyController->GetPawn()->GetActorLocation() - this->GetActorLocation();
/*#if WITH_EDITOR
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), Distance.Size()));
				}
#endif*/
				if(Distance.Size() <= InteractionRange)
				{
					if(DummyCharacter->GetInteractuableItem() == nullptr)
					{
						DummyCharacter->SetInteractuableItem(this);
					}						
				}
				else
				{
					if(DummyCharacter->GetInteractuableItem() != nullptr)
					{
					DummyCharacter->SetInteractuableItem(nullptr);
					}
				}
			}			
		}
	}
}

void AInteractuableItem::SetItemBehaviour(EItemState BehaviorState)
{
	switch (BehaviorState)
	{
	case EItemState::EIS_Initial:
		break;
	case EItemState::EIS_ReadyToInteract:
		break;
	case EItemState::EIS_Equipped:
		SetPickUpWidgetVisibility(false);
		break;
	case EItemState::EIS_Dropped:
		break;
	case EItemState::EIS_MAX:
		break;
	}
}

void AInteractuableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority() && ItemState == EItemState::EIS_Dropped) CheckForPlayerThatCanInteractWith();		
}

void AInteractuableItem::SetItemState(EItemState NewState)
{	
	ItemState = NewState;
	SetItemBehaviour(ItemState);
}

void AInteractuableItem::OnRep_SetItemState(EItemState NewState)
{
	SetItemBehaviour(ItemState);
}