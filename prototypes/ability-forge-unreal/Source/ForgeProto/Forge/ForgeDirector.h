// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"
#include "ForgeTypes.h"
#include "ForgeDirector.generated.h"

class AForgeTarget;
class APlayerController;
class APawn;

// One actor runs the whole prototype: input, the Forge, abilities, camera and targets.
// Drag it into the Third Person level and press Play.
UCLASS()
class FORGEPROTO_API AForgeDirector : public AActor
{
	GENERATED_BODY()

public:
	AForgeDirector();

	virtual void Tick(float DeltaSeconds) override;

	bool IsForgeOpen() const { return bForgeOpen; }
	FVector GetPlayerLocation() const { return PlayerLocation; }
	void DamagePlayer(float Amount, const FString& Source);

	// ---- read by the HUD ----
	struct FRow { FString Label; FString Value; };
	TArray<FRow> BuildForgeRows() const;
	int32 ForgeRow = 0;
	bool bForgeOpen = false;
	bool bShowTally = false;
	bool bAiming = false;
	float XP = 400.f;
	float PlayerHP = 100.f;
	int32 ManifestSeep = 0;
	FString LastEvent;
	FForgeAbility Draft;
	int32 EditIndex = -1;
	int32 DraftSlot = 0;
	TArray<FForgeAbility> Library;
	int32 Slots[4] = { -1, -1, -1, -1 };
	float SlotCooldown[4] = { 0.f, 0.f, 0.f, 0.f };
	float SlotCharge[4] = { -1.f, -1.f, -1.f, -1.f };
	TArray<FString> TallyLines() const;
	int32 ForgeCost() const;
	int32 GrowCost() const;
	int32 SeepCost() const;
	static constexpr int32 EditCost = 5;
	static constexpr int32 MaxSeep = 3;

private:
	// ---- simulated things (plain structs, drawn with debug shapes) ----
	struct FShot
	{
		EForgePart Part = EForgePart::SeekingBolt;
		FForgeAbility Ab;
		int32 Stage = 0;
		FVector Pos = FVector::ZeroVector;
		FVector Vel = FVector::ZeroVector;
		float Life = 1.f;
		float MaxLife = 1.f;
		float Damage = 0.f;
		bool bStaged = false;
		TArray<TWeakObjectPtr<AForgeTarget>> Hit;
	};
	struct FDevice
	{
		EForgePart Part = EForgePart::Turret; // Orb here means the lingering orb field
		bool bMote = false;                   // Manifest seep mote field
		FForgeAbility Ab;
		int32 Stage = 0;
		FVector Pos = FVector::ZeroVector;
		float Life = 1.f;
		float Timer = 0.f;
		float Arm = 0.f;
		float Angle = 0.f;
		float Damage = 0.f;
		float StageCooldown = 0.f;
	};
	struct FFx
	{
		FVector A = FVector::ZeroVector;
		FVector B = FVector::ZeroVector;
		FColor Col = FColor::White;
		float Life = 0.2f;
		float MaxLife = 0.2f;
		float Radius = 0.f;
		bool bManifest = false;
		bool bBurst = false;
	};

	void Init(APlayerController* PC, APawn* Pawn);
	void PollKeys(APlayerController* PC);
	bool Down(const FKey& K) const;
	bool Pressed(const FKey& K) const;
	bool Released(const FKey& K) const;

	void ToggleForge(APlayerController* PC);
	void ForgeInput();
	void ForgeChange(int32 Dir);
	void ForgeEnter();
	void LoadDraftFromRow();

	void UpdateCombat(float Dt, APlayerController* PC, APawn* Pawn);
	void UpdateCamera(float Dt, APawn* Pawn);
	void CastSlot(int32 Slot, float Mult);
	void Launch(EForgePart Part, const FForgeAbility& Ab, int32 Stage, const FVector& Origin, const FVector& Dir, const FVector& Point, float DmgMult, AForgeTarget* Exclude);
	void TriggerStage(const FForgeAbility& Ab, int32 Stage, const FVector& Point, AForgeTarget* Exclude);
	void HitTarget(AForgeTarget* T, float Damage, const FForgeAbility& Ab, int32 Stage);
	void UpdateShots(float Dt);
	void UpdateDevices(float Dt);
	void DrawWorld(float Dt);
	void DrawStyledLine(const FVector& A, const FVector& B, EForgeStyle Style, FColor Col, float Thickness);
	void DrawManifestCloud(const FVector& C, float Radius, FColor Col, int32 Count);
	AForgeTarget* NearestTarget(const FVector& From, float MaxDist, AForgeTarget* Exclude, const FVector* ConeDir = nullptr, float ConeCos = -1.f) const;
	void SpawnTargets(APawn* Pawn);
	void Log(const FString& Line);

	UPROPERTY()
	TArray<TObjectPtr<AForgeTarget>> Targets;

	TArray<FShot> Shots;
	TArray<FDevice> Devices;
	// New shots/devices land here first, so nothing grows an array that is being looped over.
	TArray<FShot> PendingShots;
	TArray<FDevice> PendingDevices;
	TArray<FFx> Fx;

	TMap<FKey, bool> KeyNow;
	TMap<FKey, bool> KeyPrev;
	bool bInit = false;
	FVector PlayerLocation = FVector::ZeroVector;
	FVector AimPoint = FVector::ZeroVector;
	FVector Muzzle = FVector::ZeroVector;
	FRotator CamRot = FRotator::ZeroRotator;
	float RifleCooldown = 0.f;
	float FaceCameraTimer = 0.f;
	int32 Forged = 0;
	bool bBarrierUp = false;

	// tally (press L)
	int32 ForgeOpens = 0;
	int32 NewForged = 0;
	int32 Edits = 0;
	int32 StructuralEdits = 0;
	int32 Grows = 0;
	int32 Downs = 0;
	int32 CastsInnovation = 0;
	int32 CastsManifest = 0;
	float TimeForge = 0.f;
	float TimeCombat = 0.f;
	TSet<FString> Signatures;
	TArray<FString> EventLog;
};
