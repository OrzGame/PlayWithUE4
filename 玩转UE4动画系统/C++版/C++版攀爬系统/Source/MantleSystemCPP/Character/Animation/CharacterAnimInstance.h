#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MantleSystemCPP/Library/CharacterEnumLibrary.h"
#include "MantleSystemCPP/Library/AnimationStructLibrary.h"
#include "CharacterAnimInstance.generated.h"

class AMantleCharacter;

UCLASS(Blueprintable, BlueprintType)
class MANTLESYSTEMCPP_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	protected:

	virtual void NativeInitializeAnimation() override;

	public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	EMovementState MovementState = EMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	EMovementAction MovementAction = EMovementAction::None;

	protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character Information")
	AMantleCharacter* Character = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information", Meta = (
		ShowOnlyInnerProperties))
	FAnimCharacterInformation CharacterInformation;
	
	public:
	FAnimCharacterInformation& GetCharacterInformationMutable()
	{
		return CharacterInformation;
	}
};
