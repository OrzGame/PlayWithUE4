#pragma once

#include "Curves/CurveVector.h"
#include "Engine/DataTable.h"
#include "CharacterStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FComponentAndTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	FTransform Transform;

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	class UPrimitiveComponent* Component = nullptr;
};


USTRUCT(BlueprintType)
struct FMantleAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	UCurveVector* PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	FVector StartingOffset;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowPlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float LowStartPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighPlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float HighStartPosition = 0.0f;
};

USTRUCT(BlueprintType)
struct FMantleParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	UCurveVector* PositionCorrectionCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float StartingPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float PlayRate = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	FVector StartingOffset;
};

USTRUCT(BlueprintType)
struct FMantleTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float MaxLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float MinLedgeHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float ReachDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float ForwardTraceRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mantle System")
	float DownwardTraceRadius = 0.0f;
};