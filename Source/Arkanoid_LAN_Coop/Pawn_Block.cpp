// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Block.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "Components/StaticMeshComponent.h" 

// Sets default values
APawn_Block::APawn_Block()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("floating movement"));

	SM_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(SM_Mesh);
	SM_Mesh->SetupAttachment(RootComponent);
	SM_Mesh->SetEnableGravity(false);
	SM_Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Ignore);
	bReplicates = true;
	SetReplicateMovement(true);
}

void APawn_Block::Server_PrintNamesOfReplics_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn Block %s: server call names. Owner valid %d"),
		*this->GetName(), IsValid(GetOwner()));
	Multi_PrintNamesOfReplics();
}

void APawn_Block::Multi_PrintNamesOfReplics_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn Block %s: multi call names. Owner Valid %d."),
		*this->GetName(), IsValid(GetOwner()));
}

void APawn_Block::Server_UpdatePlacement_Implementation(FVector NewLoc)
{
	if(!bServer)bServer = true;
	SetActorLocation(NewLoc);
}

// Called when the game starts or when spawned
void APawn_Block::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(FMath::Rand(), 3, FColor::Yellow, 	FString::Printf(TEXT(" Pawn start: %s"), *this->GetName()));
	if (!HasLocalNetOwner())return;
	if (GetController())return;
	//GetWorld()->get
}

// Called every frame
void APawn_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector TempVect = GetActorLocation();
	bool Tempbool = false;
	if (TempVect.X != 0) { TempVect.X = 0; Tempbool = true; }
	if (TempVect.Y > 130) { TempVect.Y = 130; Tempbool = true; }
	else if (TempVect.Y < -130) { TempVect.Y = -130; Tempbool = true; }
	if (TempVect.Z != 20) { TempVect.Z = 20; Tempbool = true; }
	if(Tempbool)SetActorLocation(TempVect);
}

void APawn_Block::MoveHorizontal(float Axis)
{
	AddMovementInput(FVector(0, Axis, 0), 1, false);
	if (bServer)return;
	Server_UpdatePlacement(GetActorLocation());
}

// Called to bind functionality to input
void APawn_Block::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

