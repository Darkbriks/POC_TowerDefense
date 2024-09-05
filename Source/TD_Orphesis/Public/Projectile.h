#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UArrowComponent;
class AEnemy;

UCLASS(Blueprintable)
class TD_ORPHESIS_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Attributes")
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Attributes")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Attributes")
    float Epsilon = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Attributes")
	AEnemy* TargetEnemy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
    USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile - Components")
    UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	UParticleSystemComponent* LaunchParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	UParticleSystemComponent* TrailParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	UParticleSystemComponent* ImpactParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	USoundBase* TravelSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile - Components")
	USoundBase* ImpactSound;

	bool bIsInitialized = false;
	bool bIsLaunched = false;
	
public:	
	AProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void InitializeProjectile(AEnemy* NewTargetEnemy);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Launch();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void MoveTowardsTarget(float DeltaTime);

protected:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Impact();

	FRotator GetLookAtRotation(FVector Start, FVector Target);
};
