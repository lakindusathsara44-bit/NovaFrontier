# Enemy Aim Controller

## Purpose
Adds a portable aim and firing-readiness layer for enemy combat. It converts engine-supplied target observations into a bounded aim-direction update, optional projectile lead point, reaction-time gate and deterministic accuracy factor.

## Behaviour
- Tracks a stable target ID and restarts reaction time when the visible target changes or disappears.
- Turns toward the predicted aim direction at a configurable maximum angular speed.
- Supports hitscan weapons (projectile speed = 0) and simple constant-velocity projectile leading with a maximum lead-time clamp.
- Requires both reaction completion and angular alignment before setting `can_fire`.
- Estimates target angular size so larger/closer targets allow a bounded wider firing tolerance.
- Scales accuracy by enemy skill, alignment, movement and suppression. It does not roll random hit chances or apply damage.

## Default tuning
- Reaction delay: 0.25 seconds
- Aim turn speed: 180 degrees/second
- Base firing tolerance: 2.5 degrees (maximum 5 degrees)
- Projectile speed: 0 cm/second (hitscan default)
- Maximum projectile lead: 0.75 seconds
- Moving accuracy multiplier: 0.75
- Suppressed accuracy multiplier: 0.60

## Engine adapter contract
1. Supply a stable target ID, visibility result, muzzle position, current aim direction, target position and target velocity each frame.
2. Set `projectile_speed_cm_per_second` to the weapon's measured projectile speed; keep it zero for hitscan.
3. Apply `aim_direction` to the engine's aim/control rotation using the returned bounded turn step.
4. Request weapon fire only when both the combat state allows attacking and `can_fire` is true.
5. Use `accuracy_factor` as a tuning input for weapon spread or authoritative server-side accuracy. Do not treat it as a client-authoritative hit result.

## Scope boundary
This module does not perform target acquisition, line-of-sight traces, muzzle sockets, projectile simulation, hitscan traces, random spread, damage, animation, networking or replication. Those remain engine/gameplay adapter responsibilities. In multiplayer, aim and fire decisions that affect damage must be validated on the authoritative server.

## Validation
Tests cover no-target handling, reaction timing, target-switch reset, turn-rate limits, fire alignment, projectile leading, skill/movement/suppression accuracy scaling and invalid input rejection.
