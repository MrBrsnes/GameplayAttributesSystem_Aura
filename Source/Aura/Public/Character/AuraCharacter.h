// Copyright CCC Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

	// Init Ability Actor Info
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:

	virtual void InitAbilityActorInfo() override;
};
