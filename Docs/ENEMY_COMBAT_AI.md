# Frontier Security Drone — Enemy Combat AI Foundation

## Scope
A deterministic, portable C++17 decision state machine. It can be unit-tested without Unreal Engine and is designed to sit behind a future Unreal perception/movement adapter. It does not perform navigation, line-of-sight traces, animation, audio, weapon firing, damage application, or network replication.

## States
- **Patrol:** default state; the Unreal adapter may follow a patrol route.
- **Investigate:** move toward a noise source or the last known player position.
- **Chase:** pursue a currently visible player outside weapon attack range.
- **Attack:** request an attack while the player is visible and inside attack range.
- **Search:** move around the last known target position after losing sight.
- **Disabled:** terminal state after health reaches zero; only an explicit reset returns the AI to Patrol.

## Inputs and outputs
Each update receives a perception snapshot: player visibility, a one-frame noise_heard event, player distance in centimetres, and current health. Delta time is in seconds. The result tells the engine adapter the selected state and whether movement, attack, or last-known-target behaviour is requested.

noise_heard is an event pulse: send true on the frame a sound is perceived, not continuously while the sound remains in a sensor buffer. player_visible should already represent a successful perception/line-of-sight result from the engine layer.

## Default prototype tuning
- Detection range: 1,800 cm
- Attack range: 250 cm
- Search duration: 4 seconds
- Investigate duration: 5 seconds

These are initial gameplay tuning values, not final balance. The adapter should own navigation, turning, animation, weapon execution and authoritative damage. The AI state machine only expresses intent.

## Transition outline
- Patrol + noise -> Investigate
- Any active state + visible target -> Chase or Attack, based on range
- Chase/Attack + lost visibility -> Search
- Search timeout -> Investigate
- Investigate timeout -> Patrol
- Any state + health at or below zero -> Disabled

## Unreal integration boundary
A future AI Controller/Character adapter should:
1. Feed perception updates from Unreal AI Perception or an equivalent trace/sensor system.
2. Translate should_move and has_last_known_target into navigation requests.
3. Execute weapon fire only when should_attack is true and the weapon component confirms it can fire.
4. Apply health changes through the shared gameplay health component.
5. Keep authoritative state and damage on the server when multiplayer is introduced.
