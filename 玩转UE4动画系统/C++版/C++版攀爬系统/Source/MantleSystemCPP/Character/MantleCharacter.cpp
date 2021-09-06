#include "MantleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MantleSystemCPP/Character/Animation/CharacterAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMantleCharacter::AMantleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}

void AMantleCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMantleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetEssentialValues(DeltaTime);
}

void AMantleCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	MainAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MainAnimInstance)
	{
		checkf(GetWorld()->WorldType == EWorldType::EditorPreview,
			TEXT("%s doesn't have a valid animation instance assigned. That's not allowed"),
			*GetName());
	}
	
}

void AMantleCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (GetCharacterMovement()->MovementMode == MOVE_Walking ||
		GetCharacterMovement()->MovementMode == MOVE_NavWalking)
	{
		SetMovementState(EMovementState::Grounded);
	}
	else if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		SetMovementState(EMovementState::InAir);
	}
}

void AMantleCharacter::SetMovementState(EMovementState NewState)
{
	if (MovementState != NewState)
	{
		PrevMovementState = MovementState;
		MovementState = NewState;
		FAnimCharacterInformation& AnimData = MainAnimInstance->GetCharacterInformationMutable();
		AnimData.PrevMovementState = PrevMovementState;
		MainAnimInstance->MovementState = MovementState;
	}
}

void AMantleCharacter::MoveRight(float Value)
{
	if (MovementAction == EMovementAction::None)
	{
		AddMovementInput(FVector(0.f,-1.f,0.f), Value);
	}
}

void AMantleCharacter::JumpPressedAction()
{
	if (JumpPressedDelegate.IsBound())
	{
		JumpPressedDelegate.Broadcast();
	}

	if (MovementAction == EMovementAction::None)
	{
		if (MovementState == EMovementState::Grounded)
		{
			Jump();
		}
	}
}

void AMantleCharacter::JumpReleasedAction()
{
	StopJumping();
}


void AMantleCharacter::SetEssentialValues(float DeltaTime)
{
	MainAnimInstance->GetCharacterInformationMutable().Speed = Speed = GetVelocity().Size2D();
}

void AMantleCharacter::SetMovementAction(const EMovementAction NewAction)
{
	if (MovementAction != NewAction)
	{
		MainAnimInstance->MovementAction = MovementAction = NewAction;
	}
}

