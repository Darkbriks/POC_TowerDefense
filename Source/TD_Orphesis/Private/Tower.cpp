#include "Tower.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SceneComponent->Mobility = EComponentMobility::Movable;
	RootComponent = SceneComponent;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCapsuleSize(AttackRange, 2000.0f);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AttackCooldown -= DeltaTime;
	if (AttackCooldown <= 0.0f) { if (Attack()) { AttackCooldown = AttackSpeed; } }
}

bool ATower::Attack()
{
	TArray<AActor*> OverlappingActors;
	CapsuleComponent->GetOverlappingActors(OverlappingActors, AEnemy::StaticClass());

	if (OverlappingActors.Num() > 0)
	{
		AEnemy* Enemy = Cast<AEnemy>(OverlappingActors[0]);
		if (Enemy)
		{
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator);
			if (Projectile)
			{
				Enemy->AddProjectile(Projectile);
				Projectile->InitializeProjectile(Enemy);
				Projectile->Launch();
				return true;
			}
		}
	}
	return false;
}