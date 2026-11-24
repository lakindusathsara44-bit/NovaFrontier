# NOVA: FRONTIER — Mission 01 Outline

**Mission title:** Silent Arrival (working title)  
**Status:** Proposed vertical-slice design v0.1  
**Playable scope:** one short, bounded level  
**Primary purpose:** prove movement, shooting, enemy AI, objective flow, checkpointing, atmosphere, and mission completion.

## 1. Mission summary

The response transport is forced into an emergency landing near Asterion Outpost. The player survives, reaches the colony's outer service entrance, and attempts to restore a local access relay. A security drone identifies the player as hostile. After disabling it and restoring a limited power route, the player receives a fragmented signal from deeper inside the settlement.

**Mission end state:** the player opens the sealed service access and receives a new objective to locate the outpost control room. The larger mystery remains unresolved.

## 2. Player objectives

1. **Reach the service entrance.**
2. **Restore the access relay.**
3. **Disable the hostile security drone.**
4. **Activate the emergency power link.**
5. **Enter the outpost.**

The sequence may be reordered during greybox testing if pacing improves, but the player must always have one clear active objective.

## 3. Level flow

### Area A — Crash perimeter
- Safe onboarding space.
- Teaches movement, camera look, jump, and basic interaction.
- Visual storytelling: damaged transport debris, emergency lights, distant colony structures.
- No enemy fire during the first control prompts.
- Exit: outer service path.

### Area B — Service approach
- Introduces cover and sightlines.
- Player sees signs of an interrupted evacuation.
- A locked relay door establishes the immediate objective.
- One optional log or environmental clue; not required for progression.

### Area C — Relay yard
- First combat encounter.
- One security drone begins in a patrol/idle state.
- Player can use simple cover and choose when to engage.
- Drone communicates detection and attack intent clearly.
- Player disables the drone and accesses the relay.

### Area D — Power junction
- Short interaction objective.
- Environmental hazard may be represented by warning lights/audio only in the first slice; avoid complex hazard simulation.
- A checkpoint is activated after the power link is restored.

### Area E — Sealed entrance
- Door opens; signal audio/text fragment plays.
- Mission completion UI appears.
- Next objective is teased but not playable in this slice.

## 4. Encounter specification

**Enemy:** Frontier Security Drone (prototype archetype).

**Encounter requirements**
- Starts outside direct line of sight or at a distance that allows the player to notice it.
- Detection is based on a simple, testable range and line-of-sight check.
- It gives an alert cue before dealing damage.
- It cannot damage the player through solid cover.
- It can be defeated with the prototype rifle using tuned, testable damage values.
- It stops attacking immediately when disabled or destroyed.

**Difficulty target:** a first-time player who understands firing and reload should be able to defeat one drone without perfect aim. Exact health, damage, fire rate, and ammunition values are established through playtesting.

## 5. Tutorialisation

- Use short prompts near the relevant action.
- Do not show all controls at once.
- Prompt can be dismissed or naturally disappear after successful use.
- Desktop prompts show keyboard/mouse bindings; Android prompts use touch-control hints.
- Essential tutorial information must be available as text and not rely only on voice.

## 6. Story beats and dialogue placeholders

1. **After landing:** a brief radio check establishes that the player is separated from the response team.
2. **At the relay:** the player finds the system manually locked from inside the colony network.
3. **After drone encounter:** a brief radio line notes that the security unit should not be active.
4. **At mission end:** a fragmented transmission repeats the unknown signal pattern.

Dialogue is placeholder-only. Final wording, voice, and character names are not approved.

## 7. Greybox layout requirements

- A clear spawn point and safe onboarding area.
- One readable route to the first objective.
- One combat arena with cover and no unavoidable crossfire.
- A short interaction space.
- One checkpoint location.
- One mission exit/trigger.
- Navigation landmarks visible from the player's normal camera height.

Use primitive geometry first. Do not create final art, detailed props, or expensive lighting before the layout is fun and readable.

## 8. Required gameplay systems

- First-person movement and camera.
- Weapon fire and reload.
- Health and damage.
- Enemy detection/attack/disabled states.
- Interaction interface.
- Objective tracker.
- Checkpoint/restart.
- Mission completion state.
- Basic pause/settings UI.
- Audio and visual feedback for critical events.

## 9. Acceptance criteria

Mission 01 is ready for external playtest when:
- A new player can finish the mission without developer instructions.
- Every objective has a clear start, progress state, and completion state.
- The drone behaves consistently across repeated attempts.
- The player cannot bypass the intended encounter through a broken collision or unintended route.
- Death/restart and checkpoint behaviour are reliable.
- No critical mission progression depends on optional lore.
- A complete run can be recorded for review.
- The Windows build completes a 30-minute repeated test without a crash or progression blocker.

Android acceptance is a separate gate: touch controls, safe-area layout, frame pacing, thermal behaviour, memory, and device compatibility must be measured on physical devices.

## 10. Explicit exclusions

- Multiplayer.
- Boss fight.
- Multiple enemy classes.
- Inventory crafting.
- Skill trees.
- Branching narrative.
- Large outdoor traversal.
- Cinematic cutscenes requiring complex animation.
- Final voice acting and expensive bespoke assets.

These exclusions protect the vertical-slice schedule and can be reconsidered after a successful playtest.


## Approved tone and protagonist implementation

The owner approved **Balanced Sci-Fi Action + Mystery** and a **Voiced protagonist**.

Mission 01 should therefore include:
- A short, confident opening radio exchange after the emergency landing.
- A brief spoken reaction when the player discovers the relay was locked from inside the colony network.
- A concise combat reaction when the security drone identifies the player as hostile.
- A final spoken response to the fragmented signal that raises a question rather than explaining the mystery.

Dialogue must be subtitle-supported, skippable where appropriate, and never required to understand an objective that is not also shown in the HUD. Final dialogue lines and character identity remain placeholders pending character approval.
