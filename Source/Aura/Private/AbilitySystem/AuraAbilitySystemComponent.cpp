// Copyright CCC Studios


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& Effectspec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied"));
}
