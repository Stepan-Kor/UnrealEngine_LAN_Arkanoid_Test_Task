// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_PlayScreen.h"
#include "GameFramework/GameStateBase.h"
#include "Brick.h"
#include "Net/UnrealNetwork.h"
#include "SaveGame_AR.h"
#include "GameState_Play.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FPointsSpreadDeclarationType, int);
UCLASS()
class ARKANOID_LAN_COOP_API AGameState_Play : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> WidgetClass_Screen = UWidget_PlayScreen::StaticClass();
	UWidget_PlayScreen* Widget_Screen;
	void ChangeCountOfBricks(int8 Diference);
	void GameLoosed();
	int8 BrickCount{ 0 };
	bool bGameFinished{ false };
	class ABall* Ball;
	void PlayerReady(int8 PlayerNumber, bool State=true);
	bool FirstPlayerReady{ false };
	bool SecondPlayerReady{ false };
	UFUNCTION(Server, Reliable)void Server_GameFinished(bool Victory);
	void Server_GameFinished_Implementation(bool Victory);
	UFUNCTION(NetMulticast, Reliable)void Multi_GameFinished(bool Victory);
	void Multi_GameFinished_Implementation(bool Victory);
	UFUNCTION(Server, Reliable)void Server_SetOwnerToActor(AActor* ActorToSetOwner,AActor* OwnerActor);
	void Server_SetOwnerToActor_Implementation(AActor* ActorToSetOwner,AActor* OwnerActor);
	UPROPERTY(VisibleAnywhere)  USaveGame_AR* LoadedGame;
	void GetPoints();
	void ChangePoints(int Diference);
	UFUNCTION(Server, Reliable)void Server_GetPoints();
	void Server_GetPoints_Implementation();
	UFUNCTION(NetMultiCast, Reliable)void Multi_GetPoints(int NewPoints);
	void Multi_GetPoints_Implementation(int NewPoints);
	int Points{ 0 };
	FPointsSpreadDeclarationType PointsWarnList;
	class AARController* PController;
	void AddBonus(int Type);
	UFUNCTION(NetMultiCast, reliable) void Multi_ScaleBlocks();
	void Multi_ScaleBlocks_Implementation();
	void ScaleLocalBlock();
	UFUNCTION(NetMultiCast, reliable) void Multi_SlowBall();
	void Multi_SlowBall_Implementation();
	void SlowBallLocal();

protected:

	void SpawnBricks();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABrick> BrickClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABrick> BrickClassUP1;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABrick> BrickClassUP2;
	
};
