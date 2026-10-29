<!-- PROTOTYPE - NOT FOR PRODUCTION -->
<!-- Question: Does building abilities from parts in the Forge feel like inventing, so players keep returning to tweak them? -->
<!-- Date: 2026-10-29 -->

# Ability Forge — concept prototype (HTML)

**Status:** in progress — awaiting playtests
**Concept:** Web of Wyrd (working title), `design/gdd/game-concept.md`

## Hypothesis

If the player builds abilities in the Forge and then fights with them, they will
want to go back and keep tweaking. Signal, with no time limit and no prompting:

1. the tester returns to the Forge after fighting at least twice, by choice;
2. they forge 3+ clearly different abilities;
3. when they stop, they can describe an ability idea they still want to build.

**Riskiest assumption:** composing abilities from parts feels like inventing, not
filling out a spreadsheet.

**Not assessed by this build:** whether builds *feel* right (weight,
responsiveness). A browser cannot answer that; the Unreal prototype will.

## How to run

Double-click `prototype.html`. No install, no server.

- **Forge:** pick a blueprint (Bolt, Ball, Beam, Shield), alterations (Split,
  Charge, Chain into a second stage), an element (Energy; Frost once found), name
  it, forge it, equip it to LMB / RMB / Q / E. New abilities cost more XP each
  time; editing an existing one costs 5 XP; Grow adds 15% damage.
- **Fight:** WASD to move, mouse to aim, LMB / RMB / Q / E to cast. F returns to
  the Forge at any time. XP is kept even on death.
- **Content:** Waves (an elite on wave 2 carries the Frost shard), The Warden
  boss (slam / volley / charge, each lit before it lands), a practice dummy with
  a damage-per-second readout.

## Running a playtest

Give the tester the file and say only "Make abilities and fight with them; stop
whenever you want." Do not explain the Forge. Watch silently.

When they stop, ask two questions:
1. "What was confusing?"
2. "Is there an ability you still wanted to build? Describe it."

Then press **L**, select everything in the tally box, and copy it. It records
Forge returns, abilities forged, edits, distinct recipes and a time-stamped log,
with the hypothesis checks at the top.

## Findings

_To be filled in at the debrief (`/prototype` Phase 6)._
