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

	static AGridManager* GetGridManager();
	
	
	static UGridComponent* GetGridAtLocation(FVector targetLoc);


	UGridComponent* FindGridAtLocation(FVector targetLoc);

protected:

	virtual void BeginPlay() override;


public:	
	
	virtual void Tick(float DeltaTime) override;

	static void AddGrid(UGridComponent* newGrid);

	
	UFUNCTION(BlueprintPure, Category = "GridManager | Grid")
	static FVector GetLocationGrid(FVector targetLoc);

	UFUNCTION(BlueprintPure, Category = "GridManager | Tiles")
	static FVector WorldLocationToTileLocation(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "GridManager | Tiles")
	static FVector2D GetTileAtLocation(FVector worldLocation);


};
