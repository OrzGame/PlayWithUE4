#pragma once

#include "CoreMinimal.h"
#include "MantleSystemCPP/Library/CharacterStructLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MathLibrary.generated.h"


class UCapsuleComponent;

UCLASS()
class MANTLESYSTEMCPP_API UMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, Category = "Math Utils")
	static FTransform MantleComponentLocalToWorld(const FComponentAndTransform& CompAndTransform);


	UFUNCTION(BlueprintCallable, Category = "Math Utils")
	static FTransform TransfromSub(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() - T2.GetRotation().Rotator(),
						T1.GetLocation() - T2.GetLocation(), T1.GetScale3D() - T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "Math Utils")
	static FTransform TransfromAdd(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() + T2.GetRotation().Rotator(),
						T1.GetLocation() + T2.GetLocation(), T1.GetScale3D() + T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "Math Utils")
	static FVector GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "Math Utils")
	static FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "Math Utils")
	static bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
									float RadiusOffset);
	
	
};
