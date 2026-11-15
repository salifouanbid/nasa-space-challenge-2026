#include "AstraExplorerCharacter.h"
#include "AstraInteractableActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Math/RotationMatrix.h"

AAstraExplorerCharacter::AAstraExplorerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 500.0f;
    GetCharacterMovement()->AirControl = 0.30f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 350.0f;
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AAstraExplorerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AAstraExplorerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAstraExplorerCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAstraExplorerCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAstraExplorerCharacter::LookUp);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AAstraExplorerCharacter::Interact);
}

void AAstraExplorerCharacter::MoveForward(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AAstraExplorerCharacter::MoveRight(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
    }
}

void AAstraExplorerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AAstraExplorerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AAstraExplorerCharacter::Interact()
{
    if (!FollowCamera || !GetWorld())
    {
        return;
    }

    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * 600.0f;
    FHitResult Hit;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ASTRA_Interaction), false, this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
    {
        if (AAstraInteractableActor* Target = Cast<AAstraInteractableActor>(Hit.GetActor()))
        {
            Target->Interact(this);
        }
    }
}