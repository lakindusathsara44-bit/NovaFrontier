# First-Person Player Foundation

## Scope
This is an engine-independent C++17 reference layer for first-person input and locomotion rules. It is portable so it can be compiled and tested without Unreal Engine.

It is not yet an Unreal ACharacter, a physics implementation, or a playable game build. The later Unreal adapter will translate Enhanced Input actions into PlayerInputFrame, apply movement intent to ACharacter/CharacterMovementComponent, and route look deltas to the controller camera.

## Input contract
- move.x: forward/backward axis; move.y: right/left axis.
- look.x: yaw delta; look.y: pitch delta. Pitch input is inverted.
- Device inputs must be normalized to [-1, 1] before constructing a frame.
- Keyboard, gamepad and Android touch controls share PlayerInputFrame.

## Behaviour
- Diagonal movement is normalized.
- Walk, sprint and crouch use configurable speeds.
- Sprint requires movement and is unavailable while crouched.
- Crouch is toggle-based.
- Jump emits a one-frame request only while grounded and not crouched. Unreal will own character physics and landing state.
- Yaw wraps to [0, 360); pitch is clamped.
- This layer does not perform input polling, rendering, camera, animation, collision or networking.

## Default tuning
| Setting | Default |
|---|---:|
| Walk speed | 450 cm/s |
| Sprint speed | 700 cm/s |
| Crouch speed | 220 cm/s |
| Look sensitivity | 1.0 |
| Pitch limits | -80 to +80 degrees |
| Jump velocity | 520 cm/s |

These are prototype values, not final balance decisions.

## Validation
Build with CMake and run CTest. Tests cover normalization, speed selection, crouch/sprint interaction, grounded jump gating, yaw wrapping, pitch clamping and invalid configuration rejection.
