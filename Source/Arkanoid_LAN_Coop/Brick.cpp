// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "GameState_Play.h"

// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SM_Brik = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Brik"));
	SetRootComponent(SM_Brik);
	SM_Brik->SetNotifyRigidBodyCollision(true);
	if (BrickType == 0 && UpgradeMaterial1)SM_Brik->SetMaterial(0, UpgradeMaterial1);
	if (BrickType == 1 && UpgradeMaterial2)SM_Brik->SetMaterial(0, UpgradeMaterial2);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();
	SM_Brik->OnComponentHit.AddDynamic(this, &ABrick::Brik_Hit);
	//auto Controller = GetWorld()->GetFirstPlayerController<APlayerController>();
	auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)GameState->ChangeCountOfBricks(1);
	
}

void ABrick::Brik_Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHited_by_Ball) {
		auto Temporal_Ball = Cast<ABall>(OtherActor);
		if (Temporal_Ball) {
			bHited_by_Ball = true;
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABrick::DestroyBrick, 0.01);
			//GEngine->AddOnScreenDebugMessage	(-1, 4, FColor::Yellow, TEXT("Brick hited."));
		}
	}
}

void ABrick::DestroyBrick()
{
	auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState) { 
		if (BrickType == 0 || BrickType == 1)GameState->AddBonus(BrickType);
		GameState->ChangeCountOfBricks(-1);
	}
	this->Destroy();
}

// Called every frame
void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrick::UpgradeBrick(int Type)
{
	//if (!HasAuthority())return;
	BrickType = Type;
	Multi_UpgradeBrick(Type);
	if (!SM_Brik)return;
	if(Type == 0 && UpgradeMaterial1)SM_Brik->SetMaterial(0,UpgradeMaterial1);
	if(Type == 1 && UpgradeMaterial2)SM_Brik->SetMaterial(0,UpgradeMaterial2);
}

void ABrick::Multi_UpgradeBrick_Implementation(int Type)
{
	BrickType = Type;
	if (!SM_Brik)return;
	if (Type == 0 && UpgradeMaterial1)SM_Brik->SetMaterial(0, UpgradeMaterial1);
	if (Type == 1 && UpgradeMaterial2)SM_Brik->SetMaterial(0, UpgradeMaterial2);
}

