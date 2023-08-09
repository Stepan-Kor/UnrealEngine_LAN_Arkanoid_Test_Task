// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_Play.h"
#include "Ball.h"
#include "Pawn_Block.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ARController.h"

void AGameState_Play::BeginPlay()
{

	Super::BeginPlay();
	Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	//this->GetPlayerId();
	if (!HasAuthority())return;
	SetReplicates(true);
	PController = GetWorld()->GetFirstPlayerController<AARController>();
	//if (PController) {	if (!PController->HasLocalNetOwner()) return;	}
	SpawnBricks();
}

void AGameState_Play::ChangeCountOfBricks(int8 Diference)
{
	BrickCount += Diference;
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::Printf(TEXT(" Amount of bricks %i"), BrickCount));
	if (BrickCount < 1 && !bGameFinished) {
		if (bGameFinished)return;
		bGameFinished = true;
		Server_GameFinished(true);
	}
}

void AGameState_Play::GameLoosed()
{
	if (bGameFinished)return;
	bGameFinished = true;
	Server_GameFinished(false);
}

void AGameState_Play::PlayerReady(int8 PlayerNumber, bool State)
{
	if (!HasAuthority())return;
	if (PlayerNumber == 0)FirstPlayerReady = State;
	else SecondPlayerReady = State;
	if(FirstPlayerReady && SecondPlayerReady)GetWorld()->ServerTravel("/Game/Level_Play?Listen");
}

void AGameState_Play::Server_GameFinished_Implementation(bool Victory)
{
	ChangePoints(Victory ? 1 : -123);
	Multi_GameFinished(Victory);
}

void AGameState_Play::Multi_GameFinished_Implementation(bool Victory)
{
	if (Ball)Ball->StopBall();
	if (Widget_Screen)Widget_Screen->GameFinished(Victory);
}

void AGameState_Play::Server_SetOwnerToActor_Implementation(AActor* ActorToSetOwner, AActor* OwnerActor)
{
	UE_LOG(LogTemp, Warning,
		TEXT("Game State %d: seting actor owner %s"),HasAuthority(), *ActorToSetOwner->GetName());
	if (IsValid(ActorToSetOwner) && IsValid(OwnerActor))ActorToSetOwner->SetOwner(OwnerActor);
	else UE_LOG(LogTemp, Warning, TEXT("Game State %s: cant set owner to actor."), *this->GetName());

}

void AGameState_Play::GetPoints()
{
	Server_GetPoints();
}

void AGameState_Play::ChangePoints(int Diference)
{
	LoadedGame = Cast<USaveGame_AR>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
	if (!IsValid(LoadedGame)) {
		LoadedGame = Cast<USaveGame_AR>(UGameplayStatics::CreateSaveGameObject(USaveGame_AR::StaticClass()));
		if (LoadedGame)UGameplayStatics::SaveGameToSlot(LoadedGame, "MainSlot", 0);
		else UE_LOG(LogTemp, Warning, TEXT("Game State %d: Cant load save file"), HasAuthority());
	}
	if (LoadedGame) {
		Points = LoadedGame->Points+ Diference;
		if (Points < 0)Points = 0;
		LoadedGame->Points = Points;
		UGameplayStatics::SaveGameToSlot(LoadedGame, "MainSlot", 0);
		
	}
}

void AGameState_Play::Server_GetPoints_Implementation()
{
	ChangePoints(0);
	if (!LoadedGame)return;
	Multi_GetPoints(Points);
}

void AGameState_Play::Multi_GetPoints_Implementation(int NewPoints)
{
	PointsWarnList.Broadcast(NewPoints);
}

void AGameState_Play::AddBonus(int Type)
{
	if (!HasAuthority())return;
	if (Type == 0) { ScaleLocalBlock(); Multi_ScaleBlocks(); };
	if (Type == 1) { SlowBallLocal(); Multi_SlowBall(); };
}

void AGameState_Play::Multi_ScaleBlocks_Implementation()
{
	ScaleLocalBlock();
}

void AGameState_Play::ScaleLocalBlock()
{
	TArray<AActor*>TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn_Block::StaticClass(), TempActors);
	for (AActor* TempActor: TempActors)
	{
		auto LocalBlock = Cast<APawn_Block>(TempActor);
		if (LocalBlock)LocalBlock->SetActorScale3D(FVector(1,1.5,1));
	}
}

void AGameState_Play::Multi_SlowBall_Implementation()
{
	SlowBallLocal();
}

void AGameState_Play::SlowBallLocal()
{
	TArray<AActor*>TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), TempActors);
	for (AActor* TempActor : TempActors)
	{
		auto LocalBall = Cast<ABall>(TempActor);
		if (LocalBall)LocalBall->SlowDown();
	}
}

void AGameState_Play::SpawnBricks()
{
	if (!BrickClass)return;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("GameState: PrintingBricks.")));
	TArray<bool> Booleans{false, false, false, false, false, false};
	TArray<int> YCords{120, 70, 20, 120, 70, 20};
	//TArray<int> Numbers{0, 1, 2, 3, 4, 5};
	//UKismetArrayLibrary::Array_Shuffle(Numbers);
	int Upgrade1= FMath::RandRange(0, 11);
	int Upgrade2= FMath::RandRange(0, 11);
	int Max = FMath::RandRange(3, 6);
	int counter{ -1 };
	for (int i =0 ; i < Max; i++) Booleans[i] = true;
	for (int i = 0; i < 6; i++)
	{
		if (FMath::RandBool())Booleans.Swap(0,i);
	}
	for (int8 i = 0; i < 2; i++)
	{
		for (int8 o = 0; o < 6; o++)
		{
			if (!Booleans[o])continue;
			counter++;
			FVector SpawnLoc = FVector(0, YCords[o] * (i == 0 ? 1 : -1), (o < 3 ? 200 : 230));
			if (counter == Upgrade1 && BrickClassUP1)
				GetWorld()->SpawnActor<ABrick>(BrickClassUP1, SpawnLoc, FRotator(0, 0, 0));
			else if (counter == Upgrade2 && BrickClassUP2)
				GetWorld()->SpawnActor<ABrick>(BrickClassUP2, SpawnLoc, FRotator(0, 0, 0));
			else 	GetWorld()->SpawnActor<ABrick>(BrickClass, SpawnLoc, FRotator(0, 0, 0));
		}

	}
}
