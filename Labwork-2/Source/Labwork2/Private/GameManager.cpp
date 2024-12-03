// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "TBPlayerController.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
    if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
    {
        PlayerController->GameManager = this;
    }

    if (Levels.IsValidIndex(CurrentLevel))
    {
        CreateLevelActors(Levels[CurrentLevel]);
    }
	
}

void AGameManager::OnActorClicked(AActor* Actor, FKey button)
{
    // Still executing a command?
    if (CurrentCommand.IsValid() && CurrentCommand->IsExecuting()) return;

    AGameSlot* Slot = Cast<AGameSlot>(Actor);

    // Clicked on a non-slot actor?
    if (!Slot) return;

    // Player click?
    if (!ThePlayer) {
        UE_LOG(LogTemp, Error, TEXT("No Player Unit Detected!"));
        return;
    }

    // Empty slot? Then it's a move command for the player
    if (Slot->Unit == nullptr)
    {
        // Move from player position to clicked position
        TSharedRef<MoveCommand> Cmd = MakeShared<MoveCommand>(ThePlayer->Slot->GridPosition, Slot->GridPosition);
        CommandPool.Add(Cmd);
        Cmd->Execute();
        CurrentCommand = Cmd;
    }
}


// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (CurrentCommand.IsValid()) 
    {
        CurrentCommand->Update(DeltaTime);
    }
}
}
void AGameManager::CreateLevelActors(FSLevelInfo& Info)
{
    ThePlayer = nullptr;

    for (auto UnitInfo : Info.Units)
    {
        if (AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
        {
            Slot->SpawnUnitHere(UnitInfo.UnitClass);
        }
    }
}
bool AGameManager::UndoLastMove()
{
    if (CommandPool.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No commands to undo!"));
        return false;
    }

    TSharedRef<Command> LastCommand = CommandPool.Pop();
    LastCommand->Revert();
    CurrentCommand.Reset();

    UE_LOG(LogTemp, Warning, TEXT("Last move undone successfully!"));
    return true;
}

void AGameManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("UndoMove", IE_Pressed, this, &AGameManager::UndoLastMove);
}