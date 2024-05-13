// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"


#include "GridComponent.generated.h"



USTRUCT(BlueprintType)
struct GRIDMAPPING_API FTilesData
{
	GENERATED_BODY();
public:

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	bool isAvailable = false;

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	bool isObstacle = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tile")
	FVector worldLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FVector2D gridIdx = FVector2D::ZeroVector;


};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDMAPPING_API UGridComponent : public USceneComponent
{
	GENERATED_BODY()


	//Properties//

private:

	UPROPERTY()
	FVector gridLocation;

	UPROPERTY()
	FVector gridWorldSize;

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Grid Properties | Tiles ")
	float tileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Debug")
	bool isDebugMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Properties | Debug", meta = (EditCondition = "isDebugMode"))
	float debugThickness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Properties | Tiles", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float spacingTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Collision")
	TEnumAsByte<ETraceTypeQuery> groundChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Collision")
	TEnumAsByte<ETraceTypeQuery> obstacleChannel;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Tiles")
	TMap<FVector2D,FTilesData> tilesData;
	
	//Methods//
public:	
	
	UGridComponent();

protected:
	
	virtual void BeginPlay() override;

	//Create Grid//
	void InitGrid();

	//Get new Pivot Grid//
	FVector GetBottomPivot();

	//Draw Debug Tiles//
	void DrawAllTiles();

	

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentCreated() override;



public:


	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Debug", meta= (AutoCreateRefTerm = "borderGridColor"))
	void DrawGrid();
	
	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	FVector2D GetTileCount();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Collision")
	bool IsTileCollision(FVector locationTile, ETraceTypeQuery traceChannel);

	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Tiles")
	void CreateGridData();

	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Tiles")
	FTilesData GetClosestTile(FVector targetPosition, bool isOnlyValid = true);

	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Tiles")
	FTilesData GetTileData(FVector2D idxTile);


	UFUNCTION(BlueprintPure, Category = "Grid Properties | Grid")
	bool IsAvailableGrid();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Grid")
	FVector GetGridLocation();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Grid")
	TArray<FVector2D> GetAllGridIdx();


};
