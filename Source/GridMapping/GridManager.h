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

	//Non Static Methods//
	int FindGridIndexAtLocation(FVector worldLocation);

	UGridComponent* FindGridAtLocation(FVector targetLoc);

	

protected:

	virtual void BeginPlay() override;


public:	
	
	virtual void Tick(float DeltaTime) override;

	static void AddGrid(UGridComponent* newGrid);

	//Grid//
	UFUNCTION(BlueprintPure, Category = "GridManager | Grid")
	static FVector GetLocationGrid(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "GridManager | Grid")
	static UGridComponent* GetGridAtIndex(int indexGrid);

	UFUNCTION(BlueprintPure, Category = "GridManager | Grid")
	static UGridComponent* GetGridAtLocation(FVector worldLocation);

	UFUNCTION(BlueprintPure, Category = "GridManager | Grid")
	static TArray<UGridComponent*> GetAllGrids();

};
