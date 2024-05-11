// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSearchAStar.h"




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

	possibleNeighbours = fourDir;

	for (FVector2D eachNeighbour : possibleNeighbours)
	{
		FVector2D currNeighbour = NodeIdx + eachNeighbour;
		
		//Adicionar comparação se o node é valido ou não aqui
		
		outNeighbours.AddUnique(currNeighbour);
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
		newNode.nodeCost = 999;
	}
		
	dictNodes.Add(pointIdx, newNode);
}

TMap<FVector2D, FNodeBase> UPathSearchAStar::GetAllNodes()
{
	return dictNodes;
}
