// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGame_AR.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_LAN_COOP_API USaveGame_AR : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) int Points = 0;
}; 
