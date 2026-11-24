# NOVA: FRONTIER — Initial Technical Design

**Status:** Proposed technical baseline v0.1  
**Engine:** Unreal Engine 5 candidate; exact minor version not locked  
**Implementation:** C++ for stable gameplay foundations; Blueprint for iteration and level scripting  
**Important:** Nothing in this document claims that code has compiled or that a packaged build exists.

## 1. Technical principles

- Keep gameplay systems small, testable, and independently understandable.
- Prefer data-driven weapon/enemy tuning over duplicated hard-coded logic.
- Separate platform input from gameplay actions.
- Keep single-player authoritative locally; design clear interfaces for a possible future server-authoritative multiplayer implementation.
- Avoid introducing online services, persistence backends, or complex plugins before a proven need.
- Commit source and project configuration; do not commit generated build folders or large derived caches.

## 2. Proposed module boundaries

| Area | Responsibility |
|---|---|
| Core | Game instance, game mode, shared interfaces, tags, configuration |
| Player | Character movement, camera, health, interaction |
| Combat | Weapon base, weapon data, firing, reload, damage |
| AI | Perception, state transitions, enemy attack and disable |
| Campaign | Mission state, objectives, checkpoints, save/load |
| UI | HUD, menus, settings, prompts, accessibility |
| Audio | Sound cues, mix, feedback routing |
| Platform | Input mapping, quality profiles, platform-specific settings |
| Online (later) | Sessions, replication, server validation; excluded from first prototype |

These are logical boundaries. Do not create empty modules or excessive abstractions before there is real code requiring them.

## 3. Input architecture

Use Unreal Enhanced Input for the production baseline:
- Input actions represent Move, Look, Jump, Sprint, Fire, Aim, Reload, Interact, Pause.
- Input mapping contexts are selected by platform and game state.
- Desktop and mobile mappings trigger the same gameplay actions.
- UI focus and gameplay input must not conflict.
- Mobile touch layout must respect device safe areas.

Do not build a custom input framework unless Enhanced Input proves insufficient.

## 4. Combat architecture

Suggested responsibilities:
- Player character requests an attack from the equipped weapon.
- Weapon owns fire cadence, ammunition, reload state, and firing presentation.
- Damage is applied through a shared damage interface/event path.
- Weapon tuning is stored in a data asset or equivalent configuration.
- The first weapon uses a line trace/hitscan implementation.
- Visual effects and audio are presentation layers; they do not determine whether a hit occurred.

Required edge-case tests:
- Firing with no ammunition.
- Reload while full.
- Reload interrupted by death or weapon change.
- Rapid input beyond fire cadence.
- Target destroyed during repeated hits.
- Pause/menu opened during reload or firing.

## 5. Enemy AI architecture

Prototype states:
- Idle/Patrol.
- Detect.
- Alert.
- Attack.
- Search.
- Disabled.

Use a small state machine or Unreal AI framework appropriate to the actual implementation. The first enemy does not need a complex behaviour tree. Ensure line-of-sight and cover checks are explicit and testable.

## 6. Mission flow

Represent objectives as explicit states:
- Locked.
- Available.
- Active.
- Completed.
- Failed (only where failure is meaningful).

Only one primary objective should be active in the first mission. Objective completion must be triggered by gameplay events, not fragile text/UI state. Checkpoints should store only the state needed to restore a fair restart.

## 7. Save and checkpoint strategy

For the first slice:
- Save at deliberate checkpoints and mission completion.
- Store versioned, minimal progress data.
- Handle missing, invalid, or older save data safely.
- Do not store platform credentials or sensitive personal data.
- Test restart after death, app close, and level reload.

Cloud saves are not required for the prototype.

## 8. Performance and content pipeline

- Build a representative test map before producing final art.
- Use profiling tools to identify actual bottlenecks.
- Keep materials, dynamic lights, shadows, particles, and texture sizes scalable.
- Avoid expensive post-processing on mobile.
- Test on physical Android hardware early.
- Record test device, engine version, build configuration, resolution, frame rate, memory, temperature, and observed issues.

## 9. Version-control policy

Track:
- C++ source.
- Config files.
- Project descriptor.
- Input assets/configuration.
- Small source assets and documentation.
- Build scripts and reproducible project setup notes.

Ignore:
- Binaries.
- Intermediate.
- Saved (except intentionally versioned config where justified).
- DerivedDataCache.
- IDE-generated folders.
- Local user settings.
- Packaged build output.

Use Git LFS for large binary source assets if/when needed. Check licences before adding external assets.

## 10. Engine-version feasibility gate

Do not assume the starter template or old sample code is compatible with the latest engine version.

Before locking the engine version:
1. Install or access a capable workstation.
2. Create a clean first-person C++ project in the selected Unreal version.
3. Confirm project opens without missing modules.
4. Compile a minimal C++ class.
5. Configure Enhanced Input.
6. Run a greybox map.
7. Package a Windows development build.
8. If Android is a target for the selected milestone, configure SDK/NDK and package to a physical device.
9. Record exact engine, compiler, SDK, and device versions.

The Dell Inspiron 3595 (AMD A9-9425, integrated Radeon R5, 8 GB RAM) should be treated as a documentation, lightweight coding, and source-control machine unless a measured test proves otherwise. Full editor work, shader compilation, cooking, and packaging may require a stronger borrowed or cloud workstation.

## 11. Build gates

| Gate | Evidence required |
|---|---|
| Project opens | Editor log and successful project load |
| C++ compiles | Successful Development Editor build |
| Core loop works | Recorded movement/fire/reload test |
| Enemy works | Repeatable AI encounter test |
| Windows package works | Launchable packaged build and test log |
| Android package works | Installable build tested on physical device |
| Performance target met | Profiling capture on defined minimum/recommended devices |

## 12. Multiplayer boundary (future)

Do not add replication speculatively. When approved:
- Define server ownership for health, damage, inventory, and match state.
- Validate fire rate, hit claims, movement, and progression server-side.
- Design reconnect and session lifecycle.
- Define privacy, moderation, abuse reporting, hosting region, operational cost, and telemetry policy.
- Keep single-player campaign progress separate from competitive match state.

## 13. Current implementation order

1. Verify engine and workstation feasibility.
2. Create clean project and source-control baseline.
3. Implement player movement and camera.
4. Implement one hitscan weapon and reload.
5. Implement health/damage and one target.
6. Implement one enemy state machine.
7. Implement objective and checkpoint flow.
8. Build Mission 01 greybox.
9. Profile and test Windows.
10. Validate Android controls and performance on a physical device.

**No production code should be considered complete until it has been built and tested in the selected engine version.**
