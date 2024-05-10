// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../GridMapping/GridComponent.h"



#include "MainPlayer.generated.h"



UCLASS()
class CUSTOMPATHFINDING_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	AActor* objGrid;


protected:

	UGridComponent* currGrid = nullptr;

public:

	AMainPlayer();

public:

	virtual void BeginPlay() override;


	virtual void OnConstruction(const FTransform& Transform) override;


public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void initPlayer();

};
