// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#include "ForgeTarget.h"
#include "ForgeDirector.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

namespace
{
	constexpr float BruteReach = 220.f;
	constexpr float BruteTell = 0.8f;
	constexpr float BruteDamage = 15.f;
	constexpr float BruteSpeed = 330.f;
	constexpr float RespawnDelay = 4.f;
}

AForgeTarget::AForgeTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (Cylinder.Succeeded())
	{
		Mesh->SetStaticMesh(Cylinder.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.8f));
}

void AForgeTarget::Setup(AForgeDirector* InDirector, bool bInBrute)
{
	Director = InDirector;
	bBrute = bInBrute;
	MaxHealth = bBrute ? 250.f : 400.f;
	Health = MaxHealth;
	HomeLocation = GetActorLocation();
}

void AForgeTarget::ApplyDamage(float Amount, EForgeElement Element)
{
	if (bDead) { return; }
	Health -= Amount;
	HitFlash = 0.1f;
	if (Element == EForgeElement::Frost) { SlowTime = 2.f; }
	if (Health <= 0.f)
	{
		bDead = true;
		bTelling = false;
		RespawnTimer = RespawnDelay;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void AForgeTarget::Push(const FVector& Delta)
{
	if (bDead) { return; }
	SetActorLocation(GetActorLocation() + FVector(Delta.X, Delta.Y, 0.f), true);
}

void AForgeTarget::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UWorld* World = GetWorld();
	AForgeDirector* Dir = Director.Get();
	if (!World || !Dir) { return; }

	if (bDead)
	{
		RespawnTimer -= DeltaSeconds;
		if (RespawnTimer <= 0.f)
		{
			bDead = false;
			Health = MaxHealth;
			SetActorLocation(HomeLocation);
			SetActorHiddenInGame(false);
			SetActorEnableCollision(true);
		}
		return;
	}

	SlowTime -= DeltaSeconds;
	HitFlash -= DeltaSeconds;
	const FVector Loc = GetActorLocation();

	// health bar and status, drawn as one-frame debug lines
	const FVector BarL = Loc + FVector(0.f, -60.f, 130.f);
	const FVector BarR = Loc + FVector(0.f, 60.f, 130.f);
	DrawDebugLine(World, BarL, BarR, FColor(40, 40, 40), false, -1.f, 0, 6.f);
	DrawDebugLine(World, BarL, FMath::Lerp(BarL, BarR, FMath::Clamp(Health / MaxHealth, 0.f, 1.f)), HitFlash > 0.f ? FColor::White : FColor(200, 205, 215), false, -1.f, 0, 6.f);
	if (SlowTime > 0.f)
	{
		DrawDebugCylinder(World, Loc - FVector(0, 0, 90), Loc - FVector(0, 0, 86), 60.f, 16, ForgeData::ElementColor(EForgeElement::Frost), false, -1.f, 0, 2.f);
	}

	if (!bBrute || Dir->IsForgeOpen()) { return; }

	const FVector PlayerLoc = Dir->GetPlayerLocation();
	FVector To = PlayerLoc - Loc;
	To.Z = 0.f;
	const float Dist = To.Size();
	const float SlowMul = SlowTime > 0.f ? 0.55f : 1.f;
	AttackCooldown -= DeltaSeconds;

	if (bTelling)
	{
		// Light before blade: a red ring grows over the wind-up, then the swing lands.
		TellTime -= DeltaSeconds;
		const float Total = BruteTell * (SlowTime > 0.f ? 1.25f : 1.f);
		const float Prog = FMath::Clamp(1.f - TellTime / Total, 0.f, 1.f);
		DrawDebugCylinder(World, Loc - FVector(0, 0, 90), Loc - FVector(0, 0, 88), FMath::Max(10.f, BruteReach * Prog), 24, FColor(255, 77, 61), false, -1.f, 0, 3.f);
		if (TellTime <= 0.f)
		{
			bTelling = false;
			AttackCooldown = 1.6f;
			if (Dist < BruteReach + 40.f)
			{
				Dir->DamagePlayer(BruteDamage, TEXT("Brute swing (red ring)"));
			}
		}
		return;
	}

	if (Dist > BruteReach * 0.6f)
	{
		SetActorLocation(Loc + To.GetSafeNormal() * BruteSpeed * SlowMul * DeltaSeconds, true);
	}
	if (Dist < BruteReach && AttackCooldown <= 0.f)
	{
		bTelling = true;
		TellTime = BruteTell * (SlowTime > 0.f ? 1.25f : 1.f);
	}
}
