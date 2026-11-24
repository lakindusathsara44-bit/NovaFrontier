# NOVA: FRONTIER — Game Design Document

**Document status:** Pre-production draft v0.1  
**Project:** NOVA: FRONTIER  
**Genre:** Science-fiction first-person shooter (FPS)  
**Target platforms:** Windows PC and Android  
**Modes:** Single-player campaign; multiplayer (planned, later phase)  
**Primary visual reference:** The visual ambition of *N.O.V.A. Legacy*; no copied characters, story, maps, assets, UI, audio, or trademarks.  
**Engine direction:** Unreal Engine 5, subject to a small hardware/toolchain feasibility spike before locking the production version.

> This is the project's working design baseline. Story details marked **PROPOSED** are original concepts for review, not immutable canon. Scope and performance targets must be validated on real devices.

---

## 1. High concept

A remote human settlement on the edge of explored space goes silent after transmitting a signal that should not exist. The player is sent to investigate, restore critical systems, protect survivors, and discover why the colony's automated defence network has begun treating humans as hostile intruders.

**Player promise:** responsive first-person combat, tense science-fiction exploration, readable enemy encounters, and a story that steadily turns a rescue mission into a fight for survival.

**Design pillars**
1. **Readable, responsive combat:** weapons must feel immediate; hit feedback, recoil, sound, enemy reactions, and mobile controls must communicate clearly.
2. **Atmosphere with purpose:** lighting, environmental storytelling, sound, and level layout build tension without hiding essential gameplay information.
3. **Fair challenge:** enemies telegraph danger, the player has meaningful cover and movement choices, and difficulty comes from decisions rather than unfair surprises.
4. **One coherent cross-platform game:** Windows and Android share the same core gameplay rules; input, UI, effects, and rendering scale to each platform.
5. **A shippable scope:** prove one polished mission before expanding to a full campaign or online multiplayer.

## 2. Product goals and non-goals

### Goals
- Deliver a stable, enjoyable first-person shooter on Windows and a defined range of Android devices.
- Build a short, replayable single-player campaign with clear objectives and a memorable science-fiction identity.
- Establish reusable systems for movement, weapons, damage, enemies, objectives, save/checkpoints, and UI.
- Produce one high-quality vertical-slice mission before committing to full campaign content.
- Design multiplayer-compatible boundaries in the architecture without building networking prematurely.

### Non-goals for the first playable milestone
- Open-world exploration.
- Battle royale, extraction, ranked matchmaking, clans, or live-service economy.
- Large-scale destruction or fully simulated physics environments.
- Photorealistic assets at the expense of frame rate and readability.
- A dedicated server fleet before the offline combat loop is proven.
- Pay-to-win purchases, loot boxes, or monetization systems.

## 3. Target audience and experience

**Audience:** players who enjoy compact science-fiction FPS campaigns, tactical encounters, weapon progression, and atmospheric environments.

**Session targets (initial design assumptions):**
- Desktop: 20–45 minute play sessions.
- Mobile: 5–15 minute natural checkpoints within the same campaign.
- First-time player should understand movement, aiming, firing, reloading, and the current objective without a long tutorial.

**Perspective:** first-person throughout gameplay. Menus and optional briefings may use a non-gameplay presentation.

## 4. Core gameplay loop

1. Receive a mission objective and a short contextual briefing.
2. Explore a bounded combat space and identify threats, routes, supplies, and interactable systems.
3. Engage enemies using movement, cover, weapon choice, and limited resources.
4. Complete an objective such as restoring power, securing a room, or extracting a survivor.
5. Reach a checkpoint or mission transition.
6. Unlock story information and, where appropriate, a weapon or equipment improvement.

The loop must remain enjoyable with only one weapon and one enemy type in the prototype. Progression is not a substitute for good core combat.

## 5. Player controls and movement

### Windows baseline
- WASD: move.
- Mouse: look/aim.
- Left mouse: fire.
- Right mouse: aim down sights (when supported by the weapon).
- R: reload.
- Space: jump.
- Left Shift: sprint.
- E: interact.
- Esc: pause.

All bindings must be remappable. Mouse sensitivity and invert-Y options are required before release.

### Android baseline
- Left virtual stick: movement.
- Right-side drag area: camera look.
- Contextual fire, aim, reload, jump, sprint, and interact buttons.
- Adjustable HUD layout, button size, opacity, and sensitivity.
- Optional aim-assist tuning must be transparent, configurable, and never give mobile players an unfair advantage in cross-platform multiplayer.

**Movement scope:** walk, sprint, jump, and grounded movement first. Crouch, slide, vaulting, and advanced traversal are deferred until the base controller is stable and tested.

## 6. Combat design

### First prototype weapon
**KX-9 Service Rifle (working name; original fictional weapon).**
- Hitscan for the initial prototype.
- Automatic fire.
- Magazine and reserve ammunition.
- Reload animation/state.
- Tunable damage, fire interval, spread, recoil, range, and reload duration.
- Clear muzzle flash, impact effect, firing sound, and hit confirmation.

Values are deliberately not fixed in this document. They must be tuned in playtests rather than selected arbitrarily.

### Weapon design rules
- Every weapon must have a distinct gameplay role.
- Weapon feedback must be readable on a small screen.
- Ammunition and reload states must be visible without covering the target.
- Avoid random spread so extreme that player skill becomes irrelevant.
- Separate weapon data from weapon logic so new weapons can be added without rewriting the firing system.

### Damage and health
- Player and enemy health use a common, testable damage interface.
- Damage sources identify instigator, causer, amount, and damage type.
- Death, hit reactions, and temporary invulnerability (if any) must be explicit states.
- No gore requirement is assumed; visual intensity and age rating need a later decision.

## 7. Enemy design

### Prototype enemy: Frontier Security Drone
A compact automated security unit that has misidentified the player as a threat.

**Initial behaviour states:** Patrol/Idle → Detect → Alert → Attack → Search → Return/Disabled.  
**Prototype scope:** one enemy archetype, one readable ranged attack, simple cover/line-of-sight checks, and a clear damage/death response.

**Fairness rules**
- Detection has a readable range and line-of-sight rule.
- An enemy should communicate when it has detected or is about to attack the player.
- Enemies must not shoot through solid cover.
- Difficulty should scale through encounter composition and behaviour tuning, not inflated health alone.

Additional enemy types (organic hostile, heavy unit, sniper, boss) are future design candidates, not first-slice commitments.

## 8. Mission and level design

- Levels are authored as bounded, hand-designed spaces.
- Each combat encounter has an entry, readable threat positions, at least one meaningful movement/cover choice, and a clear exit or completion condition.
- Objectives are stated as actionable verbs: reach, restore, defend, collect, escort, or extract.
- Critical path navigation must be understandable on a phone screen.
- Optional lore items must never block mission completion.
- Checkpoints should occur before major encounters and after substantial objective progress.

The first vertical slice is **Mission 01: Silent Arrival** (working title). See `STORY_BIBLE.md` and `MISSION_OUTLINE.md`.

## 9. Story delivery

Story is delivered through:
- Short in-engine dialogue and radio messages.
- Environmental details and optional logs.
- Mission briefings and debriefings.
- Character actions and consequences.

Avoid long exposition during active combat. All essential mission instructions must be available as readable text and not depend on audio alone. Subtitles are required.

## 10. UI / UX

**Required gameplay HUD**
- Reticle.
- Health state.
- Current weapon and ammunition.
- Reload feedback.
- Objective text.
- Interaction prompt.
- Damage direction/feedback where useful.

**Required menus**
- Main menu: Continue, New Game, Settings, Credits, Exit (Exit only where platform permits).
- Pause menu.
- Settings: audio, graphics presets, sensitivity, controls, subtitles, accessibility.
- Mission complete / failure screens.

UI must be designed for 16:9 desktop and common mobile aspect ratios, including tall screens and safe areas. Do not bake critical HUD text into textures.

## 11. Audio and visual direction

**Visual identity:** grounded near-future science fiction; industrial colony architecture; restrained high-contrast lighting; functional technology; a consistent material and colour language. The target is a strong, polished visual impression inspired by the era and readability of *N.O.V.A. Legacy*, not a promise of identical rendering fidelity on all devices.

**Android rendering priorities:** stable frame pacing, readable silhouettes, controlled overdraw, limited dynamic lights, scalable shadows, efficient materials, and texture memory budgets.

**Audio priorities:** weapon identity, enemy alert cues, environmental ambience, UI feedback, and dialogue clarity. Essential information must also have a visual cue.

Use only original, properly licensed, or public-domain assets. Maintain an asset-source and licence register.

## 12. Platform and performance strategy

### Windows
- Keyboard/mouse support first.
- Scalable graphics presets.
- Test on the actual minimum and recommended PC targets before publishing requirements.

### Android
- Touch-first interface.
- Scalable rendering and effects.
- Test on physical devices early; emulator-only validation is insufficient.
- Device compatibility, thermal behaviour, memory use, battery impact, and store SDK requirements must be checked before release.

### Performance policy
- Set measurable minimum/recommended device targets after profiling a representative test scene.
- Profile CPU, GPU, memory, shader compilation, load times, and frame pacing.
- Never use a high-end PC screenshot as evidence that the Android build is performant.
- Visual settings must degrade gracefully while preserving gameplay readability.

## 13. Multiplayer (later phase)

Multiplayer is part of the long-term product vision, but it is **not part of the first playable prototype or vertical slice**.

Before online implementation:
- Prove offline movement, weapons, damage, and enemy combat.
- Define game mode, player count, session length, regional latency expectations, moderation, privacy, and hosting budget.
- Use server-authoritative validation for competitive outcomes.
- Never trust the client for damage, inventory, match results, or progression.
- Plan reconnect, disconnect, matchmaking, abuse prevention, and operational monitoring.

The initial multiplayer mode is undecided. A small team-based mode is a candidate, but it must be approved after the single-player slice.

## 14. Accessibility and player options

- Subtitles with speaker labels.
- Independent music, effects, and dialogue volume.
- Adjustable look sensitivity and invert-Y.
- Remappable desktop controls.
- Adjustable mobile HUD.
- Multiple graphics presets.
- Clear interaction and objective text.
- Avoid relying on colour alone to communicate critical states.

## 15. Scope and milestone acceptance

### Prototype A — movement and firing
- Player can move, look, jump, fire, and reload in a greybox test room.
- One target receives damage and can be disabled.
- No crashes during a 15-minute repeated test.
- Windows development build only initially.

### Prototype B — combat encounter
- One security drone can detect, attack, search, and be defeated.
- Player can take damage and restart from a checkpoint.
- Basic HUD and pause menu work.
- Combat is playable with keyboard/mouse.

### Vertical Slice — Mission 01
- One complete short mission with beginning, middle, and end.
- One polished environment kit, one weapon, one enemy archetype, objective flow, audio, UI, checkpoint, and mission completion.
- Windows build is stable; Android feasibility and performance are measured separately.
- External testers can complete the mission without developer guidance.

### Full release
Release scope, campaign length, supported device list, multiplayer mode, and online services remain gated by the vertical-slice review.

## 16. Risks and mitigations

| Risk | Mitigation |
|---|---|
| Laptop cannot run full editor smoothly | Keep documentation/code lightweight; use a capable borrowed or cloud workstation for editor, cooking, and packaging. |
| Scope grows too quickly | Freeze the vertical-slice scope; defer multiplayer and advanced features. |
| Android performance is poor | Profile early on physical devices; use scalable assets and rendering settings. |
| Multiplayer cost/complexity becomes excessive | Do not begin until offline gameplay is proven and a budget/hosting plan exists. |
| Asset licensing is unclear | Record source, licence, attribution, and permitted use for every external asset. |
| Engine/API version changes | Lock engine version only after a clean project and packaging feasibility test. |

## 17. Open decisions

These are intentionally not silently assumed:
1. Final game title and logo.
2. Story canon, protagonist identity, and character names.
3. Campaign length and exact mission count.
4. Final visual target and minimum Android device specification.
5. Multiplayer mode, player count, and hosting approach.
6. Age rating and content intensity.
7. Monetization and distribution model.

## 18. Definition of Done for this document

- Product vision, target platforms, modes, pillars, core loop, combat, enemy, UI, audio/visual direction, performance strategy, and milestone gates are documented.
- Prototype scope is small enough to validate on limited hardware.
- Unapproved creative and technical decisions are clearly marked as open.
- No claim is made that the game has compiled, run, or been packaged.

---

**Next production gate:** approve the working story direction and Mission 01 outline, then run a minimal Unreal Engine project feasibility test before committing to an engine version or building a large asset pipeline.


## Approved creative direction — 2026-11-24

The project owner approved:
- **Tone:** Balanced Sci-Fi Action + Mystery.
- **Protagonist:** Voiced protagonist.

### Practical interpretation
- Action remains the primary moment-to-moment gameplay experience.
- Mystery is revealed through evidence, environmental details, radio exchanges, and mission discoveries.
- Horror may appear as occasional tension, but the game is not designed as a full survival-horror title.
- The protagonist has authored dialogue, a distinct voice, opinions, and character development.
- Dialogue during combat is brief and prioritises actionable information.
- The protagonist's name, background, voice casting, and final personality details remain open until separately approved.
