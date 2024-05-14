// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "GridMapping/GridManager.h"

#include "PrintStrings.h"


AMainController::AMainController()
{

	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	isDebugMode = true;
}


void AMainController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* playerSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystemController = playerSubSystem;
		playerSubSystem->AddMappingContext(PlayerMappingInput, 0);

	}


}

void AMainController::OnConstruction(const FTransform& Transform)
{
}
