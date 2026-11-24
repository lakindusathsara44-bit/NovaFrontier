# NOVA: FRONTIER — Prototype Backlog

**Milestone:** Prototype A — First playable combat encounter  
**Status:** Portable C++ core is in main; Unreal integration is planned but not yet engine-validated  
**Priority rule:** Complete and test each task before expanding scope.

## Gate 0 — Workstation and engine feasibility

- [ ] Identify a viable Windows workstation path (borrowed or time-limited cloud workstation).
- [ ] Record OS, CPU, RAM, GPU, driver, available disk space and hourly/monthly cost before committing to a service.
- [ ] Re-check official Unreal Engine and Android requirements.
- [ ] Select an Unreal Engine version only after a clean-project validation.
- [ ] Create a clean First Person C++ project.
- [ ] Open the project successfully.
- [ ] Compile a minimal C++ class.
- [ ] Run the template map.
- [ ] Package a Windows Development build.
- [ ] Record logs, exact versions, and any blockers.

**Current constraint:** The Dell Inspiron 3595 (AMD A9-9425, integrated Radeon R5, 8 GB RAM) is the available local machine. Use it for source control, documentation and lightweight code review; do not assume it can run the full UE Editor, shader compilation or packaging comfortably. Do not ask the user to acquire a second local PC as a prerequisite; evaluate borrowed/cloud access first.

**Exit criteria:** a clean project opens, compiles, runs and packages on the selected workstation. Do not start production Unreal gameplay code before this gate passes.

## Gate 1 — Unreal adapter and player foundation

- [ ] Create the clean Unreal project and source-control baseline.
- [ ] Configure Enhanced Input.
- [ ] Add the Unreal player adapter for `FirstPersonController`.
- [ ] Implement first-person camera and look.
- [ ] Implement walking, sprinting, crouching and jumping.
- [ ] Add input sensitivity settings.
- [ ] Create a greybox test room.
- [ ] Verify collision and movement boundaries.
- [ ] Confirm portable-core units and Unreal units are converted explicitly.

**Exit criteria:** player movement is repeatable and responsive in a packaged Windows Development build.

## Gate 2 — First weapon and damage

- [ ] Add health/damage adapter and damageable interface.
- [ ] Add KX-9 Unreal weapon component around `WeaponCombat`.
- [ ] Implement actual camera/weapon hitscan trace.
- [ ] Implement fire cadence and ammunition.
- [ ] Implement reload state and interruption handling.
- [ ] Add basic muzzle and impact feedback.
- [ ] Add target health and damage response.
- [ ] Test empty magazine, full reload, rapid fire, target destruction and duplicate-hit prevention.

**Exit criteria:** a player can reliably fire, hit, reload and disable a target in a packaged build.

## Gate 3 — First enemy encounter

- [ ] Add one Frontier Security Drone actor and AI controller.
- [ ] Connect Unreal perception to `EnemyCombatRuntime`.
- [ ] Connect navigation and cover evaluation to `EnemyTacticalNavigation`.
- [ ] Connect target tracking and firing readiness to `EnemyAimController`.
- [ ] Ensure firing requires attack permission, aim readiness and weapon readiness.
- [ ] Add readable alert and attack cues.
- [ ] Test repeated encounters, cover fallback, reload and disabled state.

**Exit criteria:** one fair, repeatable combat encounter works without progression blockers.

## Gate 4 — Mission loop and first playable

- [ ] Implement objective state model and interaction interface.
- [ ] Implement checkpoint and restart.
- [ ] Implement mission completion state.
- [ ] Add placeholder protagonist dialogue/subtitles.
- [ ] Add basic HUD and pause menu.
- [ ] Greybox the short Mission 01 encounter.
- [ ] Package a Windows Development build.
- [ ] Complete the arena end-to-end outside the Editor.

**Exit criteria:** a new player can complete the short encounter without developer guidance. CMake tests alone do not satisfy this gate.

## Gate 5 — Android feasibility

- [ ] Configure the supported Android toolchain for the locked engine version.
- [ ] Build and install on a physical Android device.
- [ ] Implement touch movement and camera look using the same gameplay actions.
- [ ] Add configurable touch buttons and safe-area layout.
- [ ] Measure frame pacing, memory, temperature and battery impact.
- [ ] Establish minimum and recommended device targets from evidence.

**Exit criteria:** a documented physical-device test passes. Emulator success alone is insufficient.

## Deferred

- Multiplayer and dedicated servers.
- Additional weapons and enemy classes.
- Final cinematics and voice acting.
- Full campaign content.
- Monetisation and live-service features.
- Large-scale open-world environments.

## Evidence log template

For each completed gate, record:
- Date and tester.
- Engine/compiler/SDK versions.
- Workstation/device hardware and OS.
- Build configuration.
- Steps performed.
- Expected result.
- Actual result.
- Logs/video/screenshots.
- Known issues and next action.

A checkbox must not be marked complete without evidence. Use precise status labels: Planned, Authored, Compiled, Tested, Packaged, Released.
