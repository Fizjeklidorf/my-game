<!-- PROTOTYPE - NOT FOR PRODUCTION -->
<!-- Question: In third person, do Innovation and Manifest abilities feel clearly different, and do two builds made from the same parts play differently? -->
<!-- Date: 2026-10-30 -->

# Ability Forge — Unreal prototype, round U1 (feel + Innovation vs Manifest)

**Status:** in progress — written, not yet compiled or played
**Engine:** Unreal Engine 5.8, C++ (a separate throwaway project, `ForgeProto`)
**Previous round:** `prototypes/ability-forge-concept/REPORT.md` (HTML, NOT ASSESSED)

## Hypothesis

In third person, abilities forged as **Innovation** (tools, equipment, drones:
precise and instant) and as **Manifest** (shaping: travels, bends, lingers) feel
clearly different, and two builds made from the same parts play differently.

Signals:
- watching and using an ability, the tester can tell which style it is without
  being told;
- the tester can describe what their ability does in plain words, without stats;
- two builds from the same parts feel different in the same fight.

Counted per the creative director's review: only Forge visits that produce a new
or structurally changed ability count as "returning to invent"; Grow-only visits
do not. The wide parts list, varied enemies, MATE stand-in, real boss and Grow
rebalance are round **U2**.

## What's in it

| | Parts |
|---|---|
| **Combat** (baseline) | Rifle on LMB, aim zoom on RMB |
| **Innovation** | Pulse Beam (instant, pierces) · Turret (dropped, shoots nearest) · Drone (orbits you) · Mine (placed, bursts on contact) |
| **Manifest** | Seeking Bolt (bends toward enemies) · Orb (drifts, lingers, pulls) · Force Wave (widening shove) · Barrier (softens hits, burns inside) |
| **Alterations** | Split (three at once) · Charge (hold to power up) · Chain stage 2 and 3 (on hit, becomes another part of the same style) |
| **Elements** | Energy · Frost (slows) |
| **Manifest seep** | Raise it in the Forge (0-3): your Innovation abilities start acting like Manifest — shots bend (1), leave lingering motes (2), spark on to a second enemy (3) |

Five dummies and two brutes (they walk at you and swing after a red wind-up ring)
spawn in front of you and respawn four seconds after they fall. You start with
400 XP and two pre-made abilities: an Innovation Pulse Beam (Q) and a Manifest
Seeking Bolt (E). New abilities cost more each time; edits cost 5 XP; Grow's cost
rises each level.

Visuals are debug shapes on purpose: **Innovation draws crisp hard lines and
boxes; Manifest draws clouds of particles and never a hard line.** Colour is the
element (white-gold Energy, cyan Frost); red is only for enemy warnings.

## Setup (Windows, about 30-60 minutes the first time)

1. **Visual Studio 2022** with the C++ game development workload. Unreal needs it
   to compile C++. Follow Epic's "Setting up Visual Studio for Unreal Engine" page
   for 5.8 for the exact components.
2. **Create the project.** Epic Games Launcher → Unreal Engine 5.8 → Launch →
   **Games → Third Person**. Under *Project Defaults* choose **C++** (not
   Blueprint). Name it exactly **`ForgeProto`** — the code expects that name.
   Create it anywhere outside this repo, e.g. `Documents/Unreal Projects/`.
3. **Close the editor** once it has opened.
4. **Copy the code.** Copy the `Source/ForgeProto/Forge/` folder from here into
   your project's `Source/ForgeProto/` folder, so you end up with
   `ForgeProto/Source/ForgeProto/Forge/ForgeDirector.cpp` and so on.
5. **Compile.** Double-click `ForgeProto.uproject`. When it says modules are
   missing or out of date and asks to rebuild, choose **Yes**.
   - If it fails, it will say so. Right-click `ForgeProto.uproject` →
     *Generate Visual Studio project files*, open `ForgeProto.sln`, set the
     configuration to **Development Editor**, and Build. Copy the first few errors
     from the *Error List* (or `ForgeProto/Saved/Logs/ForgeProto.log`) and paste
     them back to Claude.
6. **Place the director.** In the editor: Content Drawer → *C++ Classes* →
   ForgeProto → Forge → drag **ForgeDirector** into the level anywhere.
7. **Play** (Alt+P). Targets appear in front of you.

## Controls

| Key | Action |
|---|---|
| WASD, mouse, Space | Move, look, jump (the template's own controls) |
| LMB / RMB | Rifle / aim (camera pulls in over the shoulder) |
| Q E R F | Your four ability slots (hold the key for Charge abilities) |
| Tab | Open or close the Forge (the fight pauses) |
| In the Forge | Up/Down choose a row · Left/Right change it · Enter to forge, save, grow or raise seep |
| L | Observer tally (also logged to the Output Log as `[ForgeProto]`) |

## Running a playtest

Ideally hand it to someone else. Say only: "Make abilities in the Forge (Tab) and
fight with them; stop when you want." Afterwards ask, without showing stats:
1. "Which of your abilities were tech and which were magic? How could you tell?"
2. "Describe your favourite ability in plain words."
3. "Did any two of your abilities feel alike? Which?"

Then press L and note the tally.

## Known limits

- Debug-shape visuals; no animation, sound or hit reactions — judge style and
  shape language, not polish.
- Extra keys are read by polling (`IsInputKeyDown`), which
  `docs/engine-reference/unreal/modules/input.md` calls quick-and-dirty; the
  template's Enhanced Input still drives movement and camera.
- Uses the template's spring-arm camera. The newer Gameplay Camera System is
  Experimental in 5.8 (`plugins/gameplay-camera-system.md`) and not worth it for
  a throwaway build.
- Not compiled by Claude: this was written in a cloud session with no Unreal
  install. Expect a round or two of compile fixes.

## Findings

_To be filled in after playtesting (`/prototype` Phase 6)._
