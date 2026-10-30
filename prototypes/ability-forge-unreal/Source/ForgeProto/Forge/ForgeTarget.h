// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForgeTypes.h"
#include "ForgeTarget.generated.h"

class AForgeDirector;
class UStaticMeshComponent;

// A practice dummy, or a "brute" that walks at you and swings after a red wind-up.
UCLASS()
class FORGEPROTO_API AForgeTarget : public AActor
{
	GENERATED_BODY()

public:
	AForgeTarget();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	void Setup(AForgeDirector* InDirector, bool bInBrute);
	void ApplyDamage(float Amount, EForgeElement Element);
	void Push(const FVector& Delta);
	bool IsAlive() const { return !bDead; }
	FVector GetAimPoint() const { return GetActorLocation(); }

	bool bBrute = false;
	float Health = 400.f;
	float MaxHealth = 400.f;
	float SlowTime = 0.f;
	float HitFlash = 0.f;

private:
	TWeakObjectPtr<AForgeDirector> Director;
	FVector HomeLocation = FVector::ZeroVector;
	bool bDead = false;
	float RespawnTimer = 0.f;
	float AttackCooldown = 2.f;
	float TellTime = 0.f;
	bool bTelling = false;
};
