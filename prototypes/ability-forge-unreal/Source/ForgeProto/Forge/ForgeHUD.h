// PROTOTYPE - NOT FOR PRODUCTION
// Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently?
// Date: 2026-10-30

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ForgeHUD.generated.h"

class AForgeDirector;

// Draws the Forge panel, the combat readout and the tally with canvas text — no UMG assets needed.
UCLASS()
class FORGEPROTO_API AForgeHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	TWeakObjectPtr<AForgeDirector> Director;

private:
	void Line(const FString& Text, float X, float& Y, const FLinearColor& Color, float Scale = 1.f);
};
