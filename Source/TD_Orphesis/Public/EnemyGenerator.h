#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyGenerator.generated.h"

class APath;
class AEnemy;

USTRUCT(BlueprintType)
struct FEnemySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int EnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBeforeFirstSpawn;
};

UCLASS(Blueprintable)
class TD_ORPHESIS_API AEnemyGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyGenerator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Generator")
	TArray<FEnemySettings> EnemySettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Generator")
	APath* StartPath;
	
	float TimeSinceLastSpawn = 0.0f;
	int CurrentEnemyIndex = 0;
	int CurrentEnemyCount = 0;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();
};
