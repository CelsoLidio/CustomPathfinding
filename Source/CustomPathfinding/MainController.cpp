// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"

AMainController::AMainController()
{

	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	
}


void AMainController::BeginPlay()
{
	Super::BeginPlay();

}

void AMainController::OnConstruction(const FTransform& Transform)
{

	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
}
