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


	//Create Camera//
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);


	GetCharacterMovement()->GravityScale = 0.0f;

}



// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();



}

void AMainPlayer::OnConstruction(const FTransform& Transform)
{
	if (IsValid(objGrid))
	{
		currGrid = objGrid->GetComponentByClass<UGridComponent>();
	}
	
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

}

void AMainPlayer::initPlayer()
{

	if (IsValid(objGrid))
	{
		currGrid = objGrid->GetComponentByClass<UGridComponent>();
	}

	

	if (!IsValid(currGrid))
	{
		return;
	}





	FVector2D initidx = currGrid->GetValidClosestTile(GetActorLocation());

	//FVector2D idxTile = initTile.gridIdx;
	printf("test = %s", *initidx.ToString());

	/*FVector initPos = currGrid->tilesData.Find(FVector2D(0, 0))->worldLocation;

	SetActorLocation(initPos);*/

}

