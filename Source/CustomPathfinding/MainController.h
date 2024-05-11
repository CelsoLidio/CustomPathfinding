// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubSystems.h"

#include "MainController.generated.h"


class UInputMappingContext;


/**
 * 
 */
UCLASS()
class CUSTOMPATHFINDING_API AMainController : public APlayerController
{
	GENERATED_BODY()
	
	AMainController();


public:

	//Mapping Input//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingInput;


	

private:



protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;


	UEnhancedInputLocalPlayerSubsystem* subsystemController;


};
