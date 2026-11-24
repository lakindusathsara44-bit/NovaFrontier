# Engine Feasibility Gate

**Project:** NOVA: FRONTIER  
**Status:** Planning baseline; no engine build has been tested yet  
**Review date:** 2026-11-24

## 1. Decision

Use **Unreal Engine 5.8 stable as the first evaluation candidate**. Do not use Unreal Engine 5.9 Preview for the production project at this stage. The engine version is not locked until the clean-project validation checklist in this document passes on a capable Windows workstation.

This is a candidate selection, not a claim that the project has compiled, launched, or packaged.

## 2. Why UE 5.8 is the candidate

Epic's current documentation identifies UE 5.8 as the current version in its Android requirements page. Epic's release notes search result identifies UE 5.9 as Preview, not a stable production release. UE 5.8 also has documented Android toolchain requirements and automated Turnkey setup.

Official references:
- Hardware and Software Specifications: https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine
- Android Development Requirements: https://dev.epicgames.com/documentation/unreal-engine/android-development-requirements-for-unreal-engine?lang=en-US
- UE 5.9 Preview Release Notes: https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-9-release-notes?lang=en-US
- Automated Android SDK, NDK, and JDK Setup: https://dev.epicgames.com/documentation/unreal-engine/automated-android-sdk-ndk-and-jdk-setup-in-unreal-engine

## 3. Workstation assessment

Known development laptop:
- Dell Inspiron 3595
- Windows 11 64-bit
- AMD A9-9425
- Integrated Radeon R5 graphics
- 8 GB RAM

Epic's UE5 recommended development specification lists Windows 11, a quad-core Intel/AMD CPU at 2.5 GHz or faster, 32 GB RAM, and 8 GB or more graphics memory. The known laptop is below that recommended baseline, especially in memory and graphics capability. It may still be useful for lightweight source editing, documentation, Git operations, and reviewing small code changes. It must not be assumed capable of comfortable UE5 Editor use, shader compilation, cooking, or packaging.

**Workstation requirement for the full editor/build workflow:** arrange access to a more capable Windows PC or a suitable cloud workstation. A practical target is at least 32 GB RAM, a modern multi-core CPU, SSD storage, and a dedicated DirectX 12 GPU with adequate VRAM. This is a project target, not a claim that every lower-spec machine is unsupported.

## 4. Android toolchain candidate

For UE 5.8, Epic's Android requirements currently list:
- Android Studio: Koala 2024.1.2 Patch 1
- Recommended Android SDK target: API 35
- Minimum SDK for compilation: API 34
- Minimum install SDK: API 26
- Android NDK: r27c
- Build-tools: 35.0.1
- Java: OpenJDK 21.0.3

Use the engine-matched Turnkey automated SDK/NDK/JDK setup where possible. Do not mix arbitrary Android Studio, SDK, NDK, or JDK versions. Re-check the official UE 5.8 Android requirements immediately before installing the toolchain because platform-store requirements and engine hotfixes can change.

The first Android test should use a physical 64-bit Android device. A successful Windows build does not prove Android packaging works.

## 5. Clean-project validation checklist

Create a new, empty **C++ First Person** project in the chosen stable engine version. Do not import the old starter ZIP as the validation project.

Record evidence for each gate:

### Gate A — Editor and project
- [ ] Epic Games Launcher installs the selected stable engine.
- [ ] A clean C++ First Person project is created.
- [ ] The project opens in Unreal Editor without startup errors.
- [ ] The default test map runs in PIE (Play In Editor).
- [ ] Project files and engine version are recorded.

### Gate B — C++ and input
- [ ] IDE/toolchain integration is installed for the selected engine.
- [ ] A clean C++ build completes successfully.
- [ ] A small project-owned C++ class compiles.
- [ ] Enhanced Input is configured and a basic movement/action mapping works.
- [ ] No legacy input-binding assumptions are copied from the untested starter ZIP.

### Gate C — Windows packaging
- [ ] Windows Development build cooks successfully.
- [ ] Packaged build launches outside the Editor.
- [ ] Keyboard/mouse input works in the packaged build.
- [ ] Logs and build configuration are archived.

### Gate D — Android packaging
- [ ] Engine-matched SDK/NDK/JDK setup is recognized by Unreal.
- [ ] Android device is detected for deployment/debugging.
- [ ] A minimal test project packages successfully for Android.
- [ ] APK installs and launches on a physical device.
- [ ] Basic touch input and frame-time/performance observations are recorded.

### Gate E — Version lock
Only after Gates A–D pass:
- [ ] Lock the exact UE 5.8 hotfix/build number.
- [ ] Record Visual Studio/toolchain version.
- [ ] Record Android SDK/NDK/JDK versions.
- [ ] Update project documentation and CI/build instructions.
- [ ] Create the real game project from the validated template.

## 6. Development sequence after feasibility

1. First-person movement and camera using Enhanced Input.
2. Weapon foundation: KX-9 prototype, firing, recoil, reload, ammunition.
3. Health and damage framework.
4. Basic Frontier Security Drone AI.
5. Mission objective state and checkpoint/save foundation.
6. Mission 01 greybox: Silent Arrival.
7. Android input and performance pass.
8. Art/audio vertical-slice pass.
9. Multiplayer architecture and dedicated-server test only after the single-player loop is stable.

## 7. Scope and performance rules

- Build a scalable art direction; do not assume high-end Nanite/Lumen settings are viable on Android.
- Use device profiles and separate quality tiers for Windows and Android.
- Profile on target hardware early; do not use editor FPS as a proxy for packaged-device performance.
- Keep multiplayer out of the first playable milestone.
- Do not mark a gate complete without a build log, screenshot, or reproducible test note.

## 8. Current evidence status

| Check | Status |
|---|---|
| Candidate engine researched | UE 5.8 stable candidate |
| Clean project created | Not tested |
| Editor launch | Not tested |
| C++ compile | Not tested |
| Enhanced Input | Not tested |
| Windows package | Not tested |
| Android package | Not tested |
| Physical Android device test | Not tested |

**Important:** This document records a plan and official requirements. It does not claim that Unreal Engine has been installed or that any project has been compiled, run, or packaged.
