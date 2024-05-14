// Fill out your copyright notice in the Description page of Project Settings.


#include "PathAgentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"



#include "PrintStrings.h"


// Sets default values for this component's properties
UPathAgentComponent::UPathAgentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	pathFindingInst = NewObject<UPathSearchAStar>();

	isDebugMode = true;

	pathToMove = nullptr;
}




void UPathAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	


	pathFindingInst->isDebugMode = this->isDebugMode;


	if (CurveMovement != nullptr)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));

		MovementTimeline.AddInterpFloat(CurveMovement, TimelineProgress);

	}
	else
	{
	
		print("[ERROR - PathAgent] Curve Movement Path Not Found...")
		
	}

}


// Called every frame
void UPathAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	MovementTimeline.TickTimeline(DeltaTime);

}

void UPathAgentComponent::CreatePointNodes(FPointData pointNode)
{

	pathFindingInst->AddNode(pointNode.pointIdx, pointNode.isAvailablePoint);

}




TArray<FVector2D> UPathAgentComponent::FindPathToTarget(TArray<FPointData> allNodes, FVector2D startNode, FVector2D targetNode)
{

	TArray<FVector2D> resultPath = TArray<FVector2D>();


	for (FPointData eachPoint : allNodes)
	{
		pathFindingInst->AddNode(eachPoint.pointIdx, eachPoint.isAvailablePoint);
	}


	resultPath = pathFindingInst->CalcPathToTarget(startNode, targetNode);

	pathFindingInst->ClearNodes();

	if (isDebugMode)
	{
		printf("count cost to path = %i", resultPath.Num());
	}


	return resultPath;
}

void UPathAgentComponent::MovementActorFromPath(TArray<FVector> LocpathPoints)
{
	
	APawn* actorOwner = nullptr;

	if (Cast<ACharacter>(GetOwner()))
	{
		actorOwner = Cast<APawn>(GetOwner());
	}
	else
	{
		return;
	}
	
	
	if (pathToMove != nullptr)
	{
		pathToMove->Destroy();
		pathToMove = nullptr;
	}

	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride;

	pathToMove = GetWorld()->SpawnActor<ASplinePath>(ASplinePath::StaticClass(), GetComponentLocation(), GetComponentRotation(), spawnParam);
	
	pathToMove->GetPath()->ClearSplinePoints(true);
	
	FVector offsetMovement = FVector::ZeroVector;

	if (Cast<ACharacter>(actorOwner))
	{
		ACharacter* actorCharacter = Cast<ACharacter>(actorOwner);
		
		offsetMovement += actorCharacter->GetActorForwardVector() * actorCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

		offsetMovement += (-actorCharacter->GetActorUpVector()) * actorCharacter->GetDefaultHalfHeight() / 2;
		
	}

	pathToMove->GetPath()->AddSplinePoint(actorOwner->GetActorLocation(), ESplineCoordinateSpace::World, true);

	for (FVector eachPoint : LocpathPoints)
	{

		pathToMove->GetPath()->AddSplinePoint(eachPoint - offsetMovement,ESplineCoordinateSpace::World,true);
	}

	MovementTimeline.PlayFromStart();

}


void UPathAgentComponent::TimelineProgress(float valueCurve)
{
	APawn* actorOwner = nullptr;

	if (Cast<APawn>(GetOwner()))
	{
		actorOwner = Cast<APawn>(GetOwner());
	}
	else
	{
		return;
	}

	float lengthSpline = pathToMove->GetPath()->GetSplineLength();

	float progressionPath = valueCurve * lengthSpline;

	FVector pathLoc = pathToMove->GetPath()->GetLocationAtDistanceAlongSpline(progressionPath, ESplineCoordinateSpace::World);	
	FRotator pathRot = pathToMove->GetPath()->GetRotationAtDistanceAlongSpline(progressionPath, ESplineCoordinateSpace::World);
	
	

	if (Cast<ACharacter>(actorOwner))
	{
		ACharacter* actorCharacter = Cast<ACharacter>(actorOwner);

		FRotator actorRot = actorOwner->GetActorRotation();
		actorCharacter->GetMesh()->SetWorldRotation(FRotator(actorRot.Pitch, pathRot.Yaw, actorRot.Roll));
	}

	actorOwner->SetActorLocation(pathLoc);

}

