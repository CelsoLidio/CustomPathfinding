// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"

#include "SplinePath.h"
#include "PathSearchAStar.h"

#include "PathAgentComponent.generated.h"


class UCUrveFloat;

USTRUCT(Blueprintable)
struct FPointData
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Grid")
	FVector2D pointIdx = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Grid")
	bool isAvailablePoint = false;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHFINDING_API UPathAgentComponent : public USceneComponent
{
	GENERATED_BODY()


public:

	
private:

	UPathSearchAStar* pathFindingInst;

	//Timeline Properties//

	FTimeline MovementTimeline;

	UPROPERTY(EditAnywhere, Category = "PathFinding | Location")
	UCurveFloat* CurveMovement;


	ASplinePath* pathToMove;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugMode")
	bool isDebugMode;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFinding | Movement")
	//FVector offsetMovement;

public:	
	
	UPathAgentComponent();


protected:
	
	virtual void BeginPlay() override;


public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "PathFinding | Path")
	void CreatePointNodes(FPointData pointNode);


	UFUNCTION(BlueprintCallable, Category = "PathFinding | Path")
	TArray<FVector2D> FindPathToTarget(TArray<FPointData> allNodes, FVector2D startNode, FVector2D targetNode);

	UFUNCTION(BlueprintCallable, Category = "PathFinding | Location")
	void MovementActorFromPath(TArray<FVector> LocpathPoints);

	//UFUNCTION(BlueprintCallable, Category = "PathFinding | Location")
	//void SetActor(TArray<FVector> LocpathPoints);

	//Timeline Functions//

	UFUNCTION()
	void TimelineProgress(float valueCurve);


};
