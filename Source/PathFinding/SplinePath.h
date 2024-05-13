// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"

#include "SplinePath.generated.h"

UCLASS()
class PATHFINDING_API ASplinePath : public AActor
{
	GENERATED_BODY()
	
public:
	
	ASplinePath();

private:
	USceneComponent* RootPath;

	USplineComponent* SplinePath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	USplineComponent* GetPath();
};
