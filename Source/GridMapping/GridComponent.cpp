// Fill out your copyright notice in the Description page of Project Settings.


#include "GridComponent.h"
#include "GridManager.h"

#include "PrintStrings.h"




// Sets default values for this component's properties
UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	tileSize = 50.0f;
	debugThickness = 4.0f;
	spacingTiles = 0.5f;
	isDebugMode = false;
	groundChannel = ETraceTypeQuery::TraceTypeQuery3;
	obstacleChannel = ETraceTypeQuery::TraceTypeQuery4;
}


//Override Methods//

void UGridComponent::OnComponentCreated()
{
	InitGrid();

}

void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

	AGridManager::AddGrid(this);
	
	if (isDebugMode)
	{
		DrawGrid();
	}
	
	InitGrid();
}

void UGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


//Protected Methods//

void UGridComponent::InitGrid()
{
	gridLocation = GetOwner()->GetActorLocation();
	gridWorldSize = FVector(100.0f, 100.0f,5.0f) * GetOwner()->GetActorScale();

	CreateGridData();
}

FVector UGridComponent::GetBottomPivot()
{
	
	FVector leftPoint = GetOwner()->GetActorRightVector() * gridWorldSize.X;
	FVector bottomPoint = GetOwner()->GetActorForwardVector() * gridWorldSize.Y;

	FVector resultPivot = (gridLocation - leftPoint) - bottomPoint;

	return resultPivot;
}

void UGridComponent::DrawAllTiles()
{
	FQuat gridRot = FQuat(GetOwner()->GetActorRotation());
	const float clampSpacing = FMath::Lerp<float, float>(0.1f, 0.9f, spacingTiles);


	TArray<FVector2D> tilesIdx;
	tilesData.GetKeys(tilesIdx);


	for (FVector2D eachKey : tilesIdx)
	{
		//printf("%s", *eachKey.ToString());
		FTilesData* currTile = tilesData.Find(eachKey);

		if (currTile->isAvailable)
		{
			if (!currTile->isObstacle)
			{
				DrawDebugBox(GetWorld(), currTile->worldLocation, FVector(tileSize, tileSize, gridWorldSize.Z) * clampSpacing, gridRot, FColor::Yellow, true, -1, 0.0f, debugThickness);
			}
			else
			{
				DrawDebugBox(GetWorld(), currTile->worldLocation, FVector(tileSize, tileSize, gridWorldSize.Z) * clampSpacing, gridRot, FColor::Red, true, -1, 0.0f, debugThickness);
			}
		}

	}


}


//Public Methods//

void UGridComponent::CreateGridData()
{
	print("CRIANDO TILES...");

	for (int gridX = 0; gridX <= GetTileCount().X; gridX++)
	{
		for (int gridY = 0; gridY <= GetTileCount().Y; gridY++)
		{
			float centerX = (gridX * (tileSize * 2)) + tileSize;
			float centerY = (gridY * (tileSize * 2)) + tileSize;

			FVector TileX = GetOwner()->GetActorRightVector() * centerX;
			FVector TileY = GetOwner()->GetActorForwardVector() * centerY;

			FVector TileLoc = GetBottomPivot() + TileX + TileY;

			FVector2D idxGrid = FVector2D(gridX, gridY);

			FTilesData currTile = FTilesData();

			currTile.gridIdx = idxGrid;
			currTile.worldLocation = TileLoc;


			if (IsTileCollision(TileLoc, groundChannel))
			{
				currTile.isAvailable = true;

				if (IsTileCollision(TileLoc, obstacleChannel))
				{
					currTile.isObstacle = true;
				}

			}


			tilesData.Add(idxGrid, currTile);

		}
	}

}

TArray<FVector2D> UGridComponent::GetAllTilesGrid()
{
	TArray<FVector2D> allGridIdx;
	tilesData.GetKeys(allGridIdx);

	return allGridIdx;
}

void UGridComponent::DrawGrid()
{
	
	FlushPersistentDebugLines(GetWorld());


	FQuat gridRot = FQuat(GetOwner()->GetActorRotation());

	DrawDebugBox(GetWorld(), gridLocation, gridWorldSize, gridRot, FColor::Purple, true,-1,0.0f, debugThickness);

	FVector sizeDebugPivot = FVector(10.0f, 10.0f, 5.0f) * GetOwner()->GetActorScale();
	DrawDebugBox(GetWorld(), GetBottomPivot(), sizeDebugPivot, gridRot, FColor::Magenta, true,-1,0.0f, debugThickness);

	DrawAllTiles();

}

FVector2D UGridComponent::GetTileCount()
{
	
	FVector countTiles = gridWorldSize / tileSize;

	const int TileX = FMath::RoundToInt(countTiles.X) - 1;
	const int TileY = FMath::RoundToInt(countTiles.Y) - 1;

	
	return FVector2D(TileX, TileY);
}

bool UGridComponent::IsTileCollision(FVector locationTile, ETraceTypeQuery traceChannel)
{
	TArray<AActor*> ignoreActors = TArray<AActor*>();
	FHitResult hitTrace;
	const float clampSpacing = FMath::Lerp<float, float>(0.1, 0.9, spacingTiles);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), locationTile, locationTile, tileSize * clampSpacing, traceChannel, false, ignoreActors, EDrawDebugTrace::None, hitTrace, true);

	return hitTrace.bBlockingHit;
}

bool UGridComponent::IsAvailableGrid()
{
	TArray<FVector2D> tilesIdx;
	tilesData.GetKeys(tilesIdx);


	for (FVector2D eachKey : tilesIdx)
	{
		FTilesData* currTile = tilesData.Find(eachKey);

		if (currTile->isAvailable && !currTile->isObstacle)
		{
			return true;
		}

	}

	return false;
}

FVector UGridComponent::GetGridLocation()
{
	return gridLocation;
}

FVector2D UGridComponent::GetTileAtLocation(FVector worldLocation)
{
	return GetClosestTile(worldLocation).gridIdx;
}

TArray<FVector> UGridComponent::GetAllLocationsTiles()
{
	TArray<FVector> outLocations = TArray<FVector>();

	for (FVector2D eachTile : GetAllTilesGrid())
	{
		outLocations.Add(GetLocationTile(eachTile));
	}

	return outLocations;
}

FVector UGridComponent::GetLocationTile(FVector2D indexTile)
{
	FTilesData currTile = GetTileData(indexTile);

	return currTile.worldLocation;
}

TArray<FVector> UGridComponent::TilesToLocations(TArray<FVector2D> listTiles)
{

	TArray<FVector> resultLocations = TArray<FVector>();

	for (FVector2D eachTile : listTiles)
	{
		FTilesData currTile = GetTileData(eachTile);

		resultLocations.Add(currTile.worldLocation);
	}

	return resultLocations;
}

bool UGridComponent::isTileAvailable(FVector2D indexTile)
{
	return isTileAvailableAtLocation(GetLocationTile(indexTile));
}

bool UGridComponent::isTileAvailableAtLocation(FVector worldLocation)
{
	FTilesData currTile = GetClosestTile(worldLocation,false);

	if (currTile.isAvailable && !currTile.isObstacle)
	{
		return true;
	}

	return false;
}

FTilesData UGridComponent::GetClosestTile(FVector targetPosition, bool isOnlyValid)
{
	TArray<FVector2D> tilesIdx;
	tilesData.GetKeys(tilesIdx);

	FTilesData resultTile;

	
	float minorDist = TNumericLimits<float>::Max();

	
	for (FVector2D eachKey : tilesIdx)
	{
		FTilesData* currTile = tilesData.Find(eachKey);

		if (isOnlyValid)
		{
			if (!currTile->isAvailable || currTile->isObstacle)
			{
				continue;
			}

		}
		

		float currDist = FVector::Distance(targetPosition, currTile->worldLocation);


		if (currDist < tileSize)
		{
			
			resultTile = *currTile;
			
			return resultTile;
		}

		if (currDist < minorDist)
		{
			minorDist = currDist;
			resultTile = *currTile;
		}

	}

	//printf("%s", *resultTile.gridIdx.ToString());
	return resultTile;
}

FTilesData UGridComponent::GetTileData(FVector2D indexTile)
{
	return *tilesData.Find(indexTile);
}

