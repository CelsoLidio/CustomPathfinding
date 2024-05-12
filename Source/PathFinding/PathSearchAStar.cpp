// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSearchAStar.h"
#include "Kismet/GameplayStatics.h"

#include "PrintStrings.h"



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
	diagonalDir.Add(FVector2D(1.0f, 0.0f));
	diagonalDir.Add(FVector2D(0.0f, 1.0f));
	diagonalDir.Add(FVector2D(-1.0f, 0.0f));
	diagonalDir.Add(FVector2D(0.0f, -1.0f));
	diagonalDir.Add(FVector2D(1.0f, 1.0f));
	diagonalDir.Add(FVector2D(-1.0f, -1.0f));
	diagonalDir.Add(FVector2D(1.0f, -1.0f));
	diagonalDir.Add(FVector2D(-1.0f, 1.0f));

	possibleNeighbours = diagonalDir;

	for (FVector2D eachNeighbour : possibleNeighbours)
	{
		FVector2D currNeighbour = NodeIdx + eachNeighbour;

		if (GetAllNodes().Contains(currNeighbour))
		{
			if (GetAllNodes().Find(currNeighbour)->nodeCost < MAX_COST)
			{
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
		
	dictNodes.Add(pointIdx, newNode);
}

TMap<FVector2D, FNodeBase> UPathSearchAStar::GetAllNodes()
{
	return dictNodes;
}

TArray<FVector2D> UPathSearchAStar::CalcPathToTarget(FVector2D startNode, FVector2D targetNode)
{

	if (GetAllNodes().Find(targetNode)->nodeCost >= MAX_COST)
	{
		return TArray<FVector2D>();
	}

	CalcStartingPoint(startNode, targetNode);

	//Defautls values
	TArray<FVector2D> allOpenNodes = TArray<FVector2D>();
	TArray<FVector2D> allClosedNodes = TArray<FVector2D>();

	FVector2D currentPoint = startNode;

	
	allOpenNodes.Add(currentPoint);
	
	while (allOpenNodes.Num() > 0)
	{
		FVector2D lowestCostPoint = allOpenNodes[0];
		printf("LOOP = %i", allOpenNodes.Num());

		for (FVector2D eachOpen : allOpenNodes)
		{
			FNodeBase* openNode = GetAllNodes().Find(eachOpen);
			FNodeBase* minorCostNode = GetAllNodes().Find(lowestCostPoint);

			if (openNode->costFinal < minorCostNode->costFinal)
			{
				lowestCostPoint = eachOpen;
			}
			else
			{
				if (openNode->costFinal == minorCostNode->costFinal)
				{
					if (openNode->estimatedCost < minorCostNode->estimatedCost)
					{
						lowestCostPoint = eachOpen;
					}
					
				}
				
			}
			
		}
		
		currentPoint = lowestCostPoint;		
		
		if (currentPoint == targetNode)
		{
			//final
			
			
			TArray<FVector2D> resultPath = TArray<FVector2D>();
			resultPath.Add(targetNode);
			
			FVector2D currentNodePath = targetNode;
			FNodeBase pathNode = *GetAllNodes().Find(currentNodePath);

			while (pathNode.previousNode != FVector2D::ZeroVector)
			{
				resultPath.Add(pathNode.previousNode);
				currentNodePath = pathNode.previousNode;
			}
			resultPath.Reserve(resultPath.Num());
			printf("ENTROU = %i", resultPath.Num());
			
			
			return allOpenNodes;
		}

		allOpenNodes.Remove(currentPoint);
		allClosedNodes.AddUnique(currentPoint);


		for (FVector2D eachNeighbour : GetNodesNeighbours(currentPoint))
		{
			FVector2D neighbourPoint = eachNeighbour;
			
			if (allClosedNodes.Contains(neighbourPoint))
			{
				continue;
			}

			FNodeBase* currNode = GetAllNodes().Find(currentPoint);
			FNodeBase* neighbourNode = GetAllNodes().Find(neighbourPoint);
			
			int costToNeighbour = currNode->costFromStart + GetEstimatedCostToTarget(neighbourPoint, currentPoint);//neighbourNode->nodeCost;

			
			
			if (costToNeighbour < neighbourNode->costFromStart)
			{
				
				neighbourNode->previousNode = currentPoint;
				neighbourNode->costFromStart = costToNeighbour;
				neighbourNode->estimatedCost = GetEstimatedCostToTarget(neighbourPoint, targetNode);
				neighbourNode->costFinal = neighbourNode->costFromStart + neighbourNode->estimatedCost;
				

				GetAllNodes().Add(neighbourPoint, *neighbourNode);

			}
			if (!allOpenNodes.Contains(neighbourPoint))
			{
				allOpenNodes.AddUnique(neighbourPoint);
			}

			/*
			if (neighbourPoint == targetNode)
			{
				allOpenNodes.Empty();
				allClosedNodes.Empty();

				return RetracePath(targetNode, startNode);

			}
			*/
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
	FNodeBase startingPoint = *GetAllNodes().Find(startNode);

	int valueEstimated = GetEstimatedCostToTarget(startNode, targetNode);

	startingPoint.costFinal = valueEstimated;
	startingPoint.costFromStart = 0;
	startingPoint.estimatedCost = valueEstimated;
	startingPoint.previousNode = FVector2D::ZeroVector;

	GetAllNodes().Add(startNode, startingPoint);
}

TArray<FVector2D> UPathSearchAStar::RetracePath(FVector2D targetNode, FVector2D startNode)
{

	FVector2D currentPoint = targetNode;

	TArray<FVector2D> outPath = TArray<FVector2D>();

	TArray<FVector2D> invertedPath = TArray<FVector2D>();
	
	FNodeBase* currNode = GetAllNodes().Find(currentPoint);

	while (currNode->previousNode != FVector2D::ZeroVector || currNode->previousNode != startNode)
	{
		print("entroui")
		invertedPath.AddUnique(currentPoint);

		if (GetAllNodes().Contains(currentPoint))
		{
			currNode = GetAllNodes().Find(currentPoint);
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
