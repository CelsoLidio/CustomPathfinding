// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMPATHFINDING_API AMainController : public APlayerController
{
	GENERATED_BODY()
	
	AMainController();


protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
