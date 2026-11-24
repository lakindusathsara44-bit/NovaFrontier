# NOVA: FRONTIER — Portable Gameplay Rules Core

This is the first implementation slice for NOVA: FRONTIER. It deliberately has no Unreal Engine dependency so gameplay rules can be compiled and tested on a modest development laptop before integration into Unreal.

## Included
- Player/enemy health: bounded damage, healing, death state, reset.
- Weapon ammunition: firing, empty-magazine behavior, reload start/completion, reserve transfer.
- Sequential mission objectives: active/locked/completed states and guarded completion.
- Small automated test executable.

## Build and test
Requires a C++17 compiler and CMake 3.16+.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

On Windows, run these commands from Developer PowerShell or a terminal where CMake and a C++17 toolchain are available.

## Integration boundary
This library is not an Unreal project and does not provide rendering, input, animation, audio, networking, or a playable build. The next integration step is to wrap these tested rules in Unreal `UActorComponent` classes and connect them to Enhanced Input and the HUD after the engine version/toolchain is validated.
