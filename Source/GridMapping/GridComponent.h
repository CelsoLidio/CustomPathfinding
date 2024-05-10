// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GridComponent.generated.h"



USTRUCT(BlueprintType)
struct FTilesData
{
	GENERATED_BODY();


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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Properties | Debug")
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


	void InitGrid();

	FVector GetBottomPivot();

	void DrawAllTiles();

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentCreated() override;

	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Debug", meta= (AutoCreateRefTerm = "borderGridColor"))
	void DrawGrid(const FLinearColor &borderGridColor);
	
	UFUNCTION(BlueprintPure, Category = "Grid Properties | Tiles")
	FVector2D GetTileCount();

	UFUNCTION(BlueprintPure, Category = "Grid Properties | Collision")
	bool IsTileCollision(FVector locationTile, ETraceTypeQuery traceChannel);

	UFUNCTION(BlueprintCallable, Category = "Grid Properties | Tiles")
	void CreateGridData();

};
