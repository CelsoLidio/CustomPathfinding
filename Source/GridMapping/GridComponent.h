// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GridComponent.generated.h"




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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Grid")
	float tileSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	float debugThickness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float spacingTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TEnumAsByte<ETraceTypeQuery> groundChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TEnumAsByte<ETraceTypeQuery> obstacleChannel;

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

	UFUNCTION(BlueprintCallable, Category = "Debug", meta= (AutoCreateRefTerm = "borderGridColor"))
	void DrawGrid(const FLinearColor &borderGridColor);
	
	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector2D GetTileCount();

	UFUNCTION(BlueprintPure, Category = "Grid")
	bool IsTileCollision(FVector locationTile, ETraceTypeQuery traceChannel);


};
