// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "PathSearchAStar.generated.h"


#define MAX_COST 999
#define NORMAL_COST 1.0f

USTRUCT()
struct PATHFINDING_API FNodeBase
{
	GENERATED_BODY();

	float nodeCost = NORMAL_COST;
	float costFromStart = MAX_COST;
	float costFinal = MAX_COST;
	float estimatedCost = MAX_COST;
	FVector2D previousNode = FVector2D::ZeroVector;


};


UCLASS()
class PATHFINDING_API UPathSearchAStar : public UObject
{
	GENERATED_BODY()
	

private:
	TMap<FVector2D, FNodeBase> dictNodes;
	

public:

	bool isDebugMode;
private:

	TArray<FVector2D> GetNodesNeighbours(FVector2D NodeIdx);
	
	int GetEstimatedCostToTarget(FVector2D currentNode, FVector2D targetNode);
	
	void CalcStartingPoint(FVector2D startNode, FVector2D targetNode);

	TArray<FVector2D> RetracePath(FVector2D targetNode, FVector2D startNode);

	void SetNodesToDefault();

public:

	void AddNode(FVector2D pointIdx, bool isAvailable);

	TMap<FVector2D, FNodeBase>* GetAllNodes();


	

	TArray<FVector2D> CalcPathToTarget(FVector2D startNode, FVector2D targetNode);
};
