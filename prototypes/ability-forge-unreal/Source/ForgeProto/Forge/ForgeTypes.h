// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#pragma once

#include "CoreMinimal.h"

// Innovation = tools, equipment, drones: precise and instant (Mass Effect "Tech").
// Manifest  = shaping: travels, bends, lingers (Mass Effect "Biotics").
enum class EForgeStyle : uint8 { Innovation, Manifest };

enum class EForgePart : uint8
{
	PulseBeam, Turret, Drone, Mine,          // Innovation
	SeekingBolt, Orb, ForceWave, Barrier     // Manifest
};

enum class EForgeElement : uint8 { Energy, Frost };

struct FForgeAbility
{
	FString Name;
	EForgeStyle Style = EForgeStyle::Innovation;
	EForgePart Part = EForgePart::PulseBeam;
	bool bSplit = false;
	bool bCharge = false;
	TArray<EForgePart> Stages; // chained stages after the main part (0-2), same style
	EForgeElement Element = EForgeElement::Energy;
	int32 Level = 1;
};

namespace ForgeData
{
	inline EForgeStyle StyleOf(EForgePart P)
	{
		return static_cast<uint8>(P) <= static_cast<uint8>(EForgePart::Mine) ? EForgeStyle::Innovation : EForgeStyle::Manifest;
	}

	inline const TCHAR* StyleName(EForgeStyle S) { return S == EForgeStyle::Innovation ? TEXT("Innovation") : TEXT("Manifest"); }
	inline const TCHAR* ElementName(EForgeElement E) { return E == EForgeElement::Frost ? TEXT("Frost") : TEXT("Energy"); }

	inline const TCHAR* PartName(EForgePart P)
	{
		switch (P)
		{
		case EForgePart::PulseBeam:   return TEXT("Pulse Beam");
		case EForgePart::Turret:      return TEXT("Turret");
		case EForgePart::Drone:       return TEXT("Drone");
		case EForgePart::Mine:        return TEXT("Mine");
		case EForgePart::SeekingBolt: return TEXT("Seeking Bolt");
		case EForgePart::Orb:         return TEXT("Orb");
		case EForgePart::ForceWave:   return TEXT("Force Wave");
		case EForgePart::Barrier:     return TEXT("Barrier");
		}
		return TEXT("?");
	}

	// Plain words, no numbers: a tester should be able to say what an ability does without stats.
	inline const TCHAR* PartPlain(EForgePart P)
	{
		switch (P)
		{
		case EForgePart::PulseBeam:   return TEXT("an instant beam that pierces everything along your aim");
		case EForgePart::Turret:      return TEXT("a turret dropped where you aim that shoots the nearest enemy");
		case EForgePart::Drone:       return TEXT("a drone that circles you and shoots what you aim near");
		case EForgePart::Mine:        return TEXT("a mine placed where you aim that bursts when an enemy steps close");
		case EForgePart::SeekingBolt: return TEXT("a bolt that bends toward the nearest enemy in front of it");
		case EForgePart::Orb:         return TEXT("a slow orb that drifts to your aim point and lingers, pulling enemies in");
		case EForgePart::ForceWave:   return TEXT("a widening wave that shoves everything it passes");
		case EForgePart::Barrier:     return TEXT("a sphere around you that softens hits and burns anything inside");
		}
		return TEXT("?");
	}

	inline bool CanBeStage(EForgePart P)
	{
		return P != EForgePart::Turret && P != EForgePart::Drone && P != EForgePart::Barrier;
	}

	inline TArray<EForgePart> PartsOf(EForgeStyle S)
	{
		if (S == EForgeStyle::Innovation)
		{
			return { EForgePart::PulseBeam, EForgePart::Turret, EForgePart::Drone, EForgePart::Mine };
		}
		return { EForgePart::SeekingBolt, EForgePart::Orb, EForgePart::ForceWave, EForgePart::Barrier };
	}

	inline TArray<EForgePart> StagePartsOf(EForgeStyle S)
	{
		TArray<EForgePart> Out;
		for (EForgePart P : PartsOf(S))
		{
			if (CanBeStage(P)) { Out.Add(P); }
		}
		return Out;
	}

	inline float BaseDamage(EForgePart P)
	{
		switch (P)
		{
		case EForgePart::PulseBeam:   return 30.f;
		case EForgePart::Turret:      return 8.f;
		case EForgePart::Drone:       return 7.f;
		case EForgePart::Mine:        return 45.f;
		case EForgePart::SeekingBolt: return 18.f;
		case EForgePart::Orb:         return 6.f;
		case EForgePart::ForceWave:   return 20.f;
		case EForgePart::Barrier:     return 5.f;
		}
		return 0.f;
	}

	inline float Cooldown(EForgePart P)
	{
		switch (P)
		{
		case EForgePart::PulseBeam:   return 1.0f;
		case EForgePart::Turret:      return 6.0f;
		case EForgePart::Drone:       return 7.0f;
		case EForgePart::Mine:        return 3.0f;
		case EForgePart::SeekingBolt: return 0.6f;
		case EForgePart::Orb:         return 4.0f;
		case EForgePart::ForceWave:   return 2.5f;
		case EForgePart::Barrier:     return 8.0f;
		}
		return 1.f;
	}

	inline FColor ElementColor(EForgeElement E)
	{
		return E == EForgeElement::Frost ? FColor(127, 216, 255) : FColor(255, 233, 168);
	}

	inline FString AutoName(const FForgeAbility& A)
	{
		FString N = FString(ElementName(A.Element)) + TEXT(" ");
		if (A.bCharge) { N += TEXT("Charged "); }
		if (A.bSplit) { N += TEXT("Split "); }
		N += PartName(A.Part);
		for (EForgePart S : A.Stages) { N += FString(TEXT(" > ")) + PartName(S); }
		return N;
	}

	inline FString Describe(const FForgeAbility& A)
	{
		FString D = FString(TEXT("Fires ")) + PartPlain(A.Part);
		if (A.bSplit) { D += TEXT(", three at once"); }
		if (A.bCharge) { D += TEXT(", stronger the longer you hold"); }
		for (int32 i = 0; i < A.Stages.Num(); ++i)
		{
			D += FString(TEXT("; on hit it becomes ")) + PartPlain(A.Stages[i]);
		}
		if (A.Element == EForgeElement::Frost) { D += TEXT(". Frost slows what it touches"); }
		return D + TEXT(".");
	}

	inline FString Signature(const FForgeAbility& A)
	{
		FString S = FString::Printf(TEXT("%d|%d|%d|%d"), static_cast<int32>(A.Part), A.bSplit ? 1 : 0, A.bCharge ? 1 : 0, static_cast<int32>(A.Element));
		for (EForgePart P : A.Stages) { S += FString::Printf(TEXT(">%d"), static_cast<int32>(P)); }
		return S;
	}
}
