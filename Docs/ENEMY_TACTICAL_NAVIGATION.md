# Enemy Tactical Navigation and Cover Selection

## Purpose
Adds a portable tactical decision layer between the enemy combat runtime and the Unreal navigation/animation adapter. It converts AI state, the runtime's navigation target, supplied patrol waypoints and engine-evaluated cover candidates into explicit movement intents.

## Behaviour
- Patrol: follows an authored waypoint route. Routes can loop or ping-pong; waypoint arrival is radius-based.
- Chase, Investigate and Search: move toward the navigation target produced by the perception/combat runtime.
- Attack: holds position by default and requests facing toward the supplied threat position.
- Cover: when under fire or below the configured health fraction, selects a reachable, unoccupied cover point that the engine has confirmed blocks threat line of sight.
- Cover scoring balances travel distance against cover quality. A switch margin provides hysteresis so nearly equal cover points do not cause frame-to-frame jitter.
- If no valid cover exists, the AI falls back to its normal state movement intent.

## Default tuning
- Arrival radius: 100 cm
- Maximum cover travel distance: 1,600 cm
- Low-health threshold: 35% of maximum health
- Cover quality: 0.0 (poor) to 1.0 (excellent)
- Cover switch margin: 150 score units

## Engine adapter contract
1. Call `EnemyCombatRuntime::update` to obtain the AI state and optional navigation target.
2. Populate `EnemyTacticalInput` with the enemy's world position, threat position, health, under-fire state, patrol waypoints and cover candidates.
3. The Unreal adapter must compute `reachable` using its navigation/pathfinding layer and `blocks_threat_line_of_sight` using an authoritative trace from candidate cover to threat.
4. Consume `EnemyTacticalDecision`: move toward `target` only when `should_move` is true; use `movement_mode` and `cover_id` for animation/debugging; face the threat when requested.
5. Keep cover candidate IDs stable across frames. Recompute candidate validity when the world changes.

## Safety and authority
This module does not perform pathfinding, collision, cover traces, reservation/network ownership, locomotion, root motion, or animation. It trusts the engine-supplied reachability and line-of-sight flags. In multiplayer, compute decisions on the authoritative server and replicate validated movement/cover state.

## Validation
The C++ tests cover loop and ping-pong patrol routes, target movement and arrival, cover filtering/scoring, cover hysteresis, fallback when no cover is available, disabled state and invalid input rejection.


## Aim integration
The separate `ENEMY_AIM_CONTROLLER.md` module provides reaction timing, aim convergence, optional projectile lead and a firing-readiness gate. The engine adapter should combine its `can_fire` result with the combat state's attack permission before requesting a weapon shot.
