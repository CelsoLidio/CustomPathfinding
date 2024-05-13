// Fill out your copyright notice in the Description page of Project Settings.


#include "SplinePath.h"

// Sets default values
ASplinePath::ASplinePath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootPath = CreateDefaultSubobject<USceneComponent>(TEXT("RootPath"));
	SetRootComponent(RootPath);

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootPath);

}


void ASplinePath::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASplinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USplineComponent* ASplinePath::GetPath()
{
	return SplinePath;
}

