# Game Concept: Web of Wyrd (working title)

*Created: 2026-10-28*
*Status: Draft*

---

## Elevator Pitch

> It's an action RPG where you **engineer your own classes — and your own party —
> in an Ability Forge**, then prove them across a mission-driven sci-fi universe
> and a shattered high-fantasy world, until both worlds converge into one.

**Rationale.** The creative director's review (CD-PILLARS) found the Forge to be
the stronger half of a two-part hook, so the pitch leads with it and frames the
two worlds as its proving ground. *Alternative considered:* leading with the two
worlds ("Mass Effect meets Elden Ring") — more trailer-friendly, but it buries
what no comparable game offers.

---

## Core Identity

| Aspect | Detail |
| ---- | ---- |
| **Genre** | Action RPG — build-crafting looter-RPG with souls-like challenge; test phase is a procedurally generated roguelike |
| **Platform** | PC (Steam / Epic) |
| **Target Audience** | Build-crafters and power-fantasy players from Warframe, Destiny and souls-likes (see Target Player Profile) |
| **Player Count** | Single-player with a self-crafted AI party; online co-op, player-hosted. Co-op launches at 4, growing to 5 (fantasy) and 10 (sci-fi, role-based) |
| **Session Length** | 30–120 min (a roguelike run, 2–3 missions, or a realm push, plus Forge time) |
| **Monetization** | Undecided — Premium / Early Access assumed; a founders pack is an open question (see Open Questions) |
| **Estimated Scope** | Large (66–132+ months to the Paths Split tier, solo, part-time; 1.0 Convergence not yet estimable) |
| **Comparable Titles** | Warframe, Elden Ring, Path of Exile (also: The First Descendant, Mass Effect, Black Desert, Destiny) |

---

## Core Fantasy

**"I'm the only one who plays like this, because I invented it."**

The player doesn't pick a class — they author one, and then a whole party of
them, and prove those creations against challenge that adapts to meet them. The
emotional promise is the sci-fi power fantasy of Warframe and Destiny (striking,
distinct characters that grow for years) with one difference: every one of those
characters is the player's own design, and none of them ever waits on a
developer's release schedule.

---

## Unique Hook

**It's like Warframe, AND ALSO you invent every class from blueprints — and the
game invents counters to your inventions.**

- **The Ability Forge**: a library of blueprints for any attack, defence or
  movement type (bolt, ball, beam, shield, dash…). Abilities are created with
  experience, altered and chained into staged abilities, and grow as more is
  invested. Cost escalates with use, eventually exponentially: nothing is
  barred, but everything is a commitment.
- **MATE (Multiverse Adaptability Trait Evolution)**: enemies counter players
  who lean too hard on one thing, using the same creation system players use.
- **Two worlds, then one**: a sci-fi mission universe and a shattered fantasy
  world, each with its own upgrade layer, converge at 1.0 into one shared world
  where both layers stack.

---

## Player Experience Analysis (MDA Framework)

### Target Aesthetics (What the player FEELS)

| Aesthetic | Priority | How We Deliver It |
| ---- | ---- | ---- |
| **Sensation** (sensory pleasure) | 6 | Ability VFX that read as the player's own creation; weighty or fluid feel per build |
| **Fantasy** (make-believe, role-playing) | 3 | Sci-fi power fantasy; commanding a self-designed party; Innovation vs Manifest identity |
| **Narrative** (drama, story arc) | 7 | Manifest seeping into the sci-fi story; reconnecting shattered realms; the convergence as climax |
| **Challenge** (obstacle course, mastery) | 2 | Souls-like bosses; difficulty scaling by party size, chosen difficulty and invested experience; MATE |
| **Fellowship** (social connection) | 4 | Coordinated co-op with interlocking roles; sci-fi per-role objectives in one mission |
| **Discovery** (exploration, secrets) | 5 | Discovering elements and damage types; exploring realms; procedural runs |
| **Expression** (self-expression, creativity) | 1 | The Ability Forge, custom classes, crafted parties |
| **Submission** (relaxation, comfort zone) | N/A | Not a goal — the game is built to test builds |

### Key Dynamics (Emergent player behaviors)

- Players theorycraft and iterate: build an ability, test it, tweak a stage, retest.
- Players specialise for identity, because escalating Forge cost punishes "take everything".
- Players adapt when MATE counters them — reworking builds instead of waiting for a patch.
- Players design parties with complementary roles (tank, support, breach, control).
- Players share builds and blueprint recipes with the community.
- In sci-fi co-op, groups split roles across parallel objectives (e.g. Recon removes disruptors while the main party pushes).

### Core Mechanics (Systems we build)

1. **Ability Forge** — open blueprint library; alterations and chaining into staged abilities; experience-funded creation and growth; escalating cost; energy as base fuel; discoverable elements and damage types; Innovation (tech) and Manifest (magic) flavours.
2. **Upgrade layers** — Mods (sci-fi) and Runes/Enchants (fantasy) that modify stats but never grant abilities; gear with rarity-bounded stat ranges up to Onyx (uncapped, heavy experience cost); upgrades accept experience up to their rarity cap.
3. **Adaptive challenge** — difficulty from party size, selected difficulty and invested experience; MATE counters (curated variants first, generative later).
4. **Parties** — multiple player-made characters; crafted AI parties for solo play; online co-op with role-based objectives.
5. **World structures** — roguelike test runs from a central hub → fantasy realms and sci-fi missions → converged shared world at 1.0.

---

## Player Motivation Profile

### Primary Psychological Needs Served

| Need | How This Game Satisfies It | Strength |
| ---- | ---- | ---- |
| **Autonomy** (freedom, meaningful choice) | Authoring classes, abilities and whole parties; no class-locked abilities; feel follows build | Core |
| **Competence** (mastery, skill growth) | Souls-like challenge proves a build works; readable failure teaches why it didn't; Onyx progression | Core |
| **Relatedness** (connection, belonging) | Co-op with interlocking roles; sharing builds; a party of your own characters | Supporting |

### Player Type Appeal (Bartle Taxonomy)

- [x] **Achievers** (goal completion, collection, progression) — How: Onyx gear, ability growth, element discovery, endgame missions, beating MATE's counters.
- [x] **Explorers** (discovery, understanding systems, finding secrets) — How: mastering the Forge's systems; exploring realms; discovering elements.
- [x] **Socializers** (relationships, cooperation, community) — How: coordinated co-op; sci-fi role objectives; build sharing.
- [ ] **Killers/Competitors** (domination, PvP, leaderboards) — Not targeted: no PvP planned. Competitive drive is served by hard PvE and MATE.

Quantic Foundry: primary **Creativity (Design, Customization)**, secondary **Mastery (Challenge, Strategy)**.

### Flow State Design

- **Onboarding curve**: start with a pre-made class or role whose abilities are visibly built from Forge blueprints; the first Forge visit edits one stage of an ability the player already uses, then tests it in a short run.
- **Difficulty scaling**: party size, selected difficulty and invested experience set the challenge. Players must out-grow the scaling on average — scaling sets a floor, not a treadmill. MATE raises pressure only on over-reliance.
- **Feedback clarity**: readable damage types, resistances and enemy tells; MATE adaptations are signposted (the player sees what adapted and to what); ability shape and colour show blueprint and element (Visual Identity Anchor).
- **Recovery from failure**: roguelike runs are short and restartable; fantasy rest points hold a Forge; failure is educational — the player knows which part of the build fell short.

---

## Core Loop

### Moment-to-Moment (30 seconds)
Fighting with abilities the player designed. The reward is seeing your own idea
work — a chained staged beam landing exactly as planned. Depth comes from energy
management, elements and reading enemy tells. Pace and weight come from the
build: a fast, mobile caster and a heavy, committed bruiser play very differently
in the same encounter (Feel Follows Build).

### Short-Term (5-15 minutes)
Encounter → reward → tweak. A roguelike room or floor, a sci-fi mission
objective, or a fantasy ruin, camp or elite pays out experience, mods or runes and
sometimes a new element. The pull is "let me try my tweak": change a stage, swap a
rune, re-test. The key choice is where experience goes — forge new or grow existing.

### Session-Level (30-120 minutes)
- **Test builds**: hub → one or more procedural runs → Forge at the hub.
- **Sci-fi**: hub → 2–3 missions → Forge at the hub.
- **Fantasy**: explore a realm → dungeon or shard-holder → rest point with a Forge.

Natural stopping points are the hub and rest points; the hook between sessions is
an unbuilt ability idea or an untried element.

### Long-Term Progression
Abilities grow with investment; mods, runes and gear climb rarity toward Onyx;
elements widen what the Forge can build; Manifest creeps into the sci-fi story;
more characters join the player's party. Realms are reconnected to the shattered
world and missions escalate to endgame objectives. At 1.0 the worlds converge and
both upgrade layers stack. The endgame is open-ended: hybrid builds, hardest
content, coordinated operations.

### Retention Hooks
- **Curiosity**: undiscovered elements; unreconnected realms; untried blueprint combinations.
- **Investment**: abilities grown with experience; Onyx gear; a party of characters the player designed.
- **Social**: co-op groups; sci-fi role objectives that need other players; shared builds.
- **Mastery**: harder difficulty tiers; endgame missions; out-thinking MATE.

---

## Game Pillars

Locked after CD-PILLARS review (CONCERNS → revised; see Review Log).

### Pillar 1: Your Class, Your Design
Players author their classes — more than one — and pre-made classes are starting
points, not ceilings. Nothing is barred, but Forge costs escalate, so every class
becomes a commitment.

*Design test*: Debating a pre-built ability or a blueprint the player can build
it from → **the blueprint**. If players lean too hard on one build → **MATE
adapts and counters it; we never nerf the player's invention.**

### Pillar 2: Limitless Power, Earned
Power has no final ceiling (Onyx), but challenge rises to meet it: party size,
chosen difficulty, invested experience and MATE. Failure is readable, so players
learn why a build fell short.

*Design test*: Debating capping the player or raising the challenge → **raise
the challenge, never cap the player.**

### Pillar 3: Two Worlds, One Hero
The same hero reads differently in each world; convergence adds rather than
replaces.

*Design test*: If a feature could move to the other path unchanged → **it belongs
in the shared layer (the Forge) or it is cut.**

### Pillar 4: Feel Follows Build
Pace and range come from the player's build, never from the path they're on.

*Design test*: If a path forces a pace or range → **the path changes, not the build.**

### Pillar 5: Plans Executed Together
Co-op is coordination, not crowds. A party is either human or one you built
yourself — solo players command a party they crafted. Builds show readable roles,
and difficulty scales with party size.

*Design test*: Debating raw group size or interlocking roles → **interlocking
roles**, whether the party is people or your own creations.

**Tensions (intentional).** P2 vs P4 (demanding challenge vs every build viable);
P1 vs P2 (open Forge vs a solved meta — answered by escalating cost and MATE);
P2 vs P5 (squad vs solo tuning — answered by party-size scaling).

### Anti-Pillars (What This Game Is NOT)

- **NOT class-locked abilities**: would compromise *Your Class, Your Design*.
- **NOT new classes as content drops**: we ship blueprints, elements and alterations — players never wait on us for their class (*Your Class, Your Design*).
- **NOT gear score alone beating difficulty**: would compromise *Limitless Power, Earned*.
- **NOT one path a reskin of the other**: would compromise *Two Worlds, One Hero*.
- **NOT core progression gated behind group content**: solo with a crafted party is always a full path (*Plans Executed Together*).
- **NOT a build-calculator game**: the Forge reads in play, not only in spreadsheets (*Your Class, Your Design*, *Limitless Power, Earned*).
- **NOT selling power, blueprints or elements**: protects *Limitless Power, Earned*. Kept as written for now; the founders-pack idea is an open question.

---

## Visual Identity Anchor

**Direction:** A + B + C, as layered by the art director (AD-CONCEPT-VISUAL: STRONG).

**One-line visual rule:** *An ability's shape is its blueprint and its colour is
its element; the world never competes with the player's light.*

| Layer | Source | Role |
| ---- | ---- | ---- |
| Identity | A — "The Blueprint Is the Signature" | How abilities look |
| Environments | B — "Ruler and Brush" | How each world looks |
| Combat feedback | C — "Light Before Blade" | How threats read |

**Supporting principles**

1. **Fixed blueprint geometry.** Bolt = line, ball = sphere, beam = column,
   shield = plane, dash = streak — identical in both worlds; staged abilities
   read as linked forms.
   *Design test*: a new blueprint ships only with a silhouette distinct from
   every existing one.
2. **Ruler and brush.** Sci-fi is drawn with a ruler (chamfered hard surfaces,
   panel lines); fantasy with a brush (eroded, organic, brass steampunk accents);
   the convergence splits along visible seams (circuitry laid as rune lattice) —
   never averaged.
   *Design test*: if an environment asset could sit in the other world unchanged,
   it belongs to the convergence set or it is restyled.
3. **Light before blade.** Every threat is telegraphed by light before it lands.
   *Design test*: an enemy attack with no light-based wind-up does not ship.

**Colour philosophy.** The world stays mid-value and low-saturation; only abilities
get high saturation and emission. Base energy is a neutral white-gold core; each
element owns one hue game-wide; player identity is a trim/rim colour, not the core
hue. Innovation renders as crisp hard-light, Manifest as particulate and flowing.
Red and amber are reserved for enemy wind-ups and danger. World palettes: sci-fi
steel and cyan with signal orange; fantasy ochre, moss and candle-amber.

*Flagged by the art director:* element hues, player trims, threat colours and two
world palettes compete for hue space — allocate early and verify against
colourblind modes (ux-designer). VFX density for co-op depends on the hardware
target (technical-artist pass after platform specs).

---

## Inspiration and References

| Reference | What We Take From It | What We Do Differently | Why It Matters |
| ---- | ---- | ---- | ---- |
| Warframe | Sci-fi power fantasy; mod depth; mission variety | The frame itself is designed by the player | Proves a long-haul build-focused audience exists |
| Elden Ring | Build-proving challenge; open exploration; readable bosses | Builds are authored, not assembled from fixed spells | Proves difficulty makes builds feel earned |
| Path of Exile | Build authorship; theorycrafting community | Authorship at the ability level, readable in play | Proves players will invest deeply in build systems |
| The First Descendant / Destiny | Distinct striking characters; group content | No developer-released classes | The audience the user comes from |
| Mass Effect / Dragon Age | Commanding a party | The party is made of the player's own designs | Validates the crafted-party fantasy |
| Black Desert | Weighty, flashy action combat | Combat feel set by the build | Feel benchmark for the fantasy path |
| Ghost Recon (co-op) | Coordinated execution | Role-split objectives inside one mission | The formative moment behind Pillar 5 |
| Hades / roguelikes | Procedural runs as a test bed | Runs test Forge builds rather than run-only upgrades | Shapes the test-build phase |

**Non-game inspirations**: None stated yet — to gather during `/art-bible`.

---

## Target Player Profile

| Attribute | Detail |
| ---- | ---- |
| **Age range** | 18–40 |
| **Gaming experience** | Mid-core to hardcore |
| **Time availability** | Regular 1–2 hour sessions; long-haul players who stay for years |
| **Platform preference** | PC |
| **Current games they play** | Warframe, Destiny 2, The First Descendant, Elden Ring, Path of Exile |
| **What they're looking for** | A class that is truly theirs, that keeps evolving, tested by real challenge |
| **What would turn them away** | Pay-to-win; a solved meta; class-locked abilities; grind with no build expression; unreadable difficulty |

---

## Technical Considerations

| Consideration | Assessment |
| ---- | ---- |
| **Recommended Engine** | Unreal Engine 5 — user preference, recorded without second-guessing. Not yet pinned: `/setup-engine` pins the version. The technical director notes UE's built-in stack (Gameplay Ability System, Niagara, World Partition, replication) fits the core risks; the trade-off is a C++ learning curve for a first-time developer. |
| **Key Technical Challenges** | Data-driven ability composition and chaining; composable VFX per blueprint × element × alteration; networked co-op with player-authored abilities; companion AI for unknown builds (intent tags); MATE; procedural generation; large-number progression (Onyx, exponential costs) |
| **Art Style** | 3D, muted world with high-saturation abilities (see Visual Identity Anchor) |
| **Art Pipeline Complexity** | Medium–High — marketplace/Fab and Megascans environments; custom, systemic ability VFX |
| **Audio Needs** | Moderate — systemic ability audio per blueprint and element; music per world |
| **Networking** | None in MVP and test builds (data designed serializable from day one). Co-op: player-hosted client-server (listen server); dedicated-server option open. 1.0: player-hosted shared world, not an MMO |
| **Content Volume** | Test builds: one hub, procedural runs, pre-made classes and roles for both gameplay types. Later: realms, missions and enemies added per update — not yet quantified |
| **Procedural Systems** | Roguelike run generation in the test-build phase; scope set in its GDD |

TD-FEASIBILITY: **HIGH RISK** for the full vision as originally described, roughly
CONCERNS for the MVP alone. Since then the user phased the structure (roguelike
test builds before paths), capped parties (4 at launch → 5/10), made MATE curated
first, and defined 1.0 as player-hosted rather than an MMO — which addresses the
four blockers raised. Engine-version-specific claims were not sourceable (no pin)
and must be rechecked after `/setup-engine`.

---

## Risks and Open Questions

### Design Risks
- An open Forge may still converge on a solved meta — mitigated by escalating cost and MATE, unproven.
- Scaling difficulty against invested experience can cancel the feeling of growth (the level-scaling problem) — players must out-grow it on average.
- MATE adaptations may feel like cheating unless signposted.
- Experience is both levelling and Forge currency — a tension that must be deliberate.
- Pre-made classes and roles for both gameplay types must still feel like Forge creations (Pillar 1).

### Technical Risks
- Runtime ability composition and chaining (HIGH) — prove it in the MVP; whether the Gameplay Ability System natively supports staged abilities is not yet sourced.
- Composable VFX (HIGH) — parameterised Niagara systems, not per-combination assets.
- Performance with many player-made abilities (MEDIUM, HIGH with co-op) — budgets set once the engine and hardware target are pinned.
- Number integrity (MEDIUM) — 64-bit or big-number storage for experience and costs; keep progression currencies outside GAS float attributes; versioned saves.
- Networking retrofit — the single-player-to-co-op transition is the highest-risk tier change.
- Companion AI with unknown builds — ability intent tags.

### Market Risks
- A crowded genre led by live-service giants.
- Long time to first public release for a solo developer.
- Monetization reputation risk with this audience (pay-to-skip sensitivity).

### Scope Risks
- PR-SCOPE: **OPTIMISTIC**. First public release estimated 3–4.5 years part-time; low confidence until velocity is measured after milestone 1a.
- Solo developer, new to code: a learning tier is explicit in the plan.
- Content volume per update (realms, missions, enemies).
- The user is committed regardless of duration; scope is recorded, not used to reject.

### Open Questions
- **Founders pack**: unlocking every current and future element conflicts with the seventh anti-pillar. Options logged: a head start that stays earnable free; cosmetic element variants; keep as stated. Decide before any store page.
- **Mixed parties**: can a co-op friend's characters and your crafted members fight together? Does a crafted party count toward difficulty like human players?
- **Experience across the party**: shared or per character? This sets how practical a full crafted party is under escalating Forge cost.
- **Element discovery and blueprint discovery**: elements are discovered; blueprints are all unlocked for now (blueprint discovery is a "big maybe").
- **Co-op topology**: listen server vs dedicated — answered by the networking spike after the MVP.
- **Hardware and frame-rate target** — needed for performance budgets.
- **Weekly hours and budget** — every estimate scales with them.

---

## MVP Definition

**MVP = Minimum Viable Product**: the smallest playable build that answers one
question.

**Core hypothesis**: *Designing an ability in the Forge and then proving it against
a demanding fight is fun — and two builds made from the same parts feel clearly
different.*

**Required for MVP** (one roguelike arena, single-player), as playable milestones:
1. **1a** — one bolt ability running entirely from data; Forge cost formula with unit tests, including large values.
2. **1b** — alterations and one chained/staged ability; every combination runs from data with no per-combination code or assets.
3. **1c** — energy plus one element; layered VFX (changing element or alteration changes visuals with no new asset); two builds that feel different (fast/mobile vs weighty/committed).
4. **1d** — one demanding boss with readable tells; a worst-case stress scene; a lossless save/load round-trip of a composed ability.

Blueprints in scope: bolt, ball, beam, shield; 2–3 alterations.

**Explicitly NOT in MVP** (defer to later):
- Both gameplay types' full content; paths; convergence.
- Co-op, crafted parties, companion AI.
- MATE.
- Mods, runes, enchants, gear rarity.
- Procedural generation beyond a single arena.

### Scope Tiers (if budget/time shrinks)

Estimates from PR-SCOPE: solo, part-time (~15–20 h/week), new to code, marketplace
art. Full-time roughly halves each range. Low confidence until velocity is measured.

| Tier | Content | Features | Timeline |
| ---- | ---- | ---- | ---- |
| **0 — Learning** | 2–3 throwaway learning projects | UE5 editor, Blueprints, C++ basics, Gameplay Ability System, Niagara | 3–6 months |
| **1 — MVP** | One roguelike arena, one boss | Forge slice (milestones 1a–1d); ends with a 2–4 week networking spike | 9–18 months (~12) |
| **2 — Test builds** (first public release; stands alone if work stops here) | One central hub; procedural roguelike runs; both gameplay types tested side by side | Pre-made fantasy classes and sci-fi roles; limited custom class; curated MATE | +18–36 months (~24) |
| **3 — Paths split** (ongoing updates) | Fantasy realms (reconnect each by defeating its shard-holder); sci-fi missions; endgame missions | Co-op at 4 → 5 / 10; crafted parties + companion AI; mods, runes, rarity to Onyx | +36–72+ months |
| **4 — 1.0 Convergence** | All realms and the sci-fi universe in one shared world | Player-hosted shared open world (not an MMO); both upgrade layers stack | Not yet estimated |
| **Beyond 1.0** | — | Generative MATE | Research — not estimated |

**Momentum rules (applied from PR-SCOPE):** a playable build every quarter; a
public devlog; a hard timebox on Forge numbers and balance during Tier 1;
recalibrate all estimates after milestone 1a.

---

## Review Log

| Gate | Verdict | Outcome |
| ---- | ---- | ---- |
| CD-PILLARS | CONCERNS | Pillars revised: cost clause (escalating Forge cost), P2 reframed as *Limitless Power, Earned*, P3 escape clause removed, P4 → *Feel Follows Build*, crafted parties in P1/P5, two anti-pillars added. Locked by the user. |
| AD-CONCEPT-VISUAL | STRONG | User chose A + B + C layered. |
| TD-FEASIBILITY | HIGH RISK (full vision); ~CONCERNS (MVP) | Blockers addressed by phasing, party caps, curated MATE, player-hosted 1.0. Engine-version claims NOT SOURCEABLE until `/setup-engine`. |
| PR-SCOPE | OPTIMISTIC | Applied: momentum package; co-op launches at 4. Declined: one path first in test builds (test builds have no paths). 1.0 confirmed as player-hosted, not an MMO. |

---

## Next Steps

Prototype-first: the core mechanic (the Ability Forge) is unproven.

- [ ] `/setup-engine` — pin Unreal Engine 5 and its version-aware reference docs
- [ ] `/prototype ability-forge` — throwaway test of the Forge-and-fight loop
- [ ] If PROCEED: `/art-bible` (seeded from the Visual Identity Anchor)
- [ ] `/design-review design/gdd/game-concept.md`
- [ ] `/map-systems` → `/design-system` per system (with `/design-review` each) → `/review-all-gdds`
- [ ] `/create-architecture` → `/architecture-decision` per decision → `/architecture-review`
- [ ] `/vertical-slice` before committing to sprints
- [ ] `/gate-check` at each stage
