# Enemy Perception and Combat Runtime

## Purpose
Connects the portable EnemyCombatAI state machine to the existing WeaponCombat module and keeps short-lived memory of player and noise locations. Unreal supplies line-of-sight, sound events, positions, distance and health; this layer converts those observations into AI decisions and weapon-shot requests.

## Data flow
1. The engine adapter creates an EnemyObservation each simulation frame.
2. EnemyPerceptionMemory validates the sample, remembers the last visible player position, and accepts noise only inside the configured hearing radius.
3. EnemyCombatAI selects Patrol, Investigate, Chase, Attack, Search or Disabled.
4. EnemyCombatRuntime advances weapon cooldown/spread and asks WeaponCombat to fire only while the AI decision is Attack.
5. The result exposes a navigation target and shot feedback for Unreal to consume.

## Defaults
- Hearing range: 1,200 cm
- Last visible player memory: 8 seconds
- Last heard position memory: 6 seconds
- Existing AI and KX-9 tuning remain configurable.

## Contracts
- player_visible must come from an engine perception/line-of-sight result. This module does not perform raycasts.
- noise_heard is a one-frame event pulse; include its world position and distance.
- Coordinates and distances use centimetres; time uses seconds.
- Call EnemyCombatRuntime.update once per simulation frame.
- Reload is controlled by the engine adapter through begin/complete/cancel methods. Animation and reload duration remain engine-owned.
- WeaponShotFeedback represents a portable weapon simulation result. Unreal must perform the actual trace/projectile, apply damage, play effects and replicate authoritative results.
- In multiplayer, run this logic on the authoritative server or validate all inputs and shots server-side. Never trust a client-reported hit.

## Not implemented
Unreal AI Perception components, navigation/pathfinding, cover selection, hearing propagation, aim solving, animation montages, audio/VFX, actual projectile or hitscan traces, damage application, server replication, or packaged builds.
