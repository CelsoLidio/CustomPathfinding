// Fill out your copyright notice in the Description page of Project Settings.


#include "GridComponent.h"
#include "PrintStrings.h"




// Sets default values for this component's properties
UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	tileSize = 50.0f;
	debugThickness = 4.0f;
	spacingTiles = 0.5f;
}


// Called when the game starts
void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

	gridLocation = GetOwner()->GetActorLocation();
	
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

	printf("%f", clampSpacing);

	for (int gridX = 0; gridX <= GetTileCount().X; gridX++)
	{
		for (int gridY = 0; gridY <= GetTileCount().Y; gridY++)
		{
			float centerX = (gridX * (tileSize * 2)) + tileSize;
			float centerY = (gridY * (tileSize * 2)) + tileSize;

			FVector TileX = GetOwner()->GetActorRightVector() * centerX;
			FVector TileY = GetOwner()->GetActorForwardVector() * centerY;

			FVector TileLoc = GetBottomPivot() + TileX + TileY;

			if (IsTileCollision(TileLoc,groundChannel))
			{
				if (!IsTileCollision(TileLoc, obstacleChannel))
				{
					DrawDebugBox(GetWorld(), TileLoc, FVector(tileSize, tileSize, gridWorldSize.Z) * clampSpacing, gridRot, FColor::Yellow, true, -1, 0.0f, debugThickness);
				}
				else
				{
					DrawDebugBox(GetWorld(), TileLoc, FVector(tileSize, tileSize, gridWorldSize.Z) * clampSpacing, gridRot, FColor::Red, true, -1, 0.0f, debugThickness);
				}
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
	
	InitGrid();
	
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

