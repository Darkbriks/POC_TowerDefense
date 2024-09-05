#include "Enemy.h"

#include "Path.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCharacterMovement()->GravityScale = 0.0f;
	this->bSimGravityDisabled = true;
}

void AEnemy::InitializeEnemy(APath* NewPath, float NewCurrentDistance)
{
	Health = MaxHealth;
	this->CurrentPath = NewPath;
	this->CurrentDistance = NewCurrentDistance;
}

void AEnemy::BeginPlay() { Super::BeginPlay(); }

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAlongPath(DeltaTime);
}

void AEnemy::MoveAlongPath(float DeltaTime)
{
	if (CurrentPath)
	{
		// Get length of spline
		float SplineLength = CurrentPath->GetSplineComponent()->GetSplineLength();

		// Advance the current distance based on speed and delta time
		CurrentDistance += Speed * DeltaTime;

		if (CurrentDistance > SplineLength)
		{
			CurrentPath = CurrentPath->GetNextPath();
			if (!CurrentPath) { Die(); return; }
			CurrentDistance -= SplineLength;
		}

		// Set actor location to spline location at current distance
		FVector Location = CurrentPath->GetSplineComponent()->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
		Location.Z += ZOffset;
		SetActorLocation(Location);

		FRotator Rotation = CurrentPath->GetSplineComponent()->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
		SetActorRotation(Rotation);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.0f) { Die(); }
	return DamageAmount;
}

void AEnemy::Die()
{
	for (AProjectile* Projectile : Projectiles) { Projectile->Destroy(); }
	Destroy();
}