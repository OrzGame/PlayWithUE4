#include "CharacterAnimInstance.h"
#include "MantleSystemCPP/Character/MantleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AMantleCharacter>(TryGetPawnOwner());
}



