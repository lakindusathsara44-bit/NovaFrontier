# NOVA: FRONTIER — Master Development Plan

> Working title. An original science-fiction first-person shooter for Windows and Android.
> This document defines the complete development path from concept to release. It is a plan, not a claim that the listed features have already been implemented.

## 1. Product vision

Build an original, story-driven sci-fi FPS with the visual ambition of classic premium mobile shooters such as NOVA Legacy, while creating original characters, environments, weapons, story, sound, UI, and code.

### Target platforms
- Windows PC
- Android phones and tablets

### Game modes
- Single-player story campaign
- Online multiplayer (introduced after the core combat loop is stable)

### Quality target
- Premium sci-fi presentation, strong art direction, readable combat, responsive controls, polished animation and sound.
- Visual quality must scale by device: high/ultra tiers for capable Windows PCs; carefully optimized scalable tiers for Android.
- Do not promise identical graphics or frame rates across mobile and PC. NOVA-class visual ambition is a production target, not a guarantee for every device.

## 2. Technical direction

### Engine
- Primary candidate: Unreal Engine 5.8, subject to a minimal compatibility/build test before committing the project to a specific engine minor version.
- C++ for gameplay foundations and performance-sensitive systems.
- Blueprints for designer-facing configuration, UI flow, mission scripting and rapid iteration.
- Enhanced Input for keyboard/mouse, controller and touch input.
- UMG for menus, HUD and mobile touch interface.

### Art and content tools
- Blender: original meshes, props, collision proxies and simple animation.
- Substance 3D Painter or equivalent: materials and texture authoring if available.
- Mixamo or properly licensed animation sources for prototype motion; replace or customize where needed.
- Original or properly licensed audio, VFX, fonts and assets only.

### Online direction
- Begin with offline single-player.
- Prototype listen-server sessions only after core gameplay works.
- For public competitive multiplayer, use server-authoritative gameplay, server validation of damage/inventory/score, session discovery, reconnect handling, moderation and basic anti-cheat measures.
- Choose hosting/backend only after player-count, budget, region and operational requirements are known. Do not assume free hosting can support a production multiplayer game.

### Repository and engineering rules
- Keep source, configuration, documentation and small project assets in Git.
- Use Git LFS for large binary art/audio assets when appropriate; do not commit generated Unreal folders such as Binaries, Intermediate, Saved or DerivedDataCache.
- Work on feature branches and use pull requests for reviewed changes once the repository has its initial baseline.
- Keep a changelog, issue tracker, milestone checklist and tested build notes.
- Pin the exact Unreal Engine version after the first successful build; document plugins and SDK versions.

## 3. Proposed game concept (initial, changeable)

**Premise:** Humanity establishes a frontier research colony beyond known space. A catastrophic signal event disables the colony network and awakens an unknown hostile presence. The player is a combat specialist sent to investigate, rescue survivors, uncover the origin of the signal and prevent it from reaching inhabited space.

**Design pillars**
1. Responsive first-person combat.
2. Strong sci-fi atmosphere and readable environments.
3. A clear campaign with memorable missions and escalating threats.
4. Distinct weapons, enemy roles and encounter design.
5. Accessible controls across mouse/keyboard and touch.
6. Fair, server-authoritative multiplayer as a later mode.

This is a working premise, not final canon. Character names, factions, mission count and ending will be finalized during pre-production.

## 4. Full development phases

### Phase 0 — Feasibility, scope and project governance
**Deliverables**
- Confirm target devices and minimum Android specification.
- Confirm available development hardware and whether a cloud workstation/borrowed PC is possible.
- Select and verify the Unreal Engine version with a tiny C++ test project.
- Establish repository conventions, issue labels, branching and backup strategy.
- Define a realistic first playable target and a strict feature cut line.

**Exit criteria:** The project can be opened and a trivial C++ build succeeds on the chosen development machine.

### Phase 1 — Pre-production and game design
**Deliverables**
- Game Design Document (GDD).
- Story bible: setting, factions, characters, conflict and ending.
- Campaign outline and mission briefs.
- Core gameplay loop and combat design.
- Weapon and enemy roster.
- Control schemes for desktop and touch.
- UI/UX wireframes.
- Art direction, reference board and visual target tiers.
- Audio direction and accessibility requirements.
- Performance budgets for Windows and Android.

**Exit criteria:** Scope, story, visual direction and first vertical-slice mission are approved.

### Phase 2 — Technical foundation
**Deliverables**
- Unreal C++ project and module structure.
- GameInstance, GameMode, GameState and PlayerState foundations.
- Enhanced Input mapping contexts.
- First-person character, camera, movement, sprint, crouch and jump.
- Interaction framework.
- Save/settings framework.
- Logging, crash reporting approach and configuration profiles.
- Automated build/check workflow where feasible.

**Exit criteria:** A greybox map loads and the player can move, look, jump and interact reliably.

### Phase 3 — Combat vertical slice
**Deliverables**
- Weapon base class and data-driven weapon definitions.
- Hitscan and/or projectile firing.
- Ammo, reload, fire modes and weapon switching.
- Recoil, spread, hit feedback and damage interfaces.
- Health, armor and death/respawn logic.
- One complete prototype weapon and one target dummy.
- Basic combat HUD and sound placeholders.

**Exit criteria:** A repeatable combat encounter is playable from start to finish.

### Phase 4 — Enemy AI and encounter systems
**Deliverables**
- AI Controller and navigation.
- Perception, patrol, alert, investigate, attack, take-cover and retreat behaviours.
- Enemy archetypes with distinct combat roles.
- Damage reactions and death behaviour.
- Encounter director and difficulty tuning.
- One prototype boss encounter.

**Exit criteria:** A small arena supports a complete, balanced encounter with multiple enemy roles.

### Phase 5 — Art, animation and world pipeline
**Deliverables**
- Modular environment kit and material library.
- Player arms, weapon models and animation set.
- Enemy models, rigs and animation set.
- Lighting and VFX style guide.
- Sci-fi props, decals, particles and environmental storytelling.
- Audio effects, ambience and music pipeline.
- Level streaming/world partition strategy appropriate to the chosen level size.
- LODs, collision, texture budgets and asset naming rules.

**Exit criteria:** One representative environment meets the agreed visual target while staying within performance budgets.

### Phase 6 — Vertical slice mission
**Deliverables**
- One polished mission with intro, traversal, combat, objective, enemy encounter, checkpoint and ending.
- Final-quality target for one environment, one enemy family and a small weapon set.
- Basic cinematics/dialogue and mission UI.
- Save/load and restart flow.
- Windows and Android performance experiments.

**Exit criteria:** An external tester can play the mission without developer assistance; critical bugs are resolved.

### Phase 7 — Full single-player campaign
**Deliverables**
- Complete campaign mission list and progression.
- Mission objectives, scripting, checkpoints and boss encounters.
- Story scenes, dialogue, subtitles and localization-ready text.
- Weapon and player progression.
- Difficulty settings and accessibility options.
- Save slots, settings persistence and campaign completion.
- Credits and end-game flow.

**Exit criteria:** The entire campaign is playable from new game to ending.

### Phase 8 — Multiplayer design and implementation
**Deliverables**
- Finalize modes, player count, match length and progression rules.
- Network replication for movement, weapons and match state.
- Server-authoritative damage, inventory, scoring and match results.
- Lobby, session discovery, matchmaking approach and party flow.
- Dedicated server build and hosting plan if required.
- Disconnect/reconnect, latency handling and abuse reporting.
- Basic anti-cheat strategy and server-side validation.
- Network load tests and privacy/security review.

**Exit criteria:** A controlled external test with multiple real clients completes matches without critical synchronization or exploit issues.

### Phase 9 — Cross-platform optimization and accessibility
**Deliverables**
- Windows graphics presets and input settings.
- Android scalable rendering profiles and touch UI.
- Device compatibility matrix and minimum supported Android device.
- Frame-time, memory, storage and thermal profiling.
- LODs, texture streaming, shader/material simplification and VFX budgets.
- UI scaling, subtitles, remappable controls and sensitivity options.
- Network and bandwidth profiling for multiplayer.

**Exit criteria:** Target devices meet the agreed frame-time, memory and stability budgets.

### Phase 10 — QA, security and release preparation
**Deliverables**
- Functional, regression, performance and compatibility test plans.
- Crash and save-data recovery testing.
- Multiplayer abuse, server validation and privacy checks.
- Store assets, screenshots, trailer, description and support contact.
- Windows installer/package and Android AAB/APK.
- Release candidate, known-issues list and rollback/update plan.
- Legal review of asset licenses, trademarks and privacy disclosures.

**Exit criteria:** No release-blocking defects; store/build requirements are satisfied.

### Phase 11 — Launch and post-launch support
**Deliverables**
- Staged release and monitoring.
- Hotfix process and player feedback channel.
- Crash/performance review.
- Balance updates and bug fixes.
- Content update roadmap and operating-cost review.

**Exit criteria:** Stable launch operations and a sustainable update plan.

## 5. Proposed source-code architecture

Initial module layout (exact folders can evolve with the first successful Unreal build):

```text
NovaFrontier/
├── NovaFrontier.uproject
├── Config/
├── Content/
│   ├── Maps/
│   ├── Characters/
│   ├── Weapons/
│   ├── Enemies/
│   ├── Environments/
│   ├── UI/
│   ├── Audio/
│   └── VFX/
├── Source/
│   └── NovaFrontier/
│       ├── Core/
│       ├── Player/
│       ├── Combat/
│       ├── AI/
│       ├── Campaign/
│       ├── Online/
│       ├── UI/
│       └── Platform/
├── Docs/
├── Tests/
└── README.md
```

### Responsibility boundaries
- Core: game-wide lifecycle, settings, save services and shared interfaces.
- Player: movement, camera, input and player state.
- Combat: weapons, ammo, damage, health and hit feedback.
- AI: perception, navigation, behaviour and enemy coordination.
- Campaign: mission flow, objectives, checkpoints and story triggers.
- Online: sessions, replication, lobby and server rules.
- UI: HUD, menus, accessibility and player feedback.
- Platform: Android touch controls, device profiles and platform-specific settings.

## 6. Milestones and estimation model

These are planning ranges, not guaranteed delivery dates. They assume one primary developer using AI assistance, learning and iterating while working part-time. Actual duration depends heavily on weekly hours, access to a capable build machine, asset creation, testing, multiplayer hosting and scope changes.

| Milestone | Planning range | Meaning of completion |
|---|---:|---|
| Feasibility + pre-production | 4–8 weeks | Scope and engine test approved |
| Technical foundation | 4–8 weeks | Controllable greybox prototype |
| Combat + enemy AI | 8–14 weeks | Repeatable playable combat demo |
| Art pipeline + vertical slice | 12–20 weeks | One polished representative mission |
| Full single-player campaign | 6–12 additional months | Campaign playable end-to-end |
| Multiplayer beta | 4–9 additional months | Controlled online matches |
| Optimization, QA and release | 3–6 additional months | Release candidate for both platforms |

**Overall planning estimate:** approximately 18–30 months for a tightly scoped, part-time solo project with reliable access to a capable build machine and substantial use of existing/licensed assets. A more ambitious, highly polished game with original AAA-level content or complex live multiplayer can take several years and may require a team and a budget.

A first playable prototype may be achievable in roughly 3–6 months under the same assumptions. No finish date should be treated as committed until weekly available hours, asset strategy and build hardware are confirmed.

## 7. Hardware and production constraints

The current development laptop is reported as Dell Inspiron 3595, AMD A9-9425, 8 GB RAM and integrated Radeon R5 graphics. This is not a suitable primary workstation for full Unreal Engine 5 content production, shader compilation, high-end lighting or final packaging.

Epic's current UE5.8 hardware guidance recommends 32 GB system RAM and 8 GB or more graphics memory for a comfortable development workstation. Therefore:
- Keep documentation, planning and source review lightweight on the Dell.
- Avoid promising full editor performance on this laptop.
- Use a capable Windows PC or cloud workstation for editor builds, asset cooking and packaging.
- Test low-end Android devices early; do not wait until the end.
- Avoid Lumen/Nanite assumptions for the Android baseline; choose mobile-appropriate rendering and content budgets.
- Budget for cloud compute, storage, asset licenses and multiplayer hosting if those become necessary.

## 8. Android and Windows release requirements

### Android
- Confirm minimum Android OS, 64-bit CPU and supported GPU family.
- Install the exact SDK/NDK/JDK combination required by the selected Unreal version.
- Configure package identity, signing, permissions and store target API.
- Use scalable quality settings and compressed textures.
- Test on real devices for frame rate, heat, memory, battery and install size.
- Produce signed AAB for store release and APK for controlled testing as appropriate.

### Windows
- Define minimum OS, CPU, GPU, RAM and storage.
- Test keyboard/mouse and controller support.
- Package a Shipping build and validate installer/update behaviour.
- Test graphics presets, resolution scaling and crash recovery.

## 9. Risks and mitigation

| Risk | Mitigation |
|---|---|
| Laptop cannot run UE5 comfortably | Cloud/borrowed workstation; lightweight local workflow |
| Scope too large for one developer | Build one vertical slice first; cut features by priority |
| Multiplayer complexity | Defer until single-player core is stable |
| Mobile performance | Early real-device profiling and strict asset budgets |
| Asset production takes too long | Use original modular kits and properly licensed assets |
| Build/SDK incompatibility | Pin engine and SDK versions; document reproducible setup |
| Hosting costs | Prototype locally; estimate concurrent users before selecting services |
| Schedule uncertainty | Track actual hours and milestone completion; reforecast monthly |

## 10. Definition of done

The project is not complete merely because code exists. Final completion requires:
- Campaign playable from start to finish.
- Multiplayer modes tested with external clients.
- Windows and Android builds installed and tested on declared target devices.
- Performance and stability targets met.
- No known release-blocking defects.
- All shipped assets have valid licenses and provenance.
- Store/release documentation, privacy disclosures and support process are ready.

## 11. Immediate next actions

1. Confirm the repository baseline and add this plan.
2. Confirm weekly development hours and access to a stronger PC/cloud workstation.
3. Create the GDD and first campaign outline.
4. Run an Unreal C++ feasibility project on the actual build machine.
5. Pin the engine version and create the real project structure.
6. Build the first greybox movement prototype.
7. Track actual effort and revise the schedule after the first two milestones.

---

**Status:** Planning baseline drafted. No engine build, gameplay feature, multiplayer service or release package is considered verified until tested on the target hardware.
