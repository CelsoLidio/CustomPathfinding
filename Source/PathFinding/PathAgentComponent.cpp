// Fill out your copyright notice in the Description page of Project Settings.


#include "PathAgentComponent.h"
#include "PrintStrings.h"


// Sets default values for this component's properties
UPathAgentComponent::UPathAgentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	pathFindingInst = NewObject<UPathSearchAStar>();
}



void UPathAgentComponent::BeginPlay()
{
	Super::BeginPlay();
	
	

}


// Called every frame
void UPathAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPathAgentComponent::CreatePointNodes(FPointData pointNode)
{

	pathFindingInst->AddNode(pointNode.pointIdx, pointNode.isAvailablePoint);


	printf("dict Nodes = %i", pathFindingInst->GetAllNodes().Find(FVector2D(0,0))->nodeCost);
	//printf("dict Nodes = %i", pathFindingInst->GetAllNodes().GetMaxIndex());
}




TArray<FVector2D> UPathAgentComponent::FindPath(FVector2D startNodeIdx, FVector2D endNodeIdx)
{


	return TArray<FVector2D>();
}

