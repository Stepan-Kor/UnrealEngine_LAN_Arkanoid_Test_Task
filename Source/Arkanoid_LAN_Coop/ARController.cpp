// Fill out your copyright notice in the Description page of Project Settings.


#include "ARController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Pawn_Block.h"
#include "Ball.h"
#include "GameState_Play.h"
#include "Net/UnrealNetwork.h"


void AARController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()) return;
	if (!HasLocalNetOwner())return;
	bReplicates = true;
	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (!GameMode)bFirstPlayer = false;
	if (IsValid(WidgetClass_Screen) )	
		Widget_Screen = CreateWidget<UWidget_PlayScreen>(GetWorld(), WidgetClass_Screen);
	if (IsValid(Widget_Screen)) {
		Widget_Screen->PlController = this;
		Widget_Screen->AddToViewport();
		Widget_Screen->StartSetting(this);
	}
	TArray<AActor*>TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(),TempActors);
	FViewTargetTransitionParams Camera_Parameters;
	if (!TempActors[0])	{
		UE_LOG(LogTemp, Warning, TEXT("Controller: didnt got camera. %s"), *this->GetName());
	}
	else {
		SetViewTarget(TempActors[0], Camera_Parameters);
	}
	PawnBlock = Cast<APawn_Block>(GetPawn());
	if (PawnBlock) PawnBlock->SetOwner(this);
	Ball=Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(),ABall::StaticClass()));

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AARController::DelayedFunction, 1);
	
}

void AARController::DelayedFunction()
{
	FViewTargetTransitionParams Camera_Parameters;
	TArray<AActor*>TempActors;
	//if(!bFirstPlayer)
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(),
		TempActors);
	if (!TempActors[0]) {
		UE_LOG(LogTemp, Warning, TEXT("Controller: didnt got camera. %s"), *this->GetName());
	}
	else {
		SetViewTarget(TempActors[0], Camera_Parameters);
	}
	if (!PawnBlock) {
		//UE_LOG(LogTemp, Warning, TEXT("Controller: didnt got pawn. %s"), *this->GetName());
		PawnBlock = Cast<APawn_Block>(GetPawn());
		if (!PawnBlock){
			UE_LOG(LogTemp, Warning, TEXT("Controller: still didnt got pawn. %s"), *this->GetName());}
		else PawnBlock->SetOwner(this);
	}
	if (!PawnBlock)return;
	PawnBlock->Server_PrintNamesOfReplics();
	auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)GameState->Server_SetOwnerToActor(PawnBlock, this);

}

void AARController::SetupInputComponent()
{
	Super::SetupInputComponent();
	EnableInput(this);
	InputComponent->BindAxis("MoveHorizontal", this, &AARController::MoveHorizontal);
	InputComponent->BindAction("Launch", IE_Pressed, this, &AARController::LaunchBall);
}

void AARController::MoveHorizontal(float AxisValue)
{
	if (PawnBlock && AxisValue != 0)PawnBlock->MoveHorizontal(AxisValue);
}

void AARController::CallPoints(){	Server_CallPoints();}

void AARController::Server_CallPoints_Implementation()
{
	auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)GameState->GetPoints();
}

void AARController::LaunchBall()
{
	Server_LaunchBall(); 
	if (!PawnBlock)
		UE_LOG(LogTemp, Warning, TEXT("Controller: still didnt got pawn. %s"), *this->GetName());
}

ABall* AARController::GetBall()
{
	if (Ball) return Ball;
	Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	return Ball;
}

void AARController::ReadyForNextChanged()
{
	Server_CheckReadinessForNext(bReadyFornextState);
}

void AARController::ChangeReadyForNextState(bool NewState)
{
	//UE_LOG(LogTemp, Warning, TEXT("Controller %s: ready."), *this->GetName());
	bReadyFornextState = NewState;
	Server_CheckReadinessForNext(bReadyFornextState);
	/*
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString(" controller has authority."));
	else GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString(" controller has NO authority."));*/

}
void AARController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME (AARController, bReadyFornextState);
}

void AARController::Server_CheckReadinessForNext_Implementation(bool NewState)
{
	bReadyFornextState = NewState;
	TArray<AActor*> Controllers;
	bool AllReady{ true };
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARController::StaticClass(), Controllers);
	for (AActor* TempActor : Controllers) {
		auto ARController = Cast<AARController>(TempActor);
		if (!ARController) continue;
		if (!ARController->bReadyFornextState){
			UE_LOG(LogTemp, Warning, TEXT("Controller %s: not ready"), *this->GetName());
			AllReady = false;
		}
		else UE_LOG(LogTemp, Warning, TEXT("Controller %s: ready"), *this->GetName());
	}
	if (AllReady)GetWorld()->ServerTravel("/Game/Level_Play?Listen");
	else GEngine->AddOnScreenDebugMessage
		(-1, 12, FColor::Red, FString(" Controller: not all players ready."));

}

void AARController::Server_LaunchBall_Implementation()
{
	GetBall();
	if (Ball)Ball->Launch();
}
