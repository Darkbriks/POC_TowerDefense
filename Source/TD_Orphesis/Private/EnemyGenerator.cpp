#include "EnemyGenerator.h"
#include "Enemy.h"
#include "Path.h"

AEnemyGenerator::AEnemyGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (EnemySettings.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No enemy settings found. Destroying actor."));
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("No enemy settings found. Destroying actor."));
		Destroy();
		return;
	}

	if (!StartPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("No start path found. Destroying actor."));
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("No start path found. Destroying actor."));
		Destroy();
		return;
	}

	for (FEnemySettings EnemySetting : EnemySettings)
	{
		if (!EnemySetting.EnemyClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy class  at index %d is null. Destroying actor."), CurrentEnemyIndex);
			GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, FString::Printf(TEXT("Enemy class at index %d is null. Destroying actor."), CurrentEnemyIndex));
			Destroy();
		}
	}
}

void AEnemyGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentEnemyCount == 0)
	{
		TimeSinceLastSpawn += DeltaTime;
		if (TimeSinceLastSpawn >= EnemySettings[CurrentEnemyIndex].TimeBeforeFirstSpawn)
		{
			TimeSinceLastSpawn -= EnemySettings[CurrentEnemyIndex].TimeBeforeFirstSpawn;
			SpawnEnemy();
		}
		return;
	}

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= EnemySettings[CurrentEnemyIndex].SpawnRate)
	{
		TimeSinceLastSpawn -= EnemySettings[CurrentEnemyIndex].SpawnRate;
		SpawnEnemy();
	}
}

void AEnemyGenerator::SpawnEnemy()
{
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemySettings[CurrentEnemyIndex].EnemyClass, GetActorLocation(), GetActorRotation());
	NewEnemy->InitializeEnemy(StartPath);
	CurrentEnemyCount++;

	if (CurrentEnemyCount >= EnemySettings[CurrentEnemyIndex].EnemyCount)
	{
		CurrentEnemyIndex++;
		CurrentEnemyCount = 0;
		if (CurrentEnemyIndex >= EnemySettings.Num()) { Destroy(); }
	}
}