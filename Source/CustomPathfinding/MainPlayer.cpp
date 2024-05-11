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

}



// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	initPlayer();

}

void AMainPlayer::OnConstruction(const FTransform& Transform)
{
	
	//GetCharacterMovement()->GravityScale = 0.0f;
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AMainPlayer::initPlayer()
{
	FVector initLoc = AGridManager::WorldLocationToTileLocation(GetActorLocation());
	FVector gridLoc = AGridManager::GetLocationGrid(GetActorLocation());
	

	
	SetActorLocation(initLoc);
	FRotator lookAtGrid = (GetActorLocation() - gridLoc).Rotation().Clamp();

	//printf("look at grid = %s", *lookAtGrid.ToString());
	float offsetYaw= 90;

	if (lookAtGrid.Yaw <= 270)
	{
		offsetYaw = -90;
	}
	
	SetActorRotation(FRotator(GetActorRotation().Pitch, lookAtGrid.Yaw + offsetYaw, GetActorRotation().Roll));
	
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

	if (isClicked)
	{
		FHitResult hitCursor;
		
		APlayerController* pController = Cast<APlayerController>(GetController());
		
		pController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitCursor);

		if (hitCursor.bBlockingHit)
		{
			//printf("actor Hit = %s", *hitCursor.ImpactPoint.ToString());

			FVector tileLoc = AGridManager::WorldLocationToTileLocation(hitCursor.ImpactPoint);

			FVector2D tileIdx = AGridManager::GetTileAtLocation(hitCursor.ImpactPoint);

			printf("tile location = %s", *tileIdx.ToString())

			
		}
		else
		{
			print("NOT hitted");
		}
	}


}