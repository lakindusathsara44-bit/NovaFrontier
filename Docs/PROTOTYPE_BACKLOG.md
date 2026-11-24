# NOVA: FRONTIER — Prototype Backlog

**Milestone:** Prototype A — First-person movement and firing  
**Status:** Ready for implementation planning; engine version still gated  
**Priority rule:** Complete and test each task before expanding scope.

## Gate 0 — Workstation and engine feasibility

- [ ] Confirm access to a workstation capable of running the selected Unreal Editor version.
- [ ] Record OS, CPU, RAM, GPU, driver, available disk space.
- [ ] Select an Unreal Engine version only after checking its official requirements and Android toolchain.
- [ ] Create a clean First Person C++ project.
- [ ] Open the project successfully.
- [ ] Compile a minimal C++ class.
- [ ] Run the template map.
- [ ] Package a Windows Development build.
- [ ] Record logs, exact versions, and any blockers.

**Exit criteria:** a clean project opens, compiles, runs, and packages on the selected workstation. Do not start production gameplay code before this gate passes.

## Gate 1 — Player foundation

- [ ] Create the project source-control baseline.
- [ ] Configure Enhanced Input.
- [ ] Implement first-person camera and look.
- [ ] Implement walking, sprinting, and jumping.
- [ ] Add input sensitivity settings.
- [ ] Create a greybox test room.
- [ ] Verify collision and movement boundaries.

**Exit criteria:** player movement is repeatable and responsive in a packaged Windows Development build.

## Gate 2 — First weapon

- [ ] Create weapon data/configuration structure.
- [ ] Implement KX-9 prototype hitscan trace.
- [ ] Implement fire cadence and ammunition.
- [ ] Implement reload state and interruption handling.
- [ ] Add basic muzzle and impact feedback.
- [ ] Add target health and damage response.
- [ ] Test empty magazine, full reload, rapid fire, and target destruction.

**Exit criteria:** a player can reliably fire, hit, reload, and disable a target.

## Gate 3 — Player survivability and enemy

- [ ] Add player health and damage reception.
- [ ] Add death and restart behaviour.
- [ ] Implement one Frontier Security Drone.
- [ ] Add idle/patrol, detect, alert, attack, search, and disabled states.
- [ ] Add line-of-sight and cover checks.
- [ ] Add readable alert and attack cues.
- [ ] Test repeated encounters and edge cases.

**Exit criteria:** one fair, repeatable combat encounter works without progression blockers.

## Gate 4 — Mission loop

- [ ] Implement objective state model.
- [ ] Implement interaction interface.
- [ ] Implement checkpoint and restart.
- [ ] Implement mission completion state.
- [ ] Add subtitles and placeholder protagonist dialogue.
- [ ] Add basic HUD and pause menu.
- [ ] Greybox Mission 01.

**Exit criteria:** a new player can complete the short mission without developer guidance.

## Gate 5 — Android feasibility

- [ ] Configure the supported Android toolchain for the chosen engine.
- [ ] Build and install on a physical Android device.
- [ ] Implement touch movement and camera look.
- [ ] Add configurable touch buttons and safe-area layout.
- [ ] Measure frame pacing, memory, temperature, and battery impact.
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
- Device hardware and OS.
- Build configuration.
- Steps performed.
- Expected result.
- Actual result.
- Logs/video/screenshots.
- Known issues and next action.

A checkbox must not be marked complete without evidence.
