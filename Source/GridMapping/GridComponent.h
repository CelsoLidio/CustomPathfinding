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

	//Grid Properties//
	UPROPERTY()
	FVector gridLocation;

	UPROPERTY()
	FVector gridWorldSize;

	TMap<FVector2D, FTilesData> tilesData;

public:
	
	//Debug properties//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Debug")
	bool isDebugMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Properties | Debug", meta = (EditCondition = "isDebugMode"))
	float debugThickness;

	//Tiles Properties//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Properties | Tiles", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float spacingTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Tiles ")
	float tileSize;

	//Collisions Properties//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Collision")
	TEnumAsByte<ETraceTypeQuery> groundChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties | Collision")
	TEnumAsByte<ETraceTypeQuery> obstacleChannel;


	
	//Methods//
public:	
	
	UGridComponent();

protected:
	
	virtual void BeginPlay() override;

	//Constructor Grid Methods//

		//Create Grid//
	void InitGrid();

		//Create All Datas Grid//
	void CreateGridData();

		//Get new Pivot Grid//
	FVector GetBottomPivot();

	//Debug//
	
		//Draw Debug Tiles//	
	void DrawAllTiles();


public:	
	
	//Virtual Methods//

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentCreated() override;

	//Public Methods//

	FTilesData GetClosestTile(FVector targetPosition, bool isOnlyValid = true);

	bool IsTileCollision(FVector locationTile, ETraceTypeQuery traceChannel);

	FTilesData GetTileData(FVector2D indexTile);

	
public:

	//Blueprint Methods//
		
		//Debug Mthods//
	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Debug")
	void DrawGrid();
	
		//Tiles Methods//
	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	FVector2D GetTileCount();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	FVector2D GetTileAtLocation(FVector worldLocation);
	
	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	TArray<FVector> GetAllLocationsTiles();


	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	FVector GetLocationTile(FVector2D indexTile);

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	TArray<FVector> TilesToLocations(TArray<FVector2D> listTiles);



		//Verify Methods//
	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	bool isTileAvailableAtLocation(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	bool isTileAvailable(FVector2D indexTile);


		//Grid Methods//

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Grid")
	bool IsAvailableGrid();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Grid")
	FVector GetGridLocation();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Grid")
	TArray<FVector2D> GetAllTilesGrid();
	


};
