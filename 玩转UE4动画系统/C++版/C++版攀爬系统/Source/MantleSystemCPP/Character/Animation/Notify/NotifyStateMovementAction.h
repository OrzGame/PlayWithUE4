#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MantleSystemCPP/Library/CharacterEnumLibrary.h"
#include "NotifyStateMovementAction.generated.h"


UCLASS()
class MANTLESYSTEMCPP_API UNotifyStateMovementAction : public UAnimNotifyState
{
	GENERATED_BODY()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
							float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	EMovementAction MovementAction = EMovementAction::None;
};
