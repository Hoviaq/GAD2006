﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameManager.h"
#include "TilePlayerController.h"

// Sets default values
ATileGameManager::ATileGameManager()
	: GridSize(100),
	GridOffset(0, 0, 0.5f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();


	if (auto PlayerController = Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;

        if (TileTypes.Num() < 8)
        {
            for (int i = 0; i < 8; i++)
            {
                ATileBase* NewTile = GetWorld()->SpawnActor<ATileBase>();
                TileTypes.Add(NewTile);
            }
        }
	}
	
}

// Called every frame
void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileGameManager::UpdateGridSelection()
{
    if (TileTypes.IsValidIndex(CurrentTileIndex))
    {
        UStaticMesh* Mesh = TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh();
        GridSelection->SetStaticMesh(Mesh);
    }
}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
    if (TileTypes.Num() == 0) return;

    FVector GridLoc = GridOffset;
    GridLoc.X = FMath::GridSnap(Location.X, GridSize);
    GridLoc.Y = FMath::GridSnap(Location.Y, GridSize);
    GridLoc.Z = Location.Z;

    UPlayerInput* Input = GWorld->GetFirstPlayerController()->PlayerInput;

    if (Input->WasJustPressed(EKeys::LeftMouseButton))
    {
        int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
        int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

        if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids * 2 || GridY >= MapExtendsInGrids * 2)
        {
            // Cannot place out of the grid
            return;
        }

        // Already a tile here?
        if (Map[GridX][GridY] != nullptr) return;

        if (TileTypes.IsValidIndex(CurrentTileIndex))
        {
            ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
            Map[GridX][GridY] = SelectedTile;

            FTransform TileTransform(GridLoc + GridOffset);
            SelectedTile->InstancedMesh->AddInstance(
                SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform,
                true
            );
        }
        UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f,%f,%f"),
            HitActor ? *HitActor->GetActorLabel() : TEXT("None"),
            Location.X,
            Location.Y,
            Location.Z);
    }
    if (Input->WasJustPressed(EKeys::LeftMouseButton))
    {
        // Sol fare tıklama
    }
    else if (Input->WasJustPressed(EKeys::MouseScrollDown))
    {
        CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();
        UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
    }
    else if (Input->WasJustPressed(EKeys::MouseScrollUp))
    {
        CurrentTileIndex = (CurrentTileIndex - 1 + TileTypes.Num()) % TileTypes.Num();
        UpdateGridSelection();
    }
    else if (Input->WasJustPressed(EKeys::RightMouseButton))
    {
        FRotator CurrentRotation = GridSelection->GetComponentRotation();
        CurrentRotation.Yaw += 90.0f;
        GridSelection->SetWorldRotation(CurrentRotation);
    }
    else
    {
        GridSelection->SetWorldLocation(GridLoc + GridOffset);
    }
}

