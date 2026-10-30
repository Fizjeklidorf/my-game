// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#include "ForgeDirector.h"
#include "ForgeTarget.h"
#include "ForgeHUD.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

namespace
{
	// Key polling instead of Enhanced Input assets: quick and dirty, prototype only
	// (docs/engine-reference/unreal/modules/input.md, "Check if Key Pressed").
	const FKey& SlotKey(int32 i)
	{
		static const FKey Keys[4] = { EKeys::Q, EKeys::E, EKeys::R, EKeys::F };
		return Keys[i];
	}
	const TCHAR* SlotNames[4] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("F") };

	FVector Flat(const FVector& V) { return FVector(V.X, V.Y, 0.f); }

	float SegmentDistance(const FVector& P, const FVector& A, const FVector& B)
	{
		const FVector AB = B - A;
		const float L = AB.SizeSquared();
		const float T = L > 0.f ? FMath::Clamp(FVector::DotProduct(P - A, AB) / L, 0.f, 1.f) : 0.f;
		return FVector::Dist(P, A + AB * T);
	}
}

AForgeDirector::AForgeDirector()
{
	PrimaryActorTick.bCanEverTick = true;
}

// ------------------------------------------------------------------ setup

void AForgeDirector::Init(APlayerController* PC, APawn* Pawn)
{
	bInit = true;
	PC->ClientSetHUD(AForgeHUD::StaticClass());

	// Pre-made starting points (a class's basic kit), built from the same blueprints the player uses.
	FForgeAbility Beam;
	Beam.Style = EForgeStyle::Innovation;
	Beam.Part = EForgePart::PulseBeam;
	Beam.Name = ForgeData::AutoName(Beam);
	Library.Add(Beam);

	FForgeAbility Bolt;
	Bolt.Style = EForgeStyle::Manifest;
	Bolt.Part = EForgePart::SeekingBolt;
	Bolt.Name = ForgeData::AutoName(Bolt);
	Library.Add(Bolt);

	Slots[0] = 0;
	Slots[1] = 1;
	Signatures.Add(ForgeData::Signature(Beam));
	Signatures.Add(ForgeData::Signature(Bolt));

	Draft = FForgeAbility();
	Draft.Name = ForgeData::AutoName(Draft);
	SpawnTargets(Pawn);
	Log(TEXT("SESSION START (starter kit: Energy Pulse Beam on Q, Energy Seeking Bolt on E)"));
}

void AForgeDirector::SpawnTargets(APawn* Pawn)
{
	UWorld* World = GetWorld();
	const FVector Base = Pawn->GetActorLocation();
	const FRotator Yaw(0.f, Pawn->GetActorRotation().Yaw, 0.f);
	const FVector Fwd = Yaw.Vector();
	const FVector Right = FRotationMatrix(Yaw).GetScaledAxis(EAxis::Y);

	struct FSpot { float F; float R; bool bBrute; };
	const FSpot Spots[] = {
		{ 900.f, -500.f, false }, { 1100.f, 0.f, false }, { 900.f, 500.f, false },
		{ 1500.f, -300.f, false }, { 1500.f, 300.f, false },
		{ 2000.f, -700.f, true }, { 2100.f, 700.f, true }
	};
	for (const FSpot& S : Spots)
	{
		const FVector Loc = Base + Fwd * S.F + Right * S.R;
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (AForgeTarget* T = World->SpawnActor<AForgeTarget>(AForgeTarget::StaticClass(), Loc, FRotator::ZeroRotator, Params))
		{
			T->Setup(this, S.bBrute);
			Targets.Add(T);
		}
	}
}

void AForgeDirector::Log(const FString& Line)
{
	const int32 Secs = FMath::FloorToInt(TimeForge + TimeCombat);
	const FString Stamped = FString::Printf(TEXT("%02d:%02d  %s"), Secs / 60, Secs % 60, *Line);
	EventLog.Add(Stamped);
	LastEvent = Line;
	UE_LOG(LogTemp, Log, TEXT("[ForgeProto] %s"), *Stamped);
}

// ------------------------------------------------------------------ input

void AForgeDirector::PollKeys(APlayerController* PC)
{
	static const FKey Watched[] = {
		EKeys::LeftMouseButton, EKeys::RightMouseButton, EKeys::Q, EKeys::E, EKeys::R, EKeys::F,
		EKeys::Tab, EKeys::L, EKeys::T, EKeys::Up, EKeys::Down, EKeys::Left, EKeys::Right, EKeys::Enter
	};
	KeyPrev = KeyNow;
	for (const FKey& K : Watched)
	{
		KeyNow.Add(K, PC->IsInputKeyDown(K));
	}
}

bool AForgeDirector::Down(const FKey& K) const
{
	const bool* V = KeyNow.Find(K);
	return V && *V;
}

bool AForgeDirector::Pressed(const FKey& K) const
{
	const bool* P = KeyPrev.Find(K);
	return Down(K) && !(P && *P);
}

bool AForgeDirector::Released(const FKey& K) const
{
	const bool* P = KeyPrev.Find(K);
	return !Down(K) && (P && *P);
}

// ------------------------------------------------------------------ tick

void AForgeDirector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UWorld* World = GetWorld();
	APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
	APawn* Pawn = PC ? PC->GetPawn() : nullptr;
	if (!PC || !Pawn) { return; }
	if (!bInit) { Init(PC, Pawn); }
	if (AForgeHUD* Hud = Cast<AForgeHUD>(PC->GetHUD())) { Hud->Director = this; }

	PlayerLocation = Pawn->GetActorLocation();
	PollKeys(PC);

	if (Pressed(EKeys::Tab)) { ToggleForge(PC); }
	if (Pressed(EKeys::L)) { bShowTally = !bShowTally; }

	if (bForgeOpen)
	{
		TimeForge += DeltaSeconds;
		ForgeInput();
	}
	else
	{
		TimeCombat += DeltaSeconds;
		UpdateCombat(DeltaSeconds, PC, Pawn);
		UpdateShots(DeltaSeconds);
		UpdateDevices(DeltaSeconds);
		Shots.Append(PendingShots);
		Devices.Append(PendingDevices);
		PendingShots.Reset();
		PendingDevices.Reset();
	}
	UpdateCamera(DeltaSeconds, Pawn);
	DrawWorld(bForgeOpen ? 0.f : DeltaSeconds);
}

void AForgeDirector::DamagePlayer(float Amount, const FString& Source)
{
	if (bForgeOpen) { return; }
	const float Taken = bBarrierUp ? Amount * 0.3f : Amount;
	PlayerHP -= Taken;
	LastEvent = FString::Printf(TEXT("Hit by %s for %.0f%s"), *Source, Taken, bBarrierUp ? TEXT(" (Barrier softened it)") : TEXT(""));
	if (PlayerHP <= 0.f)
	{
		PlayerHP = 100.f;
		++Downs;
		Log(FString::Printf(TEXT("DOWNED by %s (health reset, nothing lost)"), *Source));
	}
}

// ------------------------------------------------------------------ the Forge

int32 AForgeDirector::ForgeCost() const { return FMath::RoundToInt(30.f * FMath::Pow(1.3f, static_cast<float>(Forged))); }
int32 AForgeDirector::GrowCost() const
{
	if (!Library.IsValidIndex(EditIndex)) { return 0; }
	return FMath::RoundToInt(25.f * FMath::Pow(1.6f, static_cast<float>(Library[EditIndex].Level - 1)));
}
int32 AForgeDirector::SeepCost() const { return 120 * (ManifestSeep + 1); }

void AForgeDirector::ToggleForge(APlayerController* PC)
{
	bForgeOpen = !bForgeOpen;
	if (bForgeOpen)
	{
		++ForgeOpens;
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
		LoadDraftFromRow();
		Log(TEXT("OPENED the Forge"));
	}
	else
	{
		PC->ResetIgnoreLookInput();
		PC->ResetIgnoreMoveInput();
		for (int32 i = 0; i < 4; ++i) { SlotCharge[i] = -1.f; }
		Log(TEXT("CLOSED the Forge"));
	}
}

void AForgeDirector::LoadDraftFromRow()
{
	if (Library.IsValidIndex(EditIndex))
	{
		Draft = Library[EditIndex];
		DraftSlot = 0;
		for (int32 i = 0; i < 4; ++i) { if (Slots[i] == EditIndex) { DraftSlot = i; break; } }
	}
	else
	{
		EditIndex = -1;
		Draft = FForgeAbility();
		Draft.Name = ForgeData::AutoName(Draft);
	}
}

TArray<AForgeDirector::FRow> AForgeDirector::BuildForgeRows() const
{
	TArray<FRow> R;
	R.Add({ TEXT("Working on"), Library.IsValidIndex(EditIndex) ? Library[EditIndex].Name : FString(TEXT("NEW ability")) });
	R.Add({ TEXT("Style"), ForgeData::StyleName(Draft.Style) });
	R.Add({ TEXT("Blueprint"), ForgeData::PartName(Draft.Part) });
	R.Add({ TEXT("Split"), Draft.bSplit ? TEXT("On: three at once, each weaker") : TEXT("Off") });
	R.Add({ TEXT("Charge"), Draft.bCharge ? TEXT("On: hold the key, release to cast") : TEXT("Off") });
	R.Add({ TEXT("Chain stage 2"), Draft.Stages.Num() > 0 ? ForgeData::PartName(Draft.Stages[0]) : TEXT("None") });
	R.Add({ TEXT("Chain stage 3"), Draft.Stages.Num() > 1 ? ForgeData::PartName(Draft.Stages[1]) : (Draft.Stages.Num() > 0 ? TEXT("None") : TEXT("(needs stage 2)")) });
	R.Add({ TEXT("Element"), ForgeData::ElementName(Draft.Element) });
	R.Add({ TEXT("Slot"), SlotNames[DraftSlot] });
	R.Add({ TEXT(">> "), Library.IsValidIndex(EditIndex)
		? FString::Printf(TEXT("SAVE CHANGES (%d XP)"), EditCost)
		: FString::Printf(TEXT("FORGE NEW ABILITY (%d XP)"), ForgeCost()) });
	R.Add({ TEXT(">> "), Library.IsValidIndex(EditIndex)
		? FString::Printf(TEXT("GROW to Lv %d (%d XP)"), Library[EditIndex].Level + 1, GrowCost())
		: FString(TEXT("GROW (pick an existing ability first)")) });
	R.Add({ TEXT(">> "), ManifestSeep >= MaxSeep
		? FString(TEXT("MANIFEST SEEP is at its peak (3/3)"))
		: FString::Printf(TEXT("RAISE MANIFEST SEEP to %d/3 (%d XP)"), ManifestSeep + 1, SeepCost()) });
	return R;
}

void AForgeDirector::ForgeInput()
{
	const int32 NumRows = 12;
	if (Pressed(EKeys::Up)) { ForgeRow = (ForgeRow + NumRows - 1) % NumRows; }
	if (Pressed(EKeys::Down)) { ForgeRow = (ForgeRow + 1) % NumRows; }
	if (Pressed(EKeys::Left)) { ForgeChange(-1); }
	if (Pressed(EKeys::Right)) { ForgeChange(1); }
	if (Pressed(EKeys::Enter)) { ForgeEnter(); }
}

void AForgeDirector::ForgeChange(int32 Dir)
{
	auto Cycle = [Dir](int32 Index, int32 Count) { return (Index + Dir + Count) % Count; };
	switch (ForgeRow)
	{
	case 0:
	{
		const int32 Count = Library.Num() + 1; // -1 (new) .. Num-1
		EditIndex = Cycle(EditIndex + 1, Count) - 1;
		LoadDraftFromRow();
		return;
	}
	case 1:
		Draft.Style = Draft.Style == EForgeStyle::Innovation ? EForgeStyle::Manifest : EForgeStyle::Innovation;
		Draft.Part = ForgeData::PartsOf(Draft.Style)[0];
		Draft.Stages.Empty();
		break;
	case 2:
	{
		const TArray<EForgePart> Parts = ForgeData::PartsOf(Draft.Style);
		Draft.Part = Parts[Cycle(Parts.IndexOfByKey(Draft.Part), Parts.Num())];
		break;
	}
	case 3: Draft.bSplit = !Draft.bSplit; break;
	case 4: Draft.bCharge = !Draft.bCharge; break;
	case 5:
	case 6:
	{
		const int32 StageIdx = ForgeRow - 5;
		if (StageIdx == 1 && Draft.Stages.Num() == 0) { return; }
		const TArray<EForgePart> Parts = ForgeData::StagePartsOf(Draft.Style);
		const int32 Count = Parts.Num() + 1; // None + parts
		const int32 Cur = Draft.Stages.IsValidIndex(StageIdx) ? Parts.IndexOfByKey(Draft.Stages[StageIdx]) + 1 : 0;
		const int32 Next = Cycle(Cur, Count);
		if (Next == 0)
		{
			Draft.Stages.SetNum(StageIdx); // removing stage 2 also removes stage 3
		}
		else
		{
			if (Draft.Stages.Num() <= StageIdx) { Draft.Stages.SetNum(StageIdx + 1); }
			Draft.Stages[StageIdx] = Parts[Next - 1];
		}
		break;
	}
	case 7: Draft.Element = Draft.Element == EForgeElement::Energy ? EForgeElement::Frost : EForgeElement::Energy; break;
	case 8: DraftSlot = Cycle(DraftSlot, 4); return;
	default: return;
	}
	Draft.Name = ForgeData::AutoName(Draft);
}

void AForgeDirector::ForgeEnter()
{
	if (ForgeRow == 9)
	{
		if (Library.IsValidIndex(EditIndex))
		{
			if (XP < EditCost) { LastEvent = TEXT("Not enough XP to save changes"); return; }
			const FString Before = ForgeData::Signature(Library[EditIndex]);
			const int32 KeepLevel = Library[EditIndex].Level;
			XP -= EditCost;
			Library[EditIndex] = Draft;
			Library[EditIndex].Level = KeepLevel;
			Slots[DraftSlot] = EditIndex;
			++Edits;
			const FString After = ForgeData::Signature(Library[EditIndex]);
			if (Before != After) { ++StructuralEdits; Signatures.Add(After); }
			Log(FString::Printf(TEXT("EDITED -> \"%s\" on %s%s"), *Draft.Name, SlotNames[DraftSlot], Before != After ? TEXT(" (structural change)") : TEXT("")));
		}
		else
		{
			const int32 Cost = ForgeCost();
			if (XP < Cost) { LastEvent = TEXT("Not enough XP to forge"); return; }
			XP -= Cost;
			++Forged;
			++NewForged;
			Library.Add(Draft);
			EditIndex = Library.Num() - 1;
			Slots[DraftSlot] = EditIndex;
			Signatures.Add(ForgeData::Signature(Draft));
			Log(FString::Printf(TEXT("FORGED \"%s\" for %d XP on %s"), *Draft.Name, Cost, SlotNames[DraftSlot]));
		}
	}
	else if (ForgeRow == 10 && Library.IsValidIndex(EditIndex))
	{
		const int32 Cost = GrowCost();
		if (XP < Cost) { LastEvent = TEXT("Not enough XP to grow"); return; }
		XP -= Cost;
		Library[EditIndex].Level += 1;
		Draft.Level = Library[EditIndex].Level;
		++Grows;
		Log(FString::Printf(TEXT("GREW \"%s\" to Lv %d for %d XP"), *Library[EditIndex].Name, Library[EditIndex].Level, Cost));
	}
	else if (ForgeRow == 11 && ManifestSeep < MaxSeep)
	{
		const int32 Cost = SeepCost();
		if (XP < Cost) { LastEvent = TEXT("Not enough XP to raise Manifest seep"); return; }
		XP -= Cost;
		++ManifestSeep;
		Log(FString::Printf(TEXT("MANIFEST SEEP raised to %d/3"), ManifestSeep));
	}
}

// ------------------------------------------------------------------ combat

void AForgeDirector::UpdateCombat(float Dt, APlayerController* PC, APawn* Pawn)
{
	UWorld* World = GetWorld();
	FVector CamLoc;
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Pawn);
	Params.AddIgnoredActor(this);
	const FVector End = CamLoc + CamRot.Vector() * 6000.f;
	AimPoint = World->LineTraceSingleByChannel(Hit, CamLoc, End, ECC_Visibility, Params) ? Hit.Location : End;

	const FVector Right = FRotationMatrix(CamRot).GetScaledAxis(EAxis::Y);
	Muzzle = Pawn->GetActorLocation() + FVector(0.f, 0.f, 50.f) + Right * 35.f;

	bAiming = Down(EKeys::RightMouseButton);
	RifleCooldown -= Dt;
	FaceCameraTimer -= Dt;

	// Combat baseline: a plain rifle on LMB (the Mass Effect "weapons" column).
	if (Down(EKeys::LeftMouseButton) && RifleCooldown <= 0.f)
	{
		RifleCooldown = 0.15f;
		FaceCameraTimer = 0.4f;
		Fx.Add({ Muzzle, AimPoint, FColor(150, 150, 160), 0.06f, 0.06f, 0.f, false, false });
		if (AForgeTarget* T = Cast<AForgeTarget>(Hit.GetActor()))
		{
			T->ApplyDamage(6.f, EForgeElement::Energy);
			XP += 1.2f;
		}
	}

	for (int32 i = 0; i < 4; ++i)
	{
		SlotCooldown[i] = FMath::Max(0.f, SlotCooldown[i] - Dt);
		if (!Library.IsValidIndex(Slots[i])) { continue; }
		const FForgeAbility& Ab = Library[Slots[i]];
		if (Ab.bCharge)
		{
			if (Pressed(SlotKey(i)) && SlotCooldown[i] <= 0.f) { SlotCharge[i] = 0.f; }
			if (SlotCharge[i] >= 0.f && Down(SlotKey(i))) { SlotCharge[i] = FMath::Min(1.f, SlotCharge[i] + Dt); }
			if (SlotCharge[i] >= 0.f && Released(SlotKey(i)))
			{
				CastSlot(i, 1.f + 1.5f * SlotCharge[i]);
				SlotCharge[i] = -1.f;
			}
		}
		else if (Pressed(SlotKey(i)) && SlotCooldown[i] <= 0.f)
		{
			CastSlot(i, 1.f);
		}
	}
}

void AForgeDirector::UpdateCamera(float Dt, APawn* Pawn)
{
	// Over-the-shoulder, Warframe / Mass Effect style; right mouse pulls in to aim.
	if (USpringArmComponent* Arm = Pawn->FindComponentByClass<USpringArmComponent>())
	{
		Arm->SocketOffset = FMath::VInterpTo(Arm->SocketOffset, bAiming ? FVector(0.f, 70.f, 55.f) : FVector(0.f, 60.f, 60.f), Dt, 10.f);
		Arm->TargetArmLength = FMath::FInterpTo(Arm->TargetArmLength, bAiming ? 150.f : 280.f, Dt, 10.f);
	}
	if (UCameraComponent* Cam = Pawn->FindComponentByClass<UCameraComponent>())
	{
		Cam->SetFieldOfView(FMath::FInterpTo(Cam->FieldOfView, bAiming ? 62.f : 85.f, Dt, 10.f));
	}
	const bool bFace = !bForgeOpen && (bAiming || FaceCameraTimer > 0.f);
	if (ACharacter* Char = Cast<ACharacter>(Pawn))
	{
		Char->GetCharacterMovement()->bOrientRotationToMovement = !bFace;
	}
	if (bFace)
	{
		Pawn->SetActorRotation(FRotator(0.f, CamRot.Yaw, 0.f));
	}
}

void AForgeDirector::CastSlot(int32 Slot, float Mult)
{
	const FForgeAbility Ab = Library[Slots[Slot]];
	SlotCooldown[Slot] = ForgeData::Cooldown(Ab.Part) * (Ab.bCharge ? 1.2f : 1.f);
	FaceCameraTimer = 0.6f;
	(Ab.Style == EForgeStyle::Innovation ? CastsInnovation : CastsManifest) += 1;

	const FVector Dir = (AimPoint - Muzzle).GetSafeNormal();
	const float SplitMul = Ab.bSplit ? 0.6f : 1.f;
	const int32 Count = Ab.bSplit ? 3 : 1;
	for (int32 k = 0; k < Count; ++k)
	{
		const float YawOff = Ab.bSplit ? (k - 1) * 10.f : 0.f;
		const FVector D = Dir.RotateAngleAxis(YawOff, FVector::UpVector);
		const FVector P = Muzzle + D * FVector::Dist(Muzzle, AimPoint);
		Launch(Ab.Part, Ab, 0, Muzzle, D, Ab.bSplit ? P : AimPoint, Mult * SplitMul, nullptr);
	}
}

void AForgeDirector::Launch(EForgePart Part, const FForgeAbility& Ab, int32 Stage, const FVector& Origin, const FVector& Dir, const FVector& Point, float DmgMult, AForgeTarget* Exclude)
{
	const float Dmg = ForgeData::BaseDamage(Part) * (1.f + 0.15f * (Ab.Level - 1)) * DmgMult * (Ab.Element == EForgeElement::Frost ? 0.85f : 1.f);
	const FColor Col = ForgeData::ElementColor(Ab.Element);
	const float Scale = FMath::Clamp(DmgMult, 0.6f, 2.5f);

	switch (Part)
	{
	case EForgePart::PulseBeam:
	{
		FVector End = Origin + Dir * 3000.f;
		// Manifest seep 1+: Innovation precision starts to bend toward a target near the aim line.
		if (ManifestSeep >= 1)
		{
			if (AForgeTarget* Bend = NearestTarget(Origin, 3000.f, Exclude, &Dir, FMath::Cos(FMath::DegreesToRadians(15.f))))
			{
				End = Bend->GetAimPoint();
			}
		}
		AForgeTarget* First = nullptr;
		float FirstDist = TNumericLimits<float>::Max();
		for (AForgeTarget* T : Targets)
		{
			if (!T || !T->IsAlive() || T == Exclude) { continue; }
			if (SegmentDistance(T->GetAimPoint(), Origin, End) > 70.f * Scale) { continue; }
			HitTarget(T, Dmg, Ab, Stage);
			const float D = FVector::Dist(Origin, T->GetAimPoint());
			if (D < FirstDist) { FirstDist = D; First = T; }
		}
		Fx.Add({ Origin, End, Col, 0.18f, 0.18f, 4.f * Scale, false, false });
		if (First) { TriggerStage(Ab, Stage, First->GetAimPoint(), First); }
		break;
	}
	case EForgePart::Turret:
	case EForgePart::Drone:
	case EForgePart::Mine:
	case EForgePart::Barrier:
	{
		FDevice D;
		D.Part = Part;
		D.Ab = Ab;
		D.Stage = Stage;
		D.Damage = Dmg;
		D.Pos = Part == EForgePart::Barrier ? PlayerLocation : FVector(Point.X, Point.Y, PlayerLocation.Z - 60.f);
		D.Life = Part == EForgePart::Turret ? 8.f : Part == EForgePart::Drone ? 10.f : Part == EForgePart::Mine ? 20.f : 5.f;
		D.Arm = Part == EForgePart::Mine ? 0.6f : 0.f;
		D.Angle = FMath::FRandRange(0.f, 2.f * PI);
		PendingDevices.Add(D);
		break;
	}
	case EForgePart::SeekingBolt:
	case EForgePart::Orb:
	case EForgePart::ForceWave:
	{
		FShot S;
		S.Part = Part;
		S.Ab = Ab;
		S.Stage = Stage;
		S.Pos = Origin;
		S.Damage = Dmg;
		const float Speed = Part == EForgePart::SeekingBolt ? 1400.f : Part == EForgePart::Orb ? 600.f : 1200.f;
		S.Vel = Dir * Speed;
		S.Life = Part == EForgePart::SeekingBolt ? 2.5f : Part == EForgePart::Orb ? FMath::Clamp(FVector::Dist(Origin, Point) / Speed, 0.3f, 2.5f) : 0.8f;
		S.MaxLife = S.Life;
		if (Exclude) { S.Hit.Add(Exclude); }
		PendingShots.Add(S);
		break;
	}
	}
}

void AForgeDirector::TriggerStage(const FForgeAbility& Ab, int32 Stage, const FVector& Point, AForgeTarget* Exclude)
{
	if (!Ab.Stages.IsValidIndex(Stage)) { return; }
	AForgeTarget* Next = NearestTarget(Point, 1500.f, Exclude);
	FVector Dir = Next ? (Next->GetAimPoint() - Point).GetSafeNormal() : FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f).Vector();
	const FVector Aim = Next ? Next->GetAimPoint() : Point + Dir * 600.f;
	Fx.Add({ Point, Point, ForgeData::ElementColor(Ab.Element), 0.2f, 0.2f, 45.f, Ab.Style == EForgeStyle::Manifest, true });
	Launch(Ab.Stages[Stage], Ab, Stage + 1, Point, Dir, Aim, FMath::Pow(0.7f, static_cast<float>(Stage + 1)), Exclude);
}

void AForgeDirector::HitTarget(AForgeTarget* T, float Damage, const FForgeAbility& Ab, int32 Stage)
{
	if (!T || !T->IsAlive()) { return; }
	T->ApplyDamage(Damage, Ab.Element);
	XP += Damage / 5.f;

	// Manifest seep on Innovation abilities (main hits only, to keep it readable).
	if (Ab.Style != EForgeStyle::Innovation || Stage != 0) { return; }
	if (ManifestSeep >= 2)
	{
		FDevice Mote;
		Mote.Part = EForgePart::Orb;
		Mote.bMote = true;
		Mote.Ab = Ab;
		Mote.Pos = T->GetAimPoint();
		Mote.Life = 1.5f;
		Mote.Damage = 3.f;
		PendingDevices.Add(Mote);
	}
	if (ManifestSeep >= 3)
	{
		if (AForgeTarget* Other = NearestTarget(T->GetAimPoint(), 1200.f, T))
		{
			FForgeAbility Spark;
			Spark.Style = EForgeStyle::Manifest;
			Spark.Part = EForgePart::SeekingBolt;
			Spark.Element = Ab.Element;
			Launch(EForgePart::SeekingBolt, Spark, 99, T->GetAimPoint(), (Other->GetAimPoint() - T->GetAimPoint()).GetSafeNormal(), Other->GetAimPoint(), 0.4f * Damage / ForgeData::BaseDamage(EForgePart::SeekingBolt), T);
		}
	}
}

AForgeTarget* AForgeDirector::NearestTarget(const FVector& From, float MaxDist, AForgeTarget* Exclude, const FVector* ConeDir, float ConeCos) const
{
	AForgeTarget* Best = nullptr;
	float BestD = MaxDist;
	for (AForgeTarget* T : Targets)
	{
		if (!T || !T->IsAlive() || T == Exclude) { continue; }
		const FVector To = T->GetAimPoint() - From;
		const float D = To.Size();
		if (D >= BestD) { continue; }
		if (ConeDir && FVector::DotProduct(To.GetSafeNormal(), *ConeDir) < ConeCos) { continue; }
		BestD = D;
		Best = T;
	}
	return Best;
}

void AForgeDirector::UpdateShots(float Dt)
{
	for (int32 i = Shots.Num() - 1; i >= 0; --i)
	{
		FShot& S = Shots[i];
		S.Life -= Dt;

		if (S.Part == EForgePart::SeekingBolt)
		{
			const FVector Fwd = S.Vel.GetSafeNormal();
			if (AForgeTarget* T = NearestTarget(S.Pos, 2000.f, nullptr, &Fwd, 0.6f))
			{
				const FVector Want = (T->GetAimPoint() - S.Pos).GetSafeNormal();
				S.Vel = (Fwd + Want * 5.f * Dt).GetSafeNormal() * S.Vel.Size();
			}
		}
		S.Pos += S.Vel * Dt;

		bool bDone = S.Life <= 0.f;
		const float Radius = S.Part == EForgePart::ForceWave ? 60.f + (1.f - S.Life / S.MaxLife) * 320.f : 70.f;
		for (AForgeTarget* T : Targets)
		{
			if (bDone || !T || !T->IsAlive() || S.Hit.Contains(T)) { continue; }
			if (FVector::Dist(Flat(T->GetAimPoint()), Flat(S.Pos)) > Radius || FMath::Abs(T->GetAimPoint().Z - S.Pos.Z) > 200.f) { continue; }
			S.Hit.Add(T);
			if (S.Part == EForgePart::Orb) { bDone = true; break; } // orb stops at the first body it meets
			HitTarget(T, S.Damage, S.Ab, S.Stage);
			if (S.Part == EForgePart::ForceWave)
			{
				T->Push(S.Vel.GetSafeNormal() * 250.f);
				if (!S.bStaged) { S.bStaged = true; TriggerStage(S.Ab, S.Stage, T->GetAimPoint(), T); }
			}
			else
			{
				if (S.Stage < 99) { TriggerStage(S.Ab, S.Stage, T->GetAimPoint(), T); }
				bDone = true;
			}
		}

		if (bDone)
		{
			if (S.Part == EForgePart::Orb)
			{
				// The orb arrives and lingers as a field that pulls and burns.
				FDevice Field;
				Field.Part = EForgePart::Orb;
				Field.Ab = S.Ab;
				Field.Stage = S.Stage;
				Field.Pos = S.Pos;
				Field.Life = 3.f;
				Field.Damage = S.Damage;
				PendingDevices.Add(Field);
				TriggerStage(S.Ab, S.Stage, S.Pos, nullptr);
			}
			Shots.RemoveAt(i);
		}
	}
}

void AForgeDirector::UpdateDevices(float Dt)
{
	bBarrierUp = false;
	for (int32 i = Devices.Num() - 1; i >= 0; --i)
	{
		FDevice& D = Devices[i];
		D.Life -= Dt;
		D.Timer -= Dt;
		D.StageCooldown -= Dt;
		const FColor Col = ForgeData::ElementColor(D.Ab.Element);
		bool bDone = D.Life <= 0.f;

		switch (D.Part)
		{
		case EForgePart::Turret:
		case EForgePart::Drone:
		{
			if (D.Part == EForgePart::Drone)
			{
				D.Angle += Dt * 2.f;
				D.Pos = PlayerLocation + FVector(FMath::Cos(D.Angle) * 130.f, FMath::Sin(D.Angle) * 130.f, 110.f);
			}
			if (D.Timer <= 0.f)
			{
				D.Timer = D.Part == EForgePart::Turret ? 0.5f : 0.6f;
				const FVector Gun = D.Part == EForgePart::Turret ? D.Pos + FVector(0, 0, 70.f) : D.Pos;
				AForgeTarget* T = D.Part == EForgePart::Turret ? NearestTarget(Gun, 1500.f, nullptr) : NearestTarget(AimPoint, 1800.f, nullptr);
				if (T)
				{
					HitTarget(T, D.Damage, D.Ab, D.Stage);
					Fx.Add({ Gun, T->GetAimPoint(), Col, 0.08f, 0.08f, 2.f, false, false });
					if (D.StageCooldown <= 0.f && D.Ab.Stages.IsValidIndex(D.Stage))
					{
						D.StageCooldown = 1.f;
						TriggerStage(D.Ab, D.Stage, T->GetAimPoint(), T);
					}
				}
			}
			break;
		}
		case EForgePart::Mine:
		{
			D.Arm -= Dt;
			if (D.Arm <= 0.f && NearestTarget(D.Pos, 200.f, nullptr))
			{
				for (AForgeTarget* T : Targets)
				{
					if (T && T->IsAlive() && FVector::Dist(Flat(T->GetAimPoint()), Flat(D.Pos)) < 300.f) { HitTarget(T, D.Damage, D.Ab, D.Stage); }
				}
				Fx.Add({ D.Pos, D.Pos, Col, 0.25f, 0.25f, 300.f, false, true });
				TriggerStage(D.Ab, D.Stage, D.Pos + FVector(0, 0, 60.f), nullptr);
				bDone = true;
			}
			break;
		}
		case EForgePart::Barrier:
		{
			D.Pos = PlayerLocation;
			bBarrierUp = true;
			if (D.Timer <= 0.f)
			{
				D.Timer = 0.5f;
				for (AForgeTarget* T : Targets)
				{
					if (T && T->IsAlive() && FVector::Dist(T->GetAimPoint(), D.Pos) < 220.f) { HitTarget(T, D.Damage, D.Ab, D.Stage); }
				}
			}
			break;
		}
		case EForgePart::Orb: // lingering orb field, or a seep mote
		{
			const float R = D.bMote ? 120.f : 250.f;
			if (D.Timer <= 0.f)
			{
				D.Timer = 0.3f;
				for (AForgeTarget* T : Targets)
				{
					if (!T || !T->IsAlive()) { continue; }
					const FVector To = Flat(D.Pos - T->GetAimPoint());
					if (To.Size() > R) { continue; }
					T->ApplyDamage(D.Damage, D.Ab.Element);
					XP += D.Damage / 5.f;
					if (!D.bMote) { T->Push(To.GetSafeNormal() * 45.f); }
				}
			}
			break;
		}
		default:
			break;
		}

		if (bDone) { Devices.RemoveAt(i); }
	}
}

// ------------------------------------------------------------------ drawing

void AForgeDirector::DrawStyledLine(const FVector& A, const FVector& B, EForgeStyle Style, FColor Col, float Thickness)
{
	UWorld* World = GetWorld();
	if (Style == EForgeStyle::Innovation)
	{
		// Innovation: crisp hard light. Manifest seep adds drifting motes along it.
		DrawDebugLine(World, A, B, Col, false, -1.f, 0, Thickness);
		DrawDebugLine(World, A, B, FColor::White, false, -1.f, 0, FMath::Max(1.f, Thickness * 0.3f));
		for (int32 k = 0; k < ManifestSeep * 4; ++k)
		{
			DrawDebugPoint(World, FMath::Lerp(A, B, FMath::FRand()) + FMath::VRand() * 20.f, 7.f, FColor(190, 140, 255), false, -1.f, 0);
		}
	}
	else
	{
		// Manifest: particulate and flowing, never a hard line.
		const int32 N = FMath::Clamp(FMath::RoundToInt(FVector::Dist(A, B) / 40.f), 4, 60);
		for (int32 k = 0; k <= N; ++k)
		{
			DrawDebugPoint(World, FMath::Lerp(A, B, static_cast<float>(k) / N) + FMath::VRand() * 12.f, 9.f, Col, false, -1.f, 0);
		}
	}
}

void AForgeDirector::DrawManifestCloud(const FVector& C, float Radius, FColor Col, int32 Count)
{
	UWorld* World = GetWorld();
	for (int32 k = 0; k < Count; ++k)
	{
		DrawDebugPoint(World, C + FMath::VRand() * Radius * FMath::Sqrt(FMath::FRand()), 8.f, Col, false, -1.f, 0);
	}
}

void AForgeDirector::DrawWorld(float Dt)
{
	UWorld* World = GetWorld();

	// crosshair point in the world, so you can see where abilities will land
	if (!bForgeOpen) { DrawDebugPoint(World, AimPoint, 6.f, FColor::White, false, -1.f, 1); }

	for (const FShot& S : Shots)
	{
		const FColor Col = ForgeData::ElementColor(S.Ab.Element);
		if (S.Part == EForgePart::ForceWave)
		{
			const float R = 60.f + (1.f - S.Life / S.MaxLife) * 320.f;
			const FVector Side = FVector::CrossProduct(S.Vel.GetSafeNormal(), FVector::UpVector);
			for (int32 k = -10; k <= 10; ++k)
			{
				DrawDebugPoint(World, S.Pos + Side * (R * k / 10.f) + FMath::VRand() * 15.f, 10.f, Col, false, -1.f, 0);
			}
		}
		else
		{
			const float R = S.Part == EForgePart::Orb ? 45.f : 18.f;
			DrawDebugSphere(World, S.Pos, R, 8, Col, false, -1.f, 0, 1.f);
			DrawManifestCloud(S.Pos, R * 1.6f, Col, S.Part == EForgePart::Orb ? 18 : 8);
			DrawStyledLine(S.Pos - S.Vel.GetSafeNormal() * 120.f, S.Pos, EForgeStyle::Manifest, Col, 1.f);
		}
	}

	for (const FDevice& D : Devices)
	{
		const FColor Col = ForgeData::ElementColor(D.Ab.Element);
		switch (D.Part)
		{
		case EForgePart::Turret:
			DrawDebugBox(World, D.Pos + FVector(0, 0, 35.f), FVector(25.f, 25.f, 35.f), Col, false, -1.f, 0, 3.f);
			DrawDebugLine(World, D.Pos + FVector(0, 0, 70.f), D.Pos + FVector(0, 0, 70.f) + FRotator(0.f, D.Life * 90.f, 0.f).Vector() * 45.f, FColor::White, false, -1.f, 0, 3.f);
			break;
		case EForgePart::Drone:
			DrawDebugBox(World, D.Pos, FVector(16.f, 16.f, 6.f), Col, false, -1.f, 0, 3.f);
			break;
		case EForgePart::Mine:
			DrawDebugBox(World, D.Pos + FVector(0, 0, 5.f), FVector(22.f, 22.f, 5.f), D.Arm > 0.f ? FColor(120, 120, 120) : Col, false, -1.f, 0, 3.f);
			break;
		case EForgePart::Barrier:
			DrawDebugSphere(World, D.Pos, 200.f, 12, Col, false, -1.f, 0, 0.5f);
			DrawManifestCloud(D.Pos, 210.f, Col, 30);
			break;
		case EForgePart::Orb:
			DrawManifestCloud(D.Pos, D.bMote ? 120.f : 250.f, D.bMote ? FColor(190, 140, 255) : Col, D.bMote ? 10 : 40);
			break;
		default:
			break;
		}
	}

	for (int32 i = Fx.Num() - 1; i >= 0; --i)
	{
		FFx& F = Fx[i];
		if (F.bBurst)
		{
			const float R = FMath::Max(10.f, F.Radius * (1.f - F.Life / F.MaxLife));
			if (F.bManifest) { DrawManifestCloud(F.A, R, F.Col, 20); }
			else { DrawDebugBox(World, F.A, FVector(R, R, 10.f), F.Col, false, -1.f, 0, 3.f); }
		}
		else
		{
			DrawStyledLine(F.A, F.B, EForgeStyle::Innovation, F.Col, F.Radius);
		}
		F.Life -= Dt;
		if (F.Life <= 0.f) { Fx.RemoveAt(i); }
	}
}

// ------------------------------------------------------------------ tally

TArray<FString> AForgeDirector::TallyLines() const
{
	TArray<FString> L;
	L.Add(FString::Printf(TEXT("Time in Forge / combat: %.1f / %.1f min"), TimeForge / 60.f, TimeCombat / 60.f));
	L.Add(FString::Printf(TEXT("Forge opens: %d"), ForgeOpens));
	L.Add(FString::Printf(TEXT("New abilities forged: %d"), NewForged));
	L.Add(FString::Printf(TEXT("Edits: %d (structural: %d)   Grows: %d"), Edits, StructuralEdits, Grows));
	L.Add(FString::Printf(TEXT("Distinct recipes (incl. 2 starters): %d"), Signatures.Num()));
	L.Add(FString::Printf(TEXT("Casts: Innovation %d / Manifest %d"), CastsInnovation, CastsManifest));
	L.Add(FString::Printf(TEXT("Manifest seep: %d/3   Downed: %d"), ManifestSeep, Downs));
	L.Add(TEXT("U1 signals (ask the tester, no stats on screen):"));
	L.Add(TEXT("  - Which style was that? Can they tell by watching and using it?"));
	L.Add(TEXT("  - Describe your ability in plain words."));
	L.Add(TEXT("  - Same parts, two builds: do they play differently?"));
	L.Add(TEXT("Log (also in the Output Log, category LogTemp):"));
	const int32 Start = FMath::Max(0, EventLog.Num() - 14);
	for (int32 i = Start; i < EventLog.Num(); ++i) { L.Add(TEXT("  ") + EventLog[i]); }
	return L;
}
