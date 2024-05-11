// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathSearchAStar.generated.h"


USTRUCT()
struct PATHFINDING_API FNodeBase
{
	GENERATED_BODY();

	int nodeCost = 1;
	int costG;
	int costH;
	int estimatedCost;
	FVector2D previousNode;


};


UCLASS()
class PATHFINDING_API UPathSearchAStar : public UObject
{
	GENERATED_BODY()
	

private:
	TMap<FVector2D, FNodeBase> dictNodes;


private:

	TArray<FVector2D> GetNodesNeighbours(FVector2D NodeIdx);

	
public:

	void AddNode(FVector2D pointIdx, bool isAvailable);

	TMap<FVector2D, FNodeBase> GetAllNodes();
};
