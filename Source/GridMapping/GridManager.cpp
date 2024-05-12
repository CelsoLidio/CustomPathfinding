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

//Virtuals Methods//

void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}


void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Private Methods//

	//Non Static Methods//

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


int AGridManager::FindGridIndexAtLocation(FVector worldLocation)
{
	UGridComponent* currGrid = GetGridAtLocation(worldLocation);


	if (IsValid(currGrid))
	{
		return allGrids.Find(currGrid);
	}


	return -1;
}

	//Static Methods//

//Retorna o GridManager existente do mundo e caso não exista, cria um GridManager
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

UGridComponent* AGridManager::GetGridAtLocation(FVector worldLocation)
{
	AGridManager* managerGrid = GetGridManager();

	if (!IsValid(managerGrid))
	{
		return nullptr;

	}

	UGridComponent* outGrid = managerGrid->FindGridAtLocation(worldLocation);

	return outGrid;
	

}

int AGridManager::GetGridIndex(FVector worldLocation)
{
	AGridManager* managerGrid = GetGridManager();

	if (!IsValid(managerGrid))
	{
		return -1;

	}

	int idxGrid = managerGrid->FindGridIndexAtLocation(worldLocation);

	return idxGrid;
}


FTilesData AGridManager::GetTileFromWorldLoc(FVector worldLocation, bool isOnlyValid)
{
	UGridComponent* currGrid = GetGridAtLocation(worldLocation);

	if (IsValid(currGrid))
	{

		return currGrid->GetClosestTile(worldLocation, isOnlyValid);
	}

	return FTilesData();
}

UGridComponent* AGridManager::GetGridFromIndex(int targetIdxGrid)
{
	AGridManager* managerGrid = GetGridManager();

	if (!IsValid(managerGrid) || managerGrid->allGrids.Num() <= 0)
	{
		return nullptr;

	}
	
	if (IsValid(managerGrid->allGrids[targetIdxGrid]))
	{
		UGridComponent* outGrid = managerGrid->allGrids[targetIdxGrid];

		if (!IsValid(outGrid))
		{
			return nullptr;
		}

		return outGrid;
	}

	return nullptr;
	
}


//Public Methods//

	//Static Methods//


		//Input World Location//
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
	return GetTileFromWorldLoc(worldLocation).worldLocation;
}

FVector2D AGridManager::GetTileAtLocation(FVector worldLocation)
{
	return GetTileFromWorldLoc(worldLocation).gridIdx;


}

TArray<FVector2D> AGridManager::GetAllTileAtLocation(FVector worldLocation)
{
	UGridComponent* currGrid = GetGridAtLocation(worldLocation);

	if (IsValid(currGrid))
	{
		return currGrid->GetAllGridIdx();
	}

	return TArray<FVector2D>();
}

bool AGridManager::isTileAvailableFromLoc(FVector worldLocation)
{
	FTilesData currTile = GetTileFromWorldLoc(worldLocation, true);

	if (currTile.isAvailable && !currTile.isObstacle)
	{
		return true;
	}

	return false;
}


		//Input Index Tile and Index Grid//

bool AGridManager::isTileAvailable(FVector2D idxTile, int idxGrid)
{
	UGridComponent* currGrid = GetGridFromIndex(idxGrid);

	FTilesData currTile = currGrid->GetTileData(idxTile);

	if (currTile.isAvailable && !currTile.isObstacle)
	{
		return true;
	}

	return false;
}

FVector AGridManager::TileIndexToTileLocation(FVector2D idxTile, int idxGrid)
{
	UGridComponent* currGrid = GetGridFromIndex(idxGrid);

	FTilesData currTile = currGrid->GetTileData(idxTile);

	return currTile.worldLocation;
}

TArray<FVector2D> AGridManager::GetAllTileFromGrid(int idxGrid)
{
	UGridComponent* currGrid = GetGridFromIndex(idxGrid);

	if (IsValid(currGrid))
	{
		return currGrid->GetAllGridIdx();
	}

	return TArray<FVector2D>();
}

	

