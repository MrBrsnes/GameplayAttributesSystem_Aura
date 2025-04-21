// Copyright CCC Studios


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	* Line Trace from cursor. There are several scenarios:
	* A. LastActor is null && ThisActor is null
	*	-> No actor under cursor
	* B. LastActor is null && ThisActor is valid
	*	-> New actor under cursor (Highlight ThisActor)
	* C. LastActor is valid && ThisActor is null
	*	-> No actor under cursor (Unhighlight LastActor)
	* D. Both actors are valid, but LastActor != ThisActor
	*	-> Unhighlight LastActor and Highlight ThisActor
	* E. Both actors are valid, and LastActor == ThisActor
	*	-> Do nothing
	*/

	if (LastActor == nullptr && ThisActor == nullptr) return; // A
	
	if (LastActor == nullptr && ThisActor != nullptr) // B
	{
		if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(ThisActor.GetObject()))
		{
			Enemy->HighlightActor();
		}
	}

	if (LastActor != nullptr && ThisActor == nullptr) // C
	{
		if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(LastActor.GetObject()))
		{
			Enemy->UnHighlightActor();
		}
	}

	if (LastActor != nullptr && ThisActor != nullptr && LastActor != ThisActor) // D
	{
		if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(LastActor.GetObject()))
		{
			Enemy->UnHighlightActor();
		}
		if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(ThisActor.GetObject()))
		{
			Enemy->HighlightActor();
		}
	}

	if (LastActor != nullptr && ThisActor != nullptr && LastActor == ThisActor) // E
	{
		return;
	}
	

}

void AAuraPlayerController::BeginPlay()

{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Setup Input Component
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Bind Input Actions
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector Direction = (ForwardDirection * InputAxisVector.Y) + (RightDirection * InputAxisVector.X);
		ControlledPawn->AddMovementInput(Direction);
	}
}
