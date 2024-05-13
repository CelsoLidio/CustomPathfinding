// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridComponent.h"

#include "GridManager.generated.h"


UCLASS()
class GRIDMAPPING_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	//Constructor//
	AGridManager();

	//Properties//

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grids")
	TArray<UGridComponent*> allGrids;

	//Methods//

private:


	//Statics Methods//
	static AGridManager* GetGridManager();
	
	static UGridComponent* GetGridAtLocation(FVector worldLocation);
	
	static FTilesData GetTileFromWorldLoc(FVector worldLocation, bool isOnlyValid = true);

	static UGridComponent* GetGridFromIndex(int targetIdxGrid);

	//Non Static Methods//
	int FindGridIndexAtLocation(FVector worldLocation);

	UGridComponent* FindGridAtLocation(FVector targetLoc);

	
	
	

protected:

	virtual void BeginPlay() override;


public:	
	
	virtual void Tick(float DeltaTime) override;

	static void AddGrid(UGridComponent* newGrid);

	//Methods with World Location as Input (Parameter)//

	//Grid//
	UFUNCTION(BlueprintPure, Category = "GridManager | Location Param | Grid")
	static FVector GetLocationGrid(FVector targetLoc);

	UFUNCTION(BlueprintPure, Category = "GridManager | Location Param | Grid")
	static int GetGridIndex(FVector worldLocation);

	



	//Tiles//
	UFUNCTION(BlueprintPure, Category = "GridManager | Location Param | Tiles")
	static FVector WorldLocationToTileLocation(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "GridManager | Location Param | Tiles")
	static FVector2D GetTileAtLocation(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "GridManager | Location Param | Tiles")
	static TArray<FVector2D> GetAllTileAtLocation(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "GridManager | Location Param | Tiles")
	static bool isTileAvailableFromLoc(FVector worldLocation);


	//Methods with Index Tile and Index Grid as Input (Parameter)//

	//Tiles//
	UFUNCTION(BlueprintPure, Category = "GridManager | Index Param | Tiles")
	static bool isTileAvailable(FVector2D idxTile, int idxGrid);

	UFUNCTION(BlueprintPure, Category = "GridManager | Index Param | Tiles")
	static FVector TileIndexToTileLocation(FVector2D idxTile, int idxGrid);

	UFUNCTION(BlueprintPure, Category = "GridManager | Index Param | Tiles")
	static TArray<FVector> TilesToLocations(TArray<FVector2D> listTiles, int idxGrid);

	UFUNCTION(BlueprintPure, Category = "GridManager | Index Param | Tiles")
	static TArray<FVector2D> GetAllTileFromGrid(int idxGrid);

};
