// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class GAD2006_REBER_API ACOAAvatar : public ACOABaseCharacter
{
   GENERATED_BODY()

public:
    ACOAAvatar();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    UCameraComponent* mCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    USpringArmComponent* mSpringArm;
protected:
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
void MoveForward(float value);
void MoveRight(float value);

public:
   
public:
    void MoveForward(float value);
    void MoveRight(float value);
    void RunPressed();
    void RunReleased();
    void UpMovementParams();
    void DrainStamina(float DeltaTime);
    void RegainStamina(float DeltaTime);


    
};
