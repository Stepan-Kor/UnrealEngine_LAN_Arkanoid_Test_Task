// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Block.generated.h"

class UFloatingPawnMovement;
UCLASS()
class ARKANOID_LAN_COOP_API APawn_Block : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawn_Block();
	UFUNCTION(Server, Reliable)void Server_PrintNamesOfReplics();
	void Server_PrintNamesOfReplics_Implementation();
	UFUNCTION(NetMultiCast, Reliable)void Multi_PrintNamesOfReplics();
	void Multi_PrintNamesOfReplics_Implementation();
	UFUNCTION(Server,Reliable)void Server_UpdatePlacement(FVector NewLoc);
	void Server_UpdatePlacement_Implementation(FVector NewLoc);
protected:
	bool bServer = false;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* SM_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UFloatingPawnMovement* FloatingMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveHorizontal(float Axis);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
