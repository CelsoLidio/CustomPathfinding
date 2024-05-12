// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PathSearchAStar.h"

#include "PathAgentComponent.generated.h"


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


private:

	UPathSearchAStar* pathFindingInst;


public:	
	// Sets default values for this component's properties
	UPathAgentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "PathFinding | Path")
	void CreatePointNodes(FPointData pointNode);


	UFUNCTION(BlueprintCallable, Category = "PathFinding | Path")
	TArray<FVector2D> FindPath(FVector2D startNode, FVector2D targetNode);


};
