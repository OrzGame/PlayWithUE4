#include "RPG/Public/RPGCharacterBase.h"
#include "AbilitySystemComponent.h"


ARPGCharacterBase::ARPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}


UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if(AbilitySystemComponent != nullptr)
	{
		if(PreloadedAbilities.Num() > 0)
		{
			for(auto i =0;i < PreloadedAbilities.Num(); i++)
			{
				if(PreloadedAbilities[i] != nullptr)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(PreloadedAbilities[i].GetDefaultObject(),1,0));
				}
			}
			
		}

		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
}

