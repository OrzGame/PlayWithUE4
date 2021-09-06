#include "NotifyStateMovementAction.h"
#include "MantleSystemCPP/Character/MantleCharacter.h"


void UNotifyStateMovementAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	AMantleCharacter* Character = Cast<AMantleCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetMovementAction(MovementAction);
	}
}

void UNotifyStateMovementAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMantleCharacter* Character = Cast<AMantleCharacter>(MeshComp->GetOwner());
	if (Character && Character->GetMovementAction() == MovementAction)
	{
		Character->SetMovementAction(EMovementAction::None);
	}
}

FString UNotifyStateMovementAction::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Movement Action: "));
	Name.Append(GetEnumerationToString(MovementAction));
	return Name;
}
