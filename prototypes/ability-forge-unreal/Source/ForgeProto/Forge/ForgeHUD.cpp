// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#include "ForgeHUD.h"
#include "ForgeDirector.h"
#include "ForgeTypes.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

namespace
{
	const FLinearColor Gold(1.f, 0.91f, 0.66f);
	const FLinearColor Grey(0.6f, 0.63f, 0.68f);
	const FLinearColor White(0.9f, 0.9f, 0.92f);
	const TCHAR* SlotLabels[4] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("F") };
}

void AForgeHUD::Line(const FString& Text, float X, float& Y, const FLinearColor& Color, float Scale)
{
	DrawText(Text, Color, X, Y, GEngine ? GEngine->GetMediumFont() : nullptr, Scale);
	Y += 20.f * Scale;
}

void AForgeHUD::DrawHUD()
{
	Super::DrawHUD();
	AForgeDirector* D = Director.Get();
	if (!D || !Canvas) { return; }
	const float W = Canvas->ClipX;
	const float H = Canvas->ClipY;

	if (!D->bForgeOpen)
	{
		// crosshair
		const float Cx = W * 0.5f, Cy = H * 0.5f, S = D->bAiming ? 4.f : 8.f;
		DrawRect(White, Cx - S, Cy - 1.f, S * 2.f, 2.f);
		DrawRect(White, Cx - 1.f, Cy - S, 2.f, S * 2.f);

		// health, XP, seep
		DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.6f), 20.f, H - 150.f, 300.f, 130.f);
		DrawRect(FLinearColor(0.35f, 0.15f, 0.15f), 30.f, H - 140.f, 280.f, 12.f);
		DrawRect(FLinearColor(0.88f, 0.35f, 0.29f), 30.f, H - 140.f, 280.f * FMath::Clamp(D->PlayerHP / 100.f, 0.f, 1.f), 12.f);
		float Y = H - 122.f;
		Line(FString::Printf(TEXT("XP %d    Manifest seep %d/3"), FMath::FloorToInt(D->XP), D->ManifestSeep), 30.f, Y, Gold);
		Line(TEXT("LMB rifle   RMB aim   Q E R F abilities"), 30.f, Y, Grey, 0.9f);
		Line(TEXT("Tab: the Forge   L: tally"), 30.f, Y, Grey, 0.9f);
		Line(D->LastEvent, 30.f, Y, White, 0.9f);

		// ability slots
		for (int32 i = 0; i < 4; ++i)
		{
			const float X = 340.f + i * 230.f;
			DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.6f), X, H - 90.f, 220.f, 70.f);
			float SY = H - 84.f;
			Line(SlotLabels[i], X + 8.f, SY, Grey, 0.9f);
			if (!D->Library.IsValidIndex(D->Slots[i])) { Line(TEXT("empty"), X + 8.f, SY, Grey, 0.9f); continue; }
			const FForgeAbility& Ab = D->Library[D->Slots[i]];
			const FLinearColor Col = FLinearColor(ForgeData::ElementColor(Ab.Element));
			Line(Ab.Name.Left(26), X + 8.f, SY, Col, 0.9f);
			Line(FString(ForgeData::StyleName(Ab.Style)) + FString::Printf(TEXT("  Lv %d"), Ab.Level), X + 8.f, SY, Grey, 0.8f);
			if (D->SlotCooldown[i] > 0.f)
			{
				const float Full = ForgeData::Cooldown(Ab.Part) * (Ab.bCharge ? 1.2f : 1.f);
				DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.55f), X, H - 90.f, 220.f * FMath::Clamp(D->SlotCooldown[i] / Full, 0.f, 1.f), 70.f);
			}
			if (D->SlotCharge[i] >= 0.f)
			{
				DrawRect(Col, X, H - 24.f, 220.f * D->SlotCharge[i], 4.f);
			}
		}
	}
	else
	{
		// the Forge panel
		const float PX = W * 0.5f - 420.f, PY = 60.f;
		DrawRect(FLinearColor(0.06f, 0.07f, 0.09f, 0.92f), PX, PY, 840.f, 560.f);
		float Y = PY + 14.f;
		Line(FString::Printf(TEXT("THE FORGE        XP %d"), FMath::FloorToInt(D->XP)), PX + 20.f, Y, Gold, 1.3f);
		Line(TEXT("Up/Down: choose a row   Left/Right: change it   Enter: act   Tab: back to the fight"), PX + 20.f, Y, Grey, 0.9f);
		Y += 8.f;
		const TArray<AForgeDirector::FRow> Rows = D->BuildForgeRows();
		for (int32 i = 0; i < Rows.Num(); ++i)
		{
			const bool bSel = i == D->ForgeRow;
			if (bSel) { DrawRect(FLinearColor(0.3f, 0.25f, 0.12f, 0.8f), PX + 14.f, Y - 2.f, 812.f, 22.f); }
			const FString Text = Rows[i].Label.StartsWith(TEXT(">>"))
				? Rows[i].Label + Rows[i].Value
				: FString::Printf(TEXT("%-16s  < %s >"), *Rows[i].Label, *Rows[i].Value);
			Line(Text, PX + 24.f, Y, bSel ? Gold : White);
			if (i == 8) { Y += 6.f; }
		}
		Y += 10.f;
		Line(TEXT("What it does:"), PX + 20.f, Y, Grey);
		// wrap the plain-words description
		FString Desc = ForgeData::Describe(D->Draft);
		while (Desc.Len() > 0)
		{
			int32 Cut = Desc.Len();
			if (Cut > 95) { Cut = 95; while (Cut > 40 && Desc[Cut] != TEXT(' ')) { --Cut; } }
			Line(Desc.Left(Cut).TrimStartAndEnd(), PX + 30.f, Y, FLinearColor(ForgeData::ElementColor(D->Draft.Element)));
			Desc = Desc.Mid(Cut);
		}
		Y += 6.f;
		Line(TEXT("Innovation = tools, equipment, drones: precise and instant.  Manifest = shaping: it travels, bends and lingers."), PX + 20.f, Y, Grey, 0.85f);
		Line(TEXT("Manifest seep makes your Innovation start to behave like Manifest: shots bend (1), leave motes (2), spark onward (3)."), PX + 20.f, Y, Grey, 0.85f);
		Line(D->LastEvent, PX + 20.f, Y, White, 0.9f);
	}

	if (D->bShowTally)
	{
		const float TX = W - 520.f;
		DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.85f), TX, 20.f, 500.f, 520.f);
		float Y = 30.f;
		Line(TEXT("OBSERVER TALLY (L to hide)"), TX + 12.f, Y, Gold);
		for (const FString& L : D->TallyLines()) { Line(L, TX + 12.f, Y, White, 0.8f); }
	}
}
