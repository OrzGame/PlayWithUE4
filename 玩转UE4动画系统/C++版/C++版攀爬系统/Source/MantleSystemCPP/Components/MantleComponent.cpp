#include "MantleComponent.h"

#include "Animation/AnimInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "MantleSystemCPP/Library/MathLibrary.h"
#include "MantleSystemCPP/Character/MantleCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "MantleSystemCPP/Character/Animation/CharacterAnimInstance.h"
#include "MantleSystemCPP/Library/DebugUtils.h"


const FName NAME_MantleEnd(TEXT("MantleEnd"));
const FName NAME_MantleUpdate(TEXT("MantleUpdate"));
const FName NAME_MantleTimeline(TEXT("MantleTimeline"));

FName UMantleComponent::NAME_IgnoreOnlyPawn(TEXT("IgnoreOnlyPawn"));


UMantleComponent::UMantleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	MantleTimeline = CreateDefaultSubobject<UTimelineComponent>(NAME_MantleTimeline);

}

void UMantleComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		OwnerCharacter = Cast<AMantleCharacter>(GetOwner());
		if (OwnerCharacter)
		{
			AddTickPrerequisiteActor(OwnerCharacter);
			
			FOnTimelineFloat TimelineUpdated;
			FOnTimelineEvent TimelineFinished;
			TimelineUpdated.BindUFunction(this, NAME_MantleUpdate);
			TimelineFinished.BindUFunction(this, NAME_MantleEnd);
			MantleTimeline->SetTimelineFinishedFunc(TimelineFinished);
			MantleTimeline->SetLooping(false);
			MantleTimeline->SetTimelineLengthMode(TL_TimelineLength);
			MantleTimeline->AddInterpFloat(MantleTimelineCurve, TimelineUpdated);
			OwnerCharacter->JumpPressedDelegate.AddUniqueDynamic(this, &UMantleComponent::OnOwnerJumpInput);
		}
	}
	
}


void UMantleComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (OwnerCharacter && OwnerCharacter->GetMovementState() == EMovementState::InAir)
	{
		MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForDuration);
	}
	
}



bool UMantleComponent::MantleCheck(const FMantleTraceSettings& TraceSettings, EDrawDebugTrace::Type DebugType)
{
	
	if (!OwnerCharacter)
	{
		return false;
	}

	// Step 1
	const FVector& TraceDirection = OwnerCharacter->GetActorForwardVector();
	const FVector& CapsuleBaseLocation = UMathLibrary::GetCapsuleBaseLocation(
		2.0f, OwnerCharacter->GetCapsuleComponent());
	FVector TraceStart = CapsuleBaseLocation + TraceDirection * -30.0f;
	TraceStart.Z += (TraceSettings.MaxLedgeHeight + TraceSettings.MinLedgeHeight) / 2.0f;
	const FVector TraceEnd = TraceStart + TraceDirection * TraceSettings.ReachDistance;
	const float HalfHeight = 1.0f + (TraceSettings.MaxLedgeHeight - TraceSettings.MinLedgeHeight) / 2.0f;

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	FHitResult HitResult;
	{
		const FCollisionShape CapsuleCollisionShape = FCollisionShape::MakeCapsule(TraceSettings.ForwardTraceRadius, HalfHeight);
		const bool bHit = World->SweepSingleByProfile(HitResult, TraceStart, TraceEnd, FQuat::Identity, MantleObjectDetectionProfile,
	                                                  CapsuleCollisionShape, Params);
		{
			UDebugUtils::DrawDebugCapsuleTraceSingle(World,
															TraceStart,
															TraceEnd,
															CapsuleCollisionShape,
															DebugType,
															bHit,
															HitResult,
															FLinearColor::Green,
															FLinearColor::Red,
															1.0f);
		}
	}

	if (!HitResult.IsValidBlockingHit() || OwnerCharacter->GetCharacterMovement()->IsWalkable(HitResult))
	{
		return false;
	}

	if (HitResult.GetComponent() != nullptr)
	{
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		if (PrimitiveComponent && PrimitiveComponent->GetComponentVelocity().Size() > AcceptableVelocityWhileMantling)
		{
			return false;
		}
	}

	const FVector InitialTraceImpactPoint = HitResult.ImpactPoint;
	const FVector InitialTraceNormal = HitResult.ImpactNormal;

	// Step 2
	FVector DownwardTraceEnd = InitialTraceImpactPoint;
	DownwardTraceEnd.Z = CapsuleBaseLocation.Z;
	DownwardTraceEnd += InitialTraceNormal * -15.0f;
	FVector DownwardTraceStart = DownwardTraceEnd;
	DownwardTraceStart.Z += TraceSettings.MaxLedgeHeight + TraceSettings.DownwardTraceRadius + 1.0f;

	{
		const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceSettings.DownwardTraceRadius);
		const bool bHit = World->SweepSingleByChannel(HitResult, DownwardTraceStart, DownwardTraceEnd, FQuat::Identity,
	                                                  WalkableSurfaceDetectionChannel, SphereCollisionShape,
	                                                  Params);

		{
			UDebugUtils::DrawDebugSphereTraceSingle(World,
														TraceStart,
														TraceEnd,
														SphereCollisionShape,
														DebugType,
														bHit,
														HitResult,
														FLinearColor::Green,
														FLinearColor::Red,
														1.0f);
		}
	}


	if (!OwnerCharacter->GetCharacterMovement()->IsWalkable(HitResult))
	{
		return false;
	}

	const FVector DownTraceLocation(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();

	// Step 3
	const FVector& CapsuleLocationFBase = UMathLibrary::GetCapsuleLocationFromBase(
		DownTraceLocation, 2.0f, OwnerCharacter->GetCapsuleComponent());
	const bool bCapsuleHasRoom = UMathLibrary::CapsuleHasRoomCheck(OwnerCharacter->GetCapsuleComponent(),
	                                                                  CapsuleLocationFBase, 0.0f,
	                                                                  0.0f);

	if (!bCapsuleHasRoom)
	{
		return false;
	}

	const FTransform TargetTransform(
		(InitialTraceNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator(),
		CapsuleLocationFBase,
		FVector::OneVector);

	const float MantleHeight = (CapsuleLocationFBase - OwnerCharacter->GetActorLocation()).Z;

	// Step 4
	EMantleType MantleType;
	if (OwnerCharacter->GetMovementState() == EMovementState::InAir)
	{
		MantleType = EMantleType::FallingCatch;
	}
	else
	{
		MantleType = MantleHeight > 125.0f ? EMantleType::HighMantle : EMantleType::LowMantle;
	}

	// Step 5
	FComponentAndTransform MantleWS;
	MantleWS.Component = HitComponent;
	MantleWS.Transform = TargetTransform;
	MantleStart(MantleHeight, MantleWS, MantleType);
	return true;
}

void UMantleComponent::MantleStart(float MantleHeight, const FComponentAndTransform& MantleLedgeWS,
	EMantleType MantleType)
{
	if (OwnerCharacter == nullptr || !IsValid(MantleLedgeWS.Component) || !IsValid(MantleTimeline))
	{
		return;
	}

	if (MantleType != EMantleType::LowMantle && OwnerCharacter->IsA(AMantleCharacter::StaticClass()))
	{
		Cast<AMantleCharacter>(OwnerCharacter)->ClearHeldObject();
	}
	
	SetComponentTickEnabledAsync(false);

	// Step 1
	const FMantleAsset MantleAsset = GetMantleAsset(MantleType, OwnerCharacter->GetOverlayState());
	check(MantleAsset.PositionCorrectionCurve)

	MantleParams.AnimMontage = MantleAsset.AnimMontage;
	MantleParams.PositionCorrectionCurve = MantleAsset.PositionCorrectionCurve;
	MantleParams.StartingOffset = MantleAsset.StartingOffset;
	MantleParams.StartingPosition = FMath::GetMappedRangeValueClamped({MantleAsset.LowHeight, MantleAsset.HighHeight},
	                                                                  {
		                                                                  MantleAsset.LowStartPosition,
		                                                                  MantleAsset.HighStartPosition
	                                                                  },
	                                                                  MantleHeight);
	MantleParams.PlayRate = FMath::GetMappedRangeValueClamped({MantleAsset.LowHeight, MantleAsset.HighHeight},
	                                                          {MantleAsset.LowPlayRate, MantleAsset.HighPlayRate},
	                                                          MantleHeight);

	// Step 2
	MantleLedgeLS.Component = MantleLedgeWS.Component;
	MantleLedgeLS.Transform = MantleLedgeWS.Transform * MantleLedgeWS.Component->GetComponentToWorld().Inverse();

	// Step 3
	MantleTarget = MantleLedgeWS.Transform;
	MantleActualStartOffset = UMathLibrary::TransfromSub(OwnerCharacter->GetActorTransform(), MantleTarget);

	// Step 4
	FVector RotatedVector = MantleTarget.GetRotation().Vector() * MantleParams.StartingOffset.Y;
	RotatedVector.Z = MantleParams.StartingOffset.Z;
	const FTransform StartOffset(MantleTarget.Rotator(), MantleTarget.GetLocation() - RotatedVector,
	                             FVector::OneVector);
	MantleAnimatedStartOffset = UMathLibrary::TransfromSub(StartOffset, MantleTarget);

	// Step 5
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
	OwnerCharacter->SetMovementState(EMovementState::Mantling);

	// Step 6
	float MinTime = 0.0f;
	float MaxTime = 0.0f;
	MantleParams.PositionCorrectionCurve->GetTimeRange(MinTime, MaxTime);
	MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
	MantleTimeline->SetPlayRate(MantleParams.PlayRate);
	MantleTimeline->PlayFromStart();

	// Step 7
	if (IsValid(MantleParams.AnimMontage))
	{
		OwnerCharacter->GetMainAnimInstance()->Montage_Play(MantleParams.AnimMontage, MantleParams.PlayRate,
		                                                    EMontagePlayReturnType::MontageLength,
		                                                    MantleParams.StartingPosition, false);
	}
}

void UMantleComponent::MantleUpdate(float BlendIn)
{
	if (!OwnerCharacter)
	{
		return;
	}

	// Step 1
	MantleTarget = UMathLibrary::MantleComponentLocalToWorld(MantleLedgeLS);

	// Step 2
	const FVector CurveVec = MantleParams.PositionCorrectionCurve
	                                     ->GetVectorValue(
		                                     MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
	const float PositionAlpha = CurveVec.X;
	const float XYCorrectionAlpha = CurveVec.Y;
	const float ZCorrectionAlpha = CurveVec.Z;

	// Step 3
	const FTransform TargetHzTransform(MantleAnimatedStartOffset.GetRotation(),
	                                   {
		                                   MantleAnimatedStartOffset.GetLocation().X,
		                                   MantleAnimatedStartOffset.GetLocation().Y,
		                                   MantleActualStartOffset.GetLocation().Z
	                                   },
	                                   FVector::OneVector);
	const FTransform& HzLerpResult =
		UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetHzTransform, XYCorrectionAlpha);
	
	const FTransform TargetVtTransform(MantleActualStartOffset.GetRotation(),
	                                   {
		                                   MantleActualStartOffset.GetLocation().X,
		                                   MantleActualStartOffset.GetLocation().Y,
		                                   MantleAnimatedStartOffset.GetLocation().Z
	                                   },
	                                   FVector::OneVector);
	const FTransform& VtLerpResult =
		UKismetMathLibrary::TLerp(MantleActualStartOffset, TargetVtTransform, ZCorrectionAlpha);

	const FTransform ResultTransform(HzLerpResult.GetRotation(),
	                                 {
		                                 HzLerpResult.GetLocation().X, HzLerpResult.GetLocation().Y,
		                                 VtLerpResult.GetLocation().Z
	                                 },
	                                 FVector::OneVector);

	const FTransform& ResultLerp = UKismetMathLibrary::TLerp(
		UMathLibrary::TransfromAdd(MantleTarget, ResultTransform), MantleTarget,
		PositionAlpha);

	const FTransform& LerpedTarget =
		UKismetMathLibrary::TLerp(UMathLibrary::TransfromAdd(MantleTarget, MantleActualStartOffset), ResultLerp,
		                          BlendIn);

	// Step 4
	OwnerCharacter->SetActorLocationAndRotation(LerpedTarget.GetLocation(), LerpedTarget.GetRotation().Rotator());
}

void UMantleComponent::MantleEnd()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		if (OwnerCharacter->IsA(AMantleCharacter::StaticClass()))
		{
			Cast<AMantleCharacter>(OwnerCharacter)->UpdateHeldObject();
		}
	}
	SetComponentTickEnabledAsync(true);
}

void UMantleComponent::OnOwnerJumpInput()
{
	if (OwnerCharacter && OwnerCharacter->GetMovementAction() == EMovementAction::None)
	{
		if (OwnerCharacter->GetMovementState() == EMovementState::Grounded)
		{
			MantleCheck(GroundedTraceSettings, EDrawDebugTrace::Type::ForDuration);
		}
		else if (OwnerCharacter->GetMovementState() == EMovementState::InAir)
		{
			MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForDuration);
		}
	}
}




