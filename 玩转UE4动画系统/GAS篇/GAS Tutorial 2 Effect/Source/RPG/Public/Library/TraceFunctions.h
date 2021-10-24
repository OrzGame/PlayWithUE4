// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "TraceFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UTraceFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Trace Functions")
	static void TraceForTarget(const AActor* Actor,const float Distance,const EDrawDebugTrace::Type DrawDebugTrace,bool& Detected,ARPGCharacterBase*& Target);
};
