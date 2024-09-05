#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AProjectile;
class APath;

UENUM()
enum EEnemyType
{
	Attack,
	Support,
	Tank
};

UCLASS(Blueprintable)
class TD_ORPHESIS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

protected:
	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Attributes")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Attributes")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Attributes")
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Attributes")
	TEnumAsByte<EEnemyType> EnemyType = EEnemyType::Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Movement")
	float ZOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Movement")
	float CurrentDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy - Movement")
	APath* CurrentPath = nullptr;

public:
	AEnemy();

	void InitializeEnemy(APath* NewPath, float NewCurrentDistance = 0.0f);

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Blueprintable, Category = "Enemy - Attributes")
	float GetHealthPercentage() const { return Health / MaxHealth; }
	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void AddProjectile(AProjectile* NewProjectile) { Projectiles.Add(NewProjectile); }

protected:
	virtual void BeginPlay() override;

	void MoveAlongPath(float DeltaTime);

	void Die();

private:
	float Health;
	TArray<AProjectile*> Projectiles;
};
