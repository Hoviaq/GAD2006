// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameGrid.h"
#include "UnitBase.h"
#include "Command.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

USTRUCT(BlueprintType)
struct FSUnitInfo
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AUnitBase> UnitClass;

	UPROPERTY(EditAnywhere)
	FSGridPosition StartPosition;
};

USTRUCT(BlueprintType)
struct FSLevelInfo
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	TArray<FSUnitInfo> Units;
};

UCLASS()
class AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();
	void OnActorClicked(AActor* Actor, FKey button);
	bool UndoLastMove();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	void CreateLevelActors(FSLevelInfo& Info);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AUnitBase* ThePlayer;

private:
	AUnitBase* ThePlayer;
	TArray<TSharedRef<Command>> CommandPool;
	TSharedPtr<Command> CurrentCommand;


};
