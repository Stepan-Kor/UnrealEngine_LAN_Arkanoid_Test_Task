// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.generated.h"

UCLASS()
class ARKANOID_LAN_COOP_API ABrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* SM_Brik;
	UFUNCTION()  void Brik_Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void DestroyBrick();
	bool bHited_by_Ball{ false };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) int BrickType{ -1 };
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)UMaterial* UpgradeMaterial1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)UMaterial* UpgradeMaterial2;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpgradeBrick(int Type);
	UFUNCTION(NetMulticast, reliable) void Multi_UpgradeBrick(int Type);
	void Multi_UpgradeBrick_Implementation(int Type);

};
