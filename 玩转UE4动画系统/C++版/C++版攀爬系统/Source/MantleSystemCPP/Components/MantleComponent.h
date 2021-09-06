// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MantleSystemCPP/Library/CharacterStructLibrary.h"
#include "MantleSystemCPP/Library/CharacterEnumLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MantleComponent.generated.h"


UCLASS(Blueprintable, BlueprintType)
class MANTLESYSTEMCPP_API UMantleComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
	// Sets default values for this component's properties
	UMantleComponent();
	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	bool MantleCheck(const FMantleTraceSettings& TraceSettings,
						EDrawDebugTrace::Type DebugType);

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	void MantleStart(float MantleHeight, const FComponentAndTransform& MantleLedgeWS,
					EMantleType MantleType);

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	void MantleUpdate(float BlendIn);

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	void MantleEnd();

	UFUNCTION(BlueprintCallable, Category = "Mantle System")
	void OnOwnerJumpInput();
	

	/** Implement on BP to get correct mantle parameter set according to character state */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mantle System")
	struct FMantleAsset GetMantleAsset(EMantleType MantleType, EOverlayState CurrentOverlayState);


	protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantle System")
	class UTimelineComponent* MantleTimeline = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FMantleTraceSettings GroundedTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FMantleTraceSettings AutomaticTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FMantleTraceSettings FallingTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	UCurveFloat* MantleTimelineCurve;

	
	static FName NAME_IgnoreOnlyPawn;
	/** Profile to use to detect objects we allow mantling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	FName MantleObjectDetectionProfile = NAME_IgnoreOnlyPawn;

	
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	TEnumAsByte<ECollisionChannel> WalkableSurfaceDetectionChannel = ECC_Visibility;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FMantleParams MantleParams;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FComponentAndTransform MantleLedgeLS;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FTransform MantleTarget = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FTransform MantleActualStartOffset = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category = "Mantle System")
	FTransform MantleAnimatedStartOffset = FTransform::Identity;

	/** If a dynamic object has a velocity bigger than this value, do not start mantle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mantle System")
	float AcceptableVelocityWhileMantling = 10.0f;

	private:
	UPROPERTY()
	class AMantleCharacter* OwnerCharacter = nullptr;
	
};
