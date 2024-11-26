// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"


ACOAAvatar::ACOAAvatar()
{
    mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    mSpringArm->TargetArmLength = 300.0f;
    mSpringArm->SetupAttachment(RootComponent);

    mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

    mCamera->bUsePawnControlRotation = true;
    mSpringArm->bUsePawnControlRotation = true;
    bUseControllerRotationYaw = false;
}

 void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    
   Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Turn",this, &ACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp",this, &ACharacter::AddControllerPitchInput);


    PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACOAAvatar::RunPressed);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ACOAAvatar::RunReleased);
}

void ACOAAvatar::RunPressed()
{
    GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    if (!bStaminaDrained)
    {
        bRunning = true;
    }
}

void ACOAAvatar::RunReleased()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    bRunning = false;
}

void ACOAAvatar::MoveForward(float value)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, value);
}

void ACOAAvatar::MoveRight(float value)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, value);
}

void ACOAAvatar::UpMovementParams()
{
    // If stamina is drained, force walking
    if (bStaminaDrained)
    {
        bRunning = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

        // Check if stamina has fully recovered
        if (Stamina >= MaxStamina)
        {
            bStaminaDrained = false;
        }
    }
    else if (bRunning)
    {
        // Allow running if stamina is not drained
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
    else
    {
        // Default to walking
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
      

}
void ACOAAvatar::DrainStamina(float DeltaTime)
{
    Stamina -= StaminaDrainRate * DeltaTime;

    if (Stamina <= 0.0f)
    {
        Stamina = 0.0f;
        bStaminaDrained = true;
    }
}

void ACOAAvatar::RegainStamina(float DeltaTime)
{
    if (!bRunning && Stamina < MaxStamina)
    {
        Stamina += StaminaGainRate * DeltaTime;
        if (Stamina > MaxStamina)
        {
            Stamina = MaxStamina;
        }
    }
}