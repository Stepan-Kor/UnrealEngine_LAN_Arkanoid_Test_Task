// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameState_Play.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	SM_Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball_Mesh"));
	RootComponent = SM_Ball;
	SM_Ball->SetSimulatePhysics(true);
	SM_Ball->SetEnableGravity(false);
	//SM_Ball->SetConstraintMode(EDOFMode::YZPlane);
	SM_Ball->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SM_Ball->SetCollisionProfileName(TEXT("PhysicsActor"));
	SM_Ball->SetGenerateOverlapEvents(true);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_Movement"));
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.1f;
	ProjectileMovement->Friction = 0;
	ProjectileMovement->Velocity.X = 0;
}

void ABall::SlowDown()
{
	Speed = 175;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())return;
	if (bLaunched) {
		if (GetActorLocation().Z<-15) {
			auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
			if (GameState)GameState->GameLoosed();
			return;
		}
		FVector Velocity = SM_Ball->GetComponentVelocity();
		//GEngine->AddOnScreenDebugMessage(-1, .001, FColor::Yellow, FString::Printf(TEXT(" Ball speed Z %f"), Velocity.Z));
		if (abs(Velocity.Z) < 25) {
			Velocity.Z = 30 * (Velocity.Z >= 0 ? 1 : -1);
		}
		if (abs(Velocity.Y) < 25) {
			Velocity.Y = 30 * (Velocity.Y >= 0 ? 1 : -1);
		}
		Velocity.Normalize();
		Velocity *= Speed;
		SM_Ball->SetPhysicsLinearVelocity(Velocity);
		float LenghtOfVector = Velocity.Size();
		//FString Text = FString::Printf(TEXT("Size %f"), LenghtOfVector);
		//FString::Printf(TEXT("sdf %f"), Velocity.ToText())
		//GEngine->AddOnScreenDebugMessage(-1, .001, FColor::Yellow, Text);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("World delta for current frame equals %f"), GetWorld()->TimeSeconds));

	}
}

bool ABall::Launch()
{
	if (bLaunched) return false;
	bLaunched = true;
	SM_Ball->AddImpulse(FVector(0, FMath::RandRange(-250, 250), 150), FName(), true);
	//FMath::RandRange(-250, 250)
	return true;
}

void ABall::StopBall()
{
	bLaunched = false;
	SM_Ball->SetPhysicsLinearVelocity(FVector(0,0,0));
	if (!HasAuthority())return;
	auto GameState = Cast<AGameState_Play>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)GameState->GameLoosed();
}

