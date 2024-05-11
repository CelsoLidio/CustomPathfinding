// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "PrintStrings.h"


// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//Retorna o GridManager existente do mundo e caso n�o exista, cria um GridManager
AGridManager* AGridManager::GetGridManager()
{
	if (!IsValid(GEngine->GameViewport))
	{
		return nullptr;
	}

	UWorld* world = GEngine->GameViewport->GetWorld();


	AGridManager* managerRef;
	TArray<AActor*> foundActors;

	UGameplayStatics::GetAllActorsOfClass(world, AGridManager::StaticClass(), foundActors);

	if (foundActors.Num() <= 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride;
		managerRef = world->SpawnActor<AGridManager>(AGridManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	else
	{
		managerRef = Cast<AGridManager>(foundActors[0]);
	}

	return managerRef;
}

UGridComponent* AGridManager::GetGridAtLocation(FVector targetLoc)
{
	AGridManager* managerGrid = GetGridManager();

	if (!IsValid(managerGrid))
	{
		return nullptr;

	}

	UGridComponent* outGrid = managerGrid->FindGridAtLocation(targetLoc);

	return outGrid;
	

}

void AGridManager::AddGrid(UGridComponent* newGrid)
{


	AGridManager* managerGrid = GetGridManager();

	if (!IsValid(managerGrid))
	{
		print("Manager Grid is NULL");
		return;
	}
	

	managerGrid->allGrids.Add(newGrid);
}

UGridComponent* AGridManager::FindGridAtLocation(FVector targetLoc)
{

	if (allGrids.Num() <= 0)
	{
		return nullptr;
	}

	UGridComponent* currGrid = nullptr;

	float minorDist = TNumericLimits<float>::Max();

	for (UGridComponent* eachGrid : allGrids)
	{
		if (!eachGrid->IsAvailableGrid())
		{
			continue;
		}

		FVector gridLoc = eachGrid->GetOwner()->GetActorLocation();

		float currDist = FVector::Distance(targetLoc, gridLoc);

		
		if (currDist < minorDist)
		{
			minorDist = currDist;
			currGrid = eachGrid;
		}

	}

	return currGrid;

}

FVector AGridManager::GetLocationGrid(FVector targetLoc)
{

	UGridComponent* currGrid = GetGridAtLocation(targetLoc);

	if (IsValid(currGrid))
	{
		return currGrid->GetGridLocation();
	}


	return FVector::ZeroVector;
}


FVector AGridManager::WorldLocationToTileLocation(FVector worldLocation)
{
	UGridComponent* currGrid = GetGridAtLocation(worldLocation);

	if (IsValid(currGrid))
	{

		FTilesData closestTile = currGrid->GetValidClosestTile(worldLocation);
		
		return closestTile.worldLocation;
		
	}

	return FVector::ZeroVector;
}

FVector2D AGridManager::GetTileAtLocation(FVector worldLocation)
{
	UGridComponent* currGrid = GetGridAtLocation(worldLocation);

	if (IsValid(currGrid))
	{

		FTilesData closestTile = currGrid->GetValidClosestTile(worldLocation);

		return closestTile.gridIdx;

	}

	return FVector2D::ZeroVector;
}




void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}


void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

