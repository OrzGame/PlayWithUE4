#include "MathLibrary.h"
#include "Components/CapsuleComponent.h"

FTransform UMathLibrary::MantleComponentLocalToWorld(const FComponentAndTransform& CompAndTransform)
{
	const FTransform& InverseTransform = CompAndTransform.Component->GetComponentToWorld().Inverse();
	const FVector Location = InverseTransform.InverseTransformPosition(CompAndTransform.Transform.GetLocation());
	const FQuat Quat = InverseTransform.InverseTransformRotation(CompAndTransform.Transform.GetRotation());
	const FVector Scale = InverseTransform.InverseTransformPosition(CompAndTransform.Transform.GetScale3D());
	return {Quat, Location, Scale};
}

FVector UMathLibrary::GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentLocation() -
	Capsule->GetUpVector() * (Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
}

FVector UMathLibrary::GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule)
{
	BaseLocation.Z += Capsule->GetScaledCapsuleHalfHeight() + ZOffset;
	return BaseLocation;
}

bool UMathLibrary::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
	float RadiusOffset)
{
	// Perform a trace to see if the capsule has room to be at the target location.
	const float ZTarget = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() - RadiusOffset + HeightOffset;
	FVector TraceStart = TargetLocation;
	TraceStart.Z += ZTarget;
	FVector TraceEnd = TargetLocation;
	TraceEnd.Z -= ZTarget;
	const float Radius = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;

	const UWorld* World = Capsule->GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Capsule->GetOwner());

	FHitResult HitResult;
	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(Radius);
	const bool bHit = World->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity,
												ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);
	
	return !(HitResult.bBlockingHit || HitResult.bStartPenetrating);
}
