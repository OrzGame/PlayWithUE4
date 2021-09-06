#pragma once

#include "CoreMinimal.h"
#include "CharacterEnumLibrary.h"


#include "AnimationStructLibrary.generated.h"


USTRUCT(BlueprintType)
struct FAnimCharacterInformation
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FRotator CharacterActorRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector RelativeVelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector Acceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	FVector MovementInput;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	bool bHasMovementInput = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	float Speed = 0.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	float MovementInputAmount = 0.0f;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character Information")
	EMovementState PrevMovementState = EMovementState::None;
	
};
