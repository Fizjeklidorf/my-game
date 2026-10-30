<!-- PROTOTYPE - NOT FOR PRODUCTION -->
<!-- Question: Does building abilities from parts in the Forge feel like inventing, so players keep returning to tweak them? -->
<!-- Date: 2026-10-29 -->

# Concept Prototype Report: Ability Forge (HTML round)

> **Date**: 2026-10-29
> **Prototype Path**: HTML
> **Concept File**: design/gdd/game-concept.md

---

## Hypothesis

If the player builds abilities in the Forge and then fights with them, they will
want to go back and keep tweaking. Signal, with no time limit and no prompting:
the tester returns to the Forge after fighting at least twice by choice, forges 3+
clearly different abilities, and when they stop can describe an ability idea they
still want to build.

The original draft had a 10-minute window; the developer removed it — "there
shouldn't be a need to rush someone trying to create an ability."

---

## Riskiest Assumption Tested

Composing abilities from parts feels like *inventing*, not filling out a
spreadsheet. **Not proven either way** — see Result: the build never made
inventing necessary.

---

## Approach

A single-file browser build: a Forge (blueprints Bolt, Ball, Beam, Shield;
alterations Split, Charge, Chain into a second stage; Energy plus discoverable
Frost; escalating cost for new abilities, 5 XP edits, Grow +15% per level) and a
top-down arena (waves, the Warden boss with lit telegraphs, a practice dummy). An
observer tally (L) recorded the hypothesis signals.

**Path chosen:** HTML
**Reason for path:** the riskiest assumption is about the Forge's choices, not
frame timing; it could be built entirely by the agent and shared instantly. Build
feel was explicitly declared NOT ASSESSED by this path.

**Shortcuts taken (intentional):**
- 2D top-down abstraction of a 3D third-person game
- Hardcoded values, shapes as placeholder art, no audio or menus
- No saving; one element besides Energy

---

## Result

One session by the developer, about two minutes of play (0.7 min in the Forge,
1.3 min fighting). From the tally:

- **New abilities forged: 0.** All XP went into the starter Energy Bolt: Grow ×4
  (to Lv 5) and two edits — `+Split`, then `Frost Bolt +Split ⟶ Ball`.
- **The Warden fell in 40 seconds** to a Lv 3 Split Energy Bolt — before any new
  ability existed.
- **Returned to the Forge by choice: 2** (after the boss, after wave 1). Frost was
  discovered at 01:03 and used in a reforge at 01:29.
- **The build crashed twice** shortly after continuing to wave 4. Root cause: a
  Frost-slowed enemy's wind-up outlasted its warning-ring timing, producing a
  negative radius; the canvas error stopped the game loop. Fixed and stress-tested
  (12 runs, all chain stages, boss, Frost) after the session.
- Developer's assessment: *"This isn't enough to test anything really. There's
  going to be a lot more options to play with than what was available on here."*

---

## Metrics

| Metric | Value |
|--------|-------|
| Path used | HTML |
| Iterations to playable | N/A (one fix round after play) |
| Prototype duration | ~1 hour build; ~2 minutes played |
| Playtesters | 1 internal (the developer) / 0 external |
| Feel assessment | NOT ASSESSED — HTML cannot answer feel by design |
| Hypothesis verdict | NOT ASSESSED — the test was invalidated (see below) |

---

## Recommendation: NOT ASSESSED — played, but the test was not valid

PROCEED, PIVOT and KILL all need evidence about whether forging feels like
inventing, and this session could not produce it. Three things invalidated it:
the crash cut play short; the Warden was weak enough that upgrading the starter
ability beat inventing a new one, so the build never had to change; and four
blueprints with three alterations and one element were too few for a build to
feel like the player's own. None of these is evidence against the concept — they
are defects in the test. The one positive signal (two voluntary Forge returns,
and a Frost discovery turned into a reforge within 26 seconds) is too thin to
carry a verdict.

**Next step (developer's choice):** an Unreal Engine prototype — third-person,
over-the-shoulder camera — that tests the Forge with a much wider parts list and
Innovation vs Manifest as two forge styles, plus build feel.

---

## Creative Director Review (CD-PLAYTEST): CONCERNS — accepted by the developer

The NOT ASSESSED recommendation stands. Concerns carried into the Unreal rounds:

1. **A success signal was met for the wrong reason.** Both Forge returns went to
   Grow and edits of the starter ability. From now on only returns that produce a
   new or structurally changed ability (new blueprint, chain stage or element)
   count; Grow-only visits do not.
2. **Fix Grow by escalating its cost, not by weakening it.** Weakening breaches
   *Limitless Power, Earned* and *Your Class, Your Design*. The pillar-correct
   answer is content that demands variety, plus a crude MATE stand-in (enemies
   gain resistance to the player's most-used blueprint or element).
3. **A wider parts list is not inventing by itself.** Watch the build-calculator
   anti-pillar: parts must differ in play (silhouette, range, pace, hit feel), and
   testers should be able to describe an ability without reading its stats.
4. **Too many variables at once.** Keep the Forge hypothesis primary; give build
   feel and Innovation vs Manifest their own signals. (Addressed by splitting
   Unreal into U1 feel/styles and U2 inventing-under-pressure.)
5. **Tester bias.** The developer designed the parts; get at least one external
   tester and a minimum session (boss beaten, or the player stops twice).

**Next-round validation:** 3+ new abilities forged; Forge returns produce new or
restructured abilities; at least one external tester; content no single grown
ability can solve.

---

## Lessons Learned

- **What assumptions were broken by actually building this?**
  That a small parts list is enough to test "inventing." The developer's vision has
  far more options, and a thin list reads as a spreadsheet by default — so the
  riskiest assumption cannot be tested without a representative option space.
- **What surprised us that didn't show up in the brainstorm?**
  Grow competes directly with inventing. With Grow cheap and strong, the rational
  move is to upgrade one ability forever — the "take everything into one thing"
  failure the creative director flagged, arriving through Grow rather than the
  Forge. Escalating Forge cost alone does not prevent it; Grow's cost curve and
  content that demands variety have to share the job.
- **What would we test differently next time?**
  - Content no single ability can handle: armoured, shielded, swarming and
    ranged enemies; a Warden that takes minutes, not seconds.
  - Grow weaker or pricier, so inventing and upgrading compete.
  - A wider parts list: more blueprints, alterations (multi-stage chains),
    elements with interactions, and Innovation vs Manifest.
  - Stress-test every status effect against every telegraph before handing a
    build over.

---

> *Prototype code location: `prototypes/ability-forge-concept/`*
> *This code is throwaway. Never refactor into production.*
