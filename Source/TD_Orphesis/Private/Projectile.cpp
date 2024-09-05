#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/DamageEvents.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SceneComponent->Mobility = EComponentMobility::Movable;
	RootComponent = SceneComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
	ArrowComponent->SetupAttachment(RootComponent);
	
	if (MeshComponent->GetStaticMesh() == nullptr) { UE_LOG(LogTemp, Warning, TEXT("StaticMesh is nullptr")); }
}

void AProjectile::InitializeProjectile(AEnemy* NewTargetEnemy)
{
	if (bIsInitialized) { return; }
	bIsInitialized = true;
	TargetEnemy = NewTargetEnemy;
	if (MeshComponent->GetStaticMesh() != nullptr) { MeshComponent->SetVisibility(false); }
}

void AProjectile::Launch()
{
	if (bIsLaunched) { return; }
	bIsLaunched = true;

	if (!TargetEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetEnemy is nullptr"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TargetEnemy is nullptr"));
		Destroy();
		return;
	}

	// Set actor rotation
	SetActorRotation(GetLookAtRotation(GetActorLocation(), TargetEnemy->GetActorLocation()));

	// Play sound
	UGameplayStatics::PlaySound2D(GetWorld(), LaunchSound);

	// Play particle effects
	if (LaunchParticleComponent) { LaunchParticleComponent->Activate(true); }
	if (TrailParticleComponent) { TrailParticleComponent->Activate(true); }
	
	if (MeshComponent->GetStaticMesh() != nullptr) { MeshComponent->SetVisibility(true); }
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetEnemy == nullptr) { Destroy(); return; }
	if (bIsLaunched) { MoveTowardsTarget(DeltaTime); }
}

void AProjectile::MoveTowardsTarget(float DeltaTime)
{
	if (!TargetEnemy) { return; }

	// Move towards target enemy
	FVector Direction = TargetEnemy->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;

	if (FVector::Dist(NewLocation, TargetEnemy->GetActorLocation()) < Epsilon) { SetActorLocation(TargetEnemy->GetActorLocation()); Impact(); return; }
	
	SetActorLocation(NewLocation);
	SetActorRotation(GetLookAtRotation(GetActorLocation(), TargetEnemy->GetActorLocation()));
}

void AProjectile::Impact()
{
	if (!TargetEnemy) { return; }
	TargetEnemy->TakeDamage(Damage, FDamageEvent(), nullptr, this);
	if (ImpactParticleComponent) { ImpactParticleComponent->Activate(true); }
	if (ImpactSound) { UGameplayStatics::PlaySound2D(GetWorld(), ImpactSound); }
	Destroy();
}

FRotator AProjectile::GetLookAtRotation(FVector Start, FVector Target)
{
	FVector Direction = Target - Start;
	Direction.Normalize();
	return FRotationMatrix::MakeFromX(Direction).Rotator();
}