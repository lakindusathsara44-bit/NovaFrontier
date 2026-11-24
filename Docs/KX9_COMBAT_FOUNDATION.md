# KX-9 Service Rifle — Combat Foundation

## Scope
Portable C++17 weapon rules layered over the shared WeaponAmmo implementation. This module is suitable for unit testing without Unreal Engine; it does not trace bullets, spawn projectiles, play audio, animate reloads, or apply damage to world actors.

## Prototype behaviour
- Configurable rounds per minute, magazine/reserve ammo, base/maximum spread, spread growth/recovery and vertical recoil feedback.
- Fire-rate cooldown is advanced by tick(delta_seconds).
- Firing is rejected while cooling down, reloading, or out of ammunition.
- Each accepted shot consumes exactly one round and returns the spread and recoil values for the game layer to apply.
- Spread grows per shot up to its configured maximum and recovers over time to the base value.
- Reload start/completion/cancellation delegates to WeaponAmmo. The Unreal layer should call complete_reload only from the reload animation notify/event, not immediately when reload starts.

## Unreal integration boundary
The future Unreal weapon actor/component will:
1. Read Enhanced Input fire/reload actions.
2. Call tick with frame delta time.
3. Call try_fire when firing is requested.
4. On fired=true, perform a camera/weapon trace or projectile spawn, apply returned recoil, and pass hit damage to the target Health component.
5. Start the reload montage after begin_reload succeeds and call complete_reload from its authoritative animation event.
6. Replicate authoritative firing/ammo state on the server when multiplayer is implemented.

## Initial KX-9 tuning
The defaults are prototype values only: 600 RPM, 30-round magazine, 90 reserve rounds, 0.35-degree base spread, 2.5-degree maximum spread, 0.25-degree spread growth per shot, 1.5-degree/second recovery and 0.65-degree vertical recoil feedback. Balance these after hands-on playtesting.
