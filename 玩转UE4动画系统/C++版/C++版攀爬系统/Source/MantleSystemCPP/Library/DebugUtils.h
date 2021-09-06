// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DebugUtils.generated.h"


UCLASS()
class MANTLESYSTEMCPP_API UDebugUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	static void DrawDebugLineTraceSingle(const UWorld* World,
										const FVector& Start,
										const FVector& End,
										EDrawDebugTrace::Type DrawDebugType,
										bool bHit,
										const FHitResult& OutHit,
										FLinearColor TraceColor,
										FLinearColor TraceHitColor,
										float DrawTime);

	static void DrawDebugCapsuleTraceSingle(const UWorld* World,
											const FVector& Start,
											const FVector& End,
											const FCollisionShape& CollisionShape,
											EDrawDebugTrace::Type DrawDebugType,
											bool bHit,
											const FHitResult& OutHit,
											FLinearColor TraceColor,
											FLinearColor TraceHitColor,
											float DrawTime);

	static void DrawDebugSphereTraceSingle(const UWorld* World,
										const FVector& Start,
										const FVector& End,
										const FCollisionShape& CollisionShape,
										EDrawDebugTrace::Type DrawDebugType,
										bool bHit,
										const FHitResult& OutHit,
										FLinearColor TraceColor,
										FLinearColor TraceHitColor,
										float DrawTime);
	static void DrawDebugSweptSphere(const UWorld* InWorld,
							FVector const& Start,
							FVector const& End,
							float Radius,
							FColor const& Color,
							bool bPersistentLines = false,
							float LifeTime = -1.f,
							uint8 DepthPriority = 0);
};
