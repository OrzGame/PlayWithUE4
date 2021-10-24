#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGCharacterBase.generated.h"

UCLASS()
class ARPGCharacterBase  : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARPGCharacterBase();

	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Character Abilities")
	class UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Character Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> PreloadedAbilities;
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
