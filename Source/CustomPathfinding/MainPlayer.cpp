// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "PrintStrings.h"



// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Create SpringArm//
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bDoCollisionTest = false;

	//Create Camera//
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);


	GetCharacterMovement()->GravityScale = 0.0f;
	lookSensibility = 70.0f;
	rotCameraYawMin = -90;
	rotCameraYawMax = 90;

	pController = nullptr;
}



// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	pController = Cast<AMainController>(GetController());
}

void AMainPlayer::OnConstruction(const FTransform& Transform)
{

}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);




	if (IsValid(pController))
	{
		if (pController->isDebugMode)
		{
			FVector2D cursorPos = FVector2D::ZeroVector;
			pController->GetMousePosition(cursorPos.X,cursorPos.Y);

			if (isRaycastHit())
			{
				CursorMovement(currHitRaycast.ImpactPoint);
			
			}
		}

		
	}
	

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		enhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayer::LookPlayer);

		enhancedInput->BindAction(ClickAction, ETriggerEvent::Started, this, &AMainPlayer::RaycastClick);
	}


}




void AMainPlayer::LookPlayer(const FInputActionValue& valueInput)
{
	if (GetController() == nullptr)
	{
		return;
	}

	FVector2D axisVector = valueInput.Get<FVector2D>() * lookSensibility * GetWorld()->GetDeltaSeconds();


	SpringArm->AddLocalRotation(FRotator(0, axisVector.X, 0));

	float yawCam = FMath::Clamp(SpringArm->GetRelativeRotation().Yaw, rotCameraYawMin, rotCameraYawMax);

	SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, yawCam, SpringArm->GetRelativeRotation().Roll));


}




void AMainPlayer::RaycastClick(const FInputActionValue& valueInput)
{
	bool isClicked = valueInput.Get<bool>();

	if (isClicked && isRaycastHit())
	{
		CursorRaycast(currHitRaycast.ImpactPoint);
	}


}



bool AMainPlayer::isRaycastHit()
{
	
	if (pController == nullptr)
	{
		return false;
	}

	pController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, currHitRaycast);

	if (currHitRaycast.bBlockingHit)
	{
		//printf("actor Hit = %s", *hitCursor.ImpactPoint.ToString());
		
		return true;
	}
	else
	{
		return false;

	}

	return false;
}
