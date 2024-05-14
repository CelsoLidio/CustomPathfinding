// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

#include "MainController.h"

#include "MainPlayer.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class CUSTOMPATHFINDING_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()


public:


	//Components//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float lookSensibility;


	//Actions player input//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CursorPointAction;


	//Variables//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = -90.0, ClampMax = 0.0))
	float rotCameraYawMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = 0.0, ClampMax = 90.0))
	float rotCameraYawMax;

private:

	//Variables//

	FHitResult currHitRaycast;

	AMainController* pController;

private:

	bool isRaycastHit();


	//Actions Method//
	void LookPlayer(const FInputActionValue& valueInput);
	
	void RaycastClick(const FInputActionValue& valueInput);
public:

	AMainPlayer();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void CursorRaycast(FVector impactPoint);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input | Debug")
	void CursorMovement(FVector impactPoint);
};
