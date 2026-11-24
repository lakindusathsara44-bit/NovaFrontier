# NOVA: FRONTIER — Unreal Engine Adapter Integration Plan

**Status:** Architecture and implementation contract; not yet an Unreal project or compiled adapter  
**Target milestone:** First playable Windows combat prototype  
**Engine candidate:** Unreal Engine 5.8 stable, not locked until a clean-project gate passes  
**Gameplay baseline:** C++17 portable core on `main`; Enhanced Input for desktop input

## 1. Objective

Bridge the tested, engine-independent C++ gameplay rules to Unreal Engine without duplicating their logic or pretending that portable unit tests prove Unreal compatibility.

The first playable milestone is deliberately small:

1. Launch a greybox arena.
2. Move, look, jump and sprint as a first-person player.
3. Fire and reload the KX-9.
4. Damage and disable one target.
5. Fight one Frontier Security Drone that can perceive, navigate, aim and fire.
6. Complete a short objective and restart the encounter.

This milestone is single-player only. Multiplayer, online services, campaign cinematics and final art are explicitly out of scope.

## 2. Preconditions — do not skip

Before adding Unreal-specific production classes:

- Create a clean C++ First Person project in the selected stable UE version.
- Open the project in the Editor and run its template map.
- Compile one minimal project-owned C++ class.
- Confirm Enhanced Input is available and a basic action works.
- Package and launch a Windows Development build.
- Record the exact UE hotfix, compiler, Windows version and build logs.

The current Dell Inspiron 3595 (AMD A9-9425, integrated Radeon R5, 8 GB RAM) is the source-control and lightweight-editing machine. Do not assume it can run the full UE Editor, compile shaders, cook or package comfortably. Since no stronger local workstation is currently available, evaluate a time-limited cloud Windows workstation or borrowed workstation for the Editor/build gates. Do not purchase a long-term service or commit to a paid plan until a short proof-of-access test succeeds.

If no capable workstation is available, continue portable-core development and source review only. Do not mark Unreal integration, playable prototype or packaging complete.

## 3. Proposed source layout

After the clean-project gate passes, use Unreal's generated project/module naming and place engine-facing classes in the game module. The following is a logical layout, not a claim that these files already exist:

```text
Source/NovaFrontier/
  NovaFrontier.Build.cs
  NovaFrontier.h
  NovaFrontier.cpp
  Player/
    NFPlayerCharacter.h/.cpp
    NFPlayerController.h/.cpp
  Combat/
    NFWeaponComponent.h/.cpp
    NFDamageableInterface.h
    NFHealthComponent.h/.cpp
    NFKX9WeaponComponent.h/.cpp
  AI/
    NFDroneCharacter.h/.cpp
    NFDroneAIController.h/.cpp
    NFEnemyPerceptionComponent.h/.cpp
  Mission/
    NFMissionGameMode.h/.cpp
    NFObjectiveComponent.h/.cpp
  UI/
    NFPrototypeHUD.h/.cpp
```

Keep the existing portable files under `Source/Core`. Do not move or rename them during the first adapter pass. The Unreal module should include the core headers and compile the corresponding core `.cpp` files through the project module/build configuration. Confirm the exact UBT source registration and include paths in the selected engine version before implementation.

## 4. Adapter contracts

### 4.1 Player

**Unreal owner:** `ANFPlayerCharacter` (ACharacter) plus the generated PlayerController.

- Configure Enhanced Input actions for Move, Look, Jump, Sprint, Crouch, Fire, Reload and Pause.
- Convert device input to `novafrontier::PlayerInputFrame`.
- Call `FirstPersonController::update` once per input/simulation frame with the actual grounded state.
- Apply normalized movement intent using CharacterMovementComponent; Unreal owns collision, gravity, floor checks and physical jump execution.
- Apply yaw/pitch through controller/camera rotation, respecting the portable core's pitch clamp.
- Do not apply `movement_speed` twice: set the movement component's speed once from the returned state and submit normalized movement direction separately.
- Desktop and future touch controls must map to the same gameplay actions.

### 4.2 Health and damage

**Unreal owner:** `UNFHealthComponent` and a damageable interface.

- Wrap the portable `Health` rules for current/max health, damage, healing and alive state.
- Convert Unreal damage events into bounded integer damage at one documented boundary.
- Broadcast health-changed and death events for UI, enemy AI and mission logic.
- Ensure each damage event is applied exactly once.
- In single-player, the local game owns authority; multiplayer authority design is deferred.

### 4.3 KX-9 weapon

**Unreal owner:** `UNFKX9WeaponComponent`.

- Own a `novafrontier::WeaponCombat` instance.
- Advance cooldown/spread with a validated delta time.
- On Fire input, call `try_fire()`; only perform a trace if `fired` is true.
- Use the camera/controller aim ray for the initial prototype; document that camera trace is not identical to physical muzzle trajectory.
- Apply returned recoil through the controller/camera and spread through a documented deterministic trace-offset policy.
- Apply damage through the damageable interface, not by directly editing target health.
- Start reload only when `begin_reload()` succeeds. Complete it from a reload animation notify or a timed prototype event; do not refill immediately at reload start.
- Cancel reload on death, interruption or weapon teardown.
- Add debug counters for shots, rejected shots, hits, reload starts and reload completions.

### 4.4 Frontier Security Drone

**Unreal owners:** `ANFDroneCharacter`, `ANFDroneAIController` and perception adapter.

- Use Unreal AI Perception (or explicitly documented line/sense components) to supply visible-player and hearing events.
- Build `EnemyObservation` in centimetres and seconds. Noise must be a one-frame event pulse.
- Call `EnemyCombatRuntime::update` once per AI update.
- Feed its state and navigation target into `EnemyTacticalNavigation`.
- Supply cover candidates only after Unreal has checked path reachability, occupancy and threat line-of-sight.
- Supply a stable target ID, muzzle position, aim direction, target position/velocity and visibility to `EnemyAimController`.
- Apply the returned bounded aim direction to the AI controller/weapon aim.
- Fire only when the AI state permits attack AND the aim controller says `can_fire` AND the weapon's own cooldown/ammo gate accepts the shot.
- Keep one authoritative firing decision path. Never let multiple components independently fire the same weapon during one frame.
- Apply actual traces and damage in the Unreal adapter; portable `WeaponShotFeedback` alone is not a world hit.

### 4.5 Mission and HUD

- Use an explicit objective component/state model; do not infer mission completion from HUD text.
- First arena objective: reach the relay console, activate it, disable the drone, then reach the exit.
- Use placeholder subtitles for the voiced protagonist; final voice assets are not required for this prototype.
- HUD initially displays health, magazine/reserve ammo, current objective and a small interaction prompt.
- Pause/restart must cleanly stop input and AI updates.

## 5. Frame/update ownership

Use a single documented update path per actor:

| System | Owner | Update rule |
|---|---|---|
| Player input translation | PlayerController | Build one input frame from Enhanced Input |
| Player movement rules | PlayerCharacter adapter | One core update per frame; Unreal applies movement |
| Weapon cooldown/spread | Weapon component | One tick per frame; one fire request per accepted input |
| Enemy perception memory + state | AI controller/runtime | One runtime update per AI simulation step |
| Tactical movement intent | AI controller | Consume one decision; do not duplicate MoveTo requests every tick if target is unchanged |
| Aim | AI controller/aim component | One aim update per AI step |
| Actual shot/damage | Weapon + damageable interface | Only after all gates pass |

Avoid ticking the same portable object from both an Actor and a Component. Validate delta time and prevent duplicate update calls.

## 6. Coordinate, units and type boundary

- Portable core uses centimetres for world distances/positions and seconds for time.
- Unreal uses centimetres and seconds by default; convert explicitly at the adapter boundary.
- Core `Vector3` and Unreal `FVector` are distinct types. Add small, tested conversion helpers; do not reinterpret memory or alias the types.
- Core `Vector2` is not an Unreal input type; convert from Enhanced Input values explicitly.
- Guard non-finite values before sending Unreal observations into the core.
- Do not expose Unreal types inside `Source/Core`; the portable core must remain buildable by CMake without Unreal headers.

## 7. First arena content

Create one small greybox map, not a full level:

- Player spawn and safe onboarding corridor.
- One open test lane with static targets.
- One cover loop with at least two valid cover candidates and one invalid candidate.
- One drone patrol route.
- One relay interaction point.
- One exit trigger.
- Clearly marked debug spawn/reset locations.

Use simple primitives and placeholder materials. Do not spend time on final art before the combat loop is repeatable.

## 8. Test plan and acceptance criteria

### Automated portable tests
- Continue running the seven CMake/CTest targets on every PR.
- Add tests for any changed portable-core behaviour before changing the engine adapter.

### Unreal Editor tests
- Project opens and compiles from a clean checkout.
- No missing module or stale generated-project dependency.
- Player can move, look, jump, sprint and crouch without speed stacking.
- KX-9 obeys fire cadence, ammo, reload and interruption rules.
- Hits damage a target exactly once per accepted shot.
- Drone transitions between patrol, investigate, chase, attack, search and disabled.
- Drone does not fire when invisible, outside attack permission, still reacting, misaligned, reloading or empty.
- Navigation does not spam requests or oscillate between nearly equal cover points.
- Mission objectives cannot be completed out of order.
- Restart returns the arena to a known state.

### Packaged Windows Development build
- Launch outside the Editor.
- Complete the full short encounter using keyboard and mouse.
- Verify HUD, pause, restart and objective progression.
- Record logs and a short capture.

### Definition of Done
The first playable prototype is complete only when a packaged Windows Development build launches and a tester can complete the arena encounter end-to-end. CMake success alone is not sufficient.

## 9. Android and multiplayer sequencing

After the Windows encounter is stable:

1. Add touch mappings for the same gameplay actions.
2. Configure the engine-matched Android SDK/NDK/JDK.
3. Package and install on a physical Android device.
4. Profile frame time, memory, heat and battery behaviour.
5. Establish minimum/recommended device targets from measured results.

Multiplayer remains deferred until the single-player combat loop and damage model are stable. When started, move damage, ammo, fire cadence, enemy decisions and match state to an authoritative server model; do not simply replicate the current local prototype.

## 10. Evidence and status discipline

For every gate, record exact engine/compiler/SDK versions, machine/device specs, build configuration, test steps, expected/actual results, logs and capture links.

Allowed status labels:
- **Planned:** described but not implemented.
- **Authored:** source/docs committed but not engine-compiled.
- **Compiled:** built successfully in the named engine/toolchain.
- **Tested:** acceptance checks executed with recorded results.
- **Packaged:** installable/launchable build produced.
- **Released:** reviewed and distributed build.

Never label a class playable, compiled or packaged based only on portable CMake tests.
