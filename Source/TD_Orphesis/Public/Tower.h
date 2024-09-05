#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class UCapsuleComponent;
class AEnemy;
class AProjectile;

UCLASS()
class TD_ORPHESIS_API ATower : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower - Attributes")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower - Attributes")
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower - Attributes")
	float AttackRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower - Components")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower - Components")
	UCapsuleComponent* CapsuleComponent;

private:
	float AttackCooldown = 0.0f;
	
public:	
	ATower();
	
	virtual void Tick(float DeltaTime) override;

	bool Attack();
};
