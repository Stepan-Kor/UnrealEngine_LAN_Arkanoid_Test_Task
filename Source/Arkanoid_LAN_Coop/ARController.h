// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widget_PlayScreen.h"
#include "ARController.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_LAN_COOP_API AARController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UFUNCTION() virtual void SetupInputComponent() override;
	void MoveHorizontal(float AxisValue);
	void DelayedFunction();
	class APawn_Block* PawnBlock;
	class ABall* Ball;
	bool bFirstPlayer{ true };
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> WidgetClass_Screen = UWidget_PlayScreen::StaticClass();
	UWidget_PlayScreen* Widget_Screen;
	
public:
	void CallPoints();
	UFUNCTION(Server, reliable)void Server_CallPoints();
	void Server_CallPoints_Implementation();
	void LaunchBall();
	ABall* GetBall();
	UFUNCTION() void ReadyForNextChanged();
	UPROPERTY(ReplicatedUsing = ReadyForNextChanged, VisibleAnywhere, BlueprintReadOnly) 
		bool bReadyFornextState{false};
	void ChangeReadyForNextState(bool NewState=true);
	virtual void GetLifetimeReplicatedProps(class TArray<FLifetimeProperty>& OutLifetimeProps)const;
	UFUNCTION(Server, Reliable) void Server_CheckReadinessForNext(bool NewState);
	void Server_CheckReadinessForNext_Implementation(bool NewState);
	UFUNCTION(Server, Reliable) void Server_LaunchBall();
	void Server_LaunchBall_Implementation();
};
