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

	
}


void UGridComponent::OnComponentCreated()
{
	InitGrid();
	CreateGridData();
}


// Called when the game starts
void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

	AGridManager::AddGrid(this);
	
}

void UGridComponent::InitGrid()
{
	gridLocation = GetOwner()->GetActorLocation();
	gridWorldSize = FVector(100.0f, 100.0f,5.0f) * GetOwner()->GetActorScale();

	
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


// Called every frame
void UGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UGridComponent::DrawGrid(const FLinearColor& borderGridColor)
{
	
	FlushPersistentDebugLines(GetWorld());


	FQuat gridRot = FQuat(GetOwner()->GetActorRotation());

	DrawDebugBox(GetWorld(), gridLocation, gridWorldSize, gridRot, borderGridColor.ToFColor(true), true,-1,0.0f, debugThickness);

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

void UGridComponent::CreateGridData()
{
	

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

FTilesData UGridComponent::GetValidClosestTile(FVector targetPosition)
{
	TArray<FVector2D> tilesIdx;
	tilesData.GetKeys(tilesIdx);

	FTilesData resultTile;

	
	float minorDist = TNumericLimits<float>::Max();

	
	for (FVector2D eachKey : tilesIdx)
	{
		FTilesData* currTile = tilesData.Find(eachKey);

		if (!currTile->isAvailable || currTile->isObstacle)
		{
			continue;
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

