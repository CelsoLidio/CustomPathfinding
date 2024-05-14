// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSearchAStar.h"
#include "Kismet/GameplayStatics.h"

#include "PrintStrings.h"

void UPathSearchAStar::SetNodesToDefault()
{

	//create default//

	FNodeBase defaultNode;

	defaultNode.costFinal = MAX_COST;
	defaultNode.costFromStart = MAX_COST;
	defaultNode.estimatedCost = MAX_COST;
	defaultNode.previousNode = FVector2D::ZeroVector;


	TArray<FVector2D> keysNodes;

	GetAllNodes()->GetKeys(keysNodes);

	for (FVector2D eachKey : keysNodes)
	{
		FNodeBase* currNode = GetAllNodes()->Find(eachKey);

		defaultNode.nodeCost = currNode->nodeCost;

		currNode = &defaultNode;

		GetAllNodes()->Add(eachKey, *currNode);

	}


}

TArray<FVector2D> UPathSearchAStar::GetNodesNeighbours(FVector2D NodeIdx)
{
	TArray<FVector2D> outNeighbours;
	TArray<FVector2D> possibleNeighbours;

	TArray<FVector2D> fourDir;
	fourDir.Add(FVector2D(1.0f,0.0f));
	fourDir.Add(FVector2D(0.0f, 1.0f));
	fourDir.Add(FVector2D(-1.0f, 0.0f));
	fourDir.Add(FVector2D(0.0f, -1.0f));

	TArray<FVector2D> diagonalDir;
	diagonalDir.Add(FVector2D(1.0f, 1.0f));
	diagonalDir.Add(FVector2D(-1.0f, -1.0f));
	diagonalDir.Add(FVector2D(1.0f, -1.0f));
	diagonalDir.Add(FVector2D(-1.0f, 1.0f));


	for (FVector2D eachFour : fourDir)
	{ 
		possibleNeighbours.Add(eachFour);
	} 
	for (FVector2D eachDiagonal : diagonalDir)
	{ 
		possibleNeighbours.Add(eachDiagonal);
	}

	for (FVector2D eachNeighbour : possibleNeighbours)
	{
		FVector2D currNeighbour = NodeIdx + eachNeighbour;

		if (GetAllNodes()->Contains(currNeighbour))
		{
			if (GetAllNodes()->Find(currNeighbour)->nodeCost < MAX_COST)
			{
				if (diagonalDir.Contains(eachNeighbour))
				{
					FNodeBase* diagonalNode = GetAllNodes()->Find(currNeighbour);

					diagonalNode->nodeCost = NORMAL_COST + (NORMAL_COST * 0.4);

					GetAllNodes()->Add(currNeighbour, *diagonalNode);
				}
				
				outNeighbours.AddUnique(currNeighbour);
			}
		}

		
		
		
	}

	return outNeighbours;
}

void UPathSearchAStar::AddNode(FVector2D pointIdx, bool isAvailable)
{

	FNodeBase newNode;
	
	if (isAvailable)
	{
		newNode.nodeCost = 1;
	}
	else
	{
		newNode.nodeCost = MAX_COST;
	}
		
	GetAllNodes()->Add(pointIdx, newNode);
}

TMap<FVector2D, FNodeBase>* UPathSearchAStar::GetAllNodes()
{
	return &dictNodes;
}

void UPathSearchAStar::ClearNodes()
{
	GetAllNodes()->Empty();
}

TArray<FVector2D> UPathSearchAStar::CalcPathToTarget(FVector2D startNode, FVector2D targetNode)
{
	if (GetAllNodes()->Num() <= 0)
	{
		return TArray<FVector2D>();
	}

	if (!GetAllNodes()->Contains(targetNode))
	{
		return TArray<FVector2D>();
	}
	
	if (GetAllNodes()->Find(targetNode)->nodeCost >= MAX_COST)
	{
		return TArray<FVector2D>();
	}
	
	SetNodesToDefault();

	CalcStartingPoint(startNode, targetNode);

	//Defautls values
	TArray<FVector2D> allOpenNodes = TArray<FVector2D>();
	TArray<FVector2D> allClosedNodes = TArray<FVector2D>();

	FVector2D currentPoint = startNode;

	
	allOpenNodes.Add(currentPoint);
	
	while (allOpenNodes.Num() > 0)
	{
		FVector2D lowestCostPoint = allOpenNodes[0];
		//printf("LOOP = %i", allOpenNodes.Num());

		for (FVector2D eachOpen : allOpenNodes)
		{
			FNodeBase* openNode = GetAllNodes()->Find(eachOpen);
			FNodeBase* minorCostNode = GetAllNodes()->Find(lowestCostPoint);

			

			if ((openNode->costFinal < minorCostNode->costFinal) ||
				(openNode->costFinal == minorCostNode->costFinal && openNode->estimatedCost < minorCostNode->estimatedCost))
			{
				lowestCostPoint = eachOpen;
			}
			
		}
		
		currentPoint = lowestCostPoint;		

		allOpenNodes.Remove(currentPoint);
		allClosedNodes.AddUnique(currentPoint);


		for (FVector2D eachNeighbour : GetNodesNeighbours(currentPoint))
		{
			FVector2D neighbourPoint = eachNeighbour;
			
			if (allClosedNodes.Contains(neighbourPoint))
			{
				continue;
			}

			FNodeBase* currNode = GetAllNodes()->Find(currentPoint);
			FNodeBase* neighbourNode = GetAllNodes()->Find(neighbourPoint);
			
			int costToNeighbour = currNode->costFromStart + neighbourNode->nodeCost;

			if (!allOpenNodes.Contains(neighbourPoint))
			{
				allOpenNodes.AddUnique(neighbourPoint);
			}
			
			if (costToNeighbour < neighbourNode->costFromStart)
			{

				neighbourNode->previousNode = currentPoint;
				neighbourNode->costFromStart = costToNeighbour;
				neighbourNode->estimatedCost = GetEstimatedCostToTarget(neighbourPoint, targetNode);
				neighbourNode->costFinal = neighbourNode->costFromStart + neighbourNode->estimatedCost;
				

				GetAllNodes()->Add(neighbourPoint, *neighbourNode);

			}
			

			
			if (neighbourPoint == targetNode)
			{
				allOpenNodes.Empty();
				allClosedNodes.Empty();

				return RetracePath(targetNode, startNode);

			}
			
		}

	}

	return TArray<FVector2D>();

}

int UPathSearchAStar::GetEstimatedCostToTarget(FVector2D currentNode, FVector2D targetNode)
{
	FVector2D distNode = currentNode - targetNode;

	distNode = distNode.GetAbs();

	float resultCost = distNode.X + distNode.Y;
	
	return FMath::RoundToInt(resultCost);
}

void UPathSearchAStar::CalcStartingPoint(FVector2D startNode, FVector2D targetNode)
{
	FNodeBase* startingPoint = GetAllNodes()->Find(startNode);

	float valueEstimated = GetEstimatedCostToTarget(startNode, targetNode);

	if (isDebugMode)
	{
		printf("Estimated Cost To Target = %i", valueEstimated);
	}

	startingPoint->costFinal = valueEstimated;
	startingPoint->costFromStart = 0;
	startingPoint->estimatedCost = valueEstimated;
	startingPoint->previousNode = FVector2D::ZeroVector;

	GetAllNodes()->Add(startNode, *startingPoint);
}

TArray<FVector2D> UPathSearchAStar::RetracePath(FVector2D targetNode, FVector2D startNode)
{

	FVector2D currentPoint = targetNode;

	TArray<FVector2D> outPath = TArray<FVector2D>();

	TArray<FVector2D> invertedPath = TArray<FVector2D>();
	
	

	while (currentPoint != startNode)
	{
		invertedPath.AddUnique(currentPoint);

		if (GetAllNodes()->Contains(currentPoint))
		{
			FNodeBase* currNode = GetAllNodes()->Find(currentPoint);
			currentPoint = currNode->previousNode;
		}

	}

	for (int i = invertedPath.Num() - 1; i >= 0; i--)
	{
		FVector2D nodeInverted = invertedPath[i];

		outPath.AddUnique(nodeInverted);

	}

	return outPath;
}


