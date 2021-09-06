// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MantleSystemCPP/Library/CharacterEnumLibrary.h"
#include "MantleCharacter.generated.h"


class UTimelineComponent;
class UAnimInstance;
class UAnimMontage;
class UCharacterAnimInstance;
enum class EVisibilityBasedAnimTickOption : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpPressedSignature);

UCLASS()
class MANTLESYSTEMCPP_API AMantleCharacter : public ACharacter
{
	GENERATED_BODY()

	public:
	AMantleCharacter();

	protected:
	virtual void BeginPlay() override;

	public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PreInitializeComponents() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay System")
	void ClearHeldObject();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay System")
	void UpdateHeldObject();
	
	UFUNCTION(BlueprintGetter, Category = "Character States")
	EOverlayState GetOverlayState() const { return EOverlayState::Default; }

	UFUNCTION(BlueprintCallable, Category = "Utility")
	UCharacterAnimInstance* GetMainAnimInstance() { return MainAnimInstance; }
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FJumpPressedSignature JumpPressedDelegate;
	/** State Changes */

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	void SetEssentialValues(float DeltaTime);


	/** Character States */
	
	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementAction(EMovementAction NewAction);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EMovementAction GetMovementAction() const { return MovementAction; }

	UFUNCTION(BlueprintCallable, Category = "Character States")
	void SetMovementState(EMovementState NewState);

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "Character States")
	EMovementState GetPrevMovementState() const { return PrevMovementState; }


	UFUNCTION(BlueprintGetter, Category = "Essential Information")
	float GetSpeed() const { return Speed; }

	/** State Values */

	protected:

	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EMovementState MovementState = EMovementState::None;
	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EMovementState PrevMovementState = EMovementState::None;
	UPROPERTY(BlueprintReadOnly, Category = "State Values")
	EMovementAction MovementAction = EMovementAction::None;
	UPROPERTY(BlueprintReadOnly, Category = "Utility")
	UCharacterAnimInstance* MainAnimInstance = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Information")
	float Speed = 0.0f;


	public:




	protected:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	
	UFUNCTION(BlueprintCallable, Category = "Player Input")
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Player Input")
	void JumpPressedAction();
	UFUNCTION(BlueprintCallable, Category = "Player Input")
	void JumpReleasedAction();
};
