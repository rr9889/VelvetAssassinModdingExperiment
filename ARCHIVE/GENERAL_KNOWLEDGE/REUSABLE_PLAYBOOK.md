# Reusable playbook

SESSION EVIDENCE denotes an archived observation, tool result or explicit user report. GENERALIZED INFERENCE denotes transferable advice, not universal engine behavior. See KNOWLEDGE_INDEX.md for provenance and limits.

## Workflow

Reconnaissance → inventory game files → determine whether configuration/data already exposes the desired behavior → establish source of truth → make backups and hashes → make one minimal reversible change → verify in game → only then escalate to scripts, binary analysis, runtime hooks, or DLL injection.

1. **Reconnaissance.** GENERALIZED INFERENCE: define the outcome, timing, actual installation and permissions. SESSION EVIDENCE: executable identity was ultimately established by full path, size and SHA-256; initial provider setup was blocked.
2. **Inventory.** GENERALIZED INFERENCE: list extensions and loose/packed resources before focusing on the executable. SESSION EVIDENCE: outfit controls existed in loose aio configuration and compiled mir script; executable defined strings lacked the variant paths.
3. **Find direct controls.** GENERALIZED INFERENCE: inspect configuration comments and script/reflection names. SESSION EVIDENCE: levelstructure.aio documented Outfit and LoadSSUniform.
4. **Establish source of truth.** GENERALIZED INFERENCE: connect the exact level ID, scene and setting to observed baseline behavior. SESSION EVIDENCE: the user verified [level1]/TL_1/THE LOST Part 1 and fresh-load outfit outcomes; static comments alone were not behavioral proof.
5. **Preserve originals.** GENERALIZED INFERENCE: before an authorized edit, hash and back up the target; verify the copy. SESSION EVIDENCE: executable/script hashes were recorded, but original configuration restoration was not reported. Do not claim a verified backup existed for every test.
6. **Minimal experiment.** GENERALIZED INFERENCE: predict a result, change one variable, specify clean-start/checkpoint conditions, observe and restore. SESSION EVIDENCE: Outfit 1→0 with a fresh level produced Campaign without an executable patch; the user later confirmed 2=Nightdress.
7. **Stop or escalate.** GENERALIZED INFERENCE: stop when the requested outcome has a simpler proven control. SESSION EVIDENCE: level-load appearance was solved; runtime switching/disguise were separate later goals and were not runtime-tested.

## Escalation gates

- **Exposed scripts:** configuration cannot express the desired timing/state. Inspect existing actions before adding code. SetOutfit/ToggleUniform records supported this direction, but were not runtime-tested.
- **Resource substitution:** identifiers and compatibility/rollback are known. Enemy rig mismatch made naive substitution unjustified here.
- **Static binary analysis:** simpler selectors are absent or native semantics need clarification. Trace the producer of the choice before generic loader internals.
- **Runtime observation:** static evidence cannot resolve dispatch, caching, identity or gameplay effects. Obtain authorization appropriate to the observation scope.
- **Hook/DLL:** a stable target and ABI exist, but existing activation routes cannot meet the goal. Stock wardrobe behavior does not prove a DLL is needed.
- **Binary patch/custom format:** simpler adequate routes are exhausted and version-specific semantics, ownership and rollback are understood. No executable patch was needed for the tested outcome.

These gates are GENERALIZED INFERENCE, not actions performed during archival.
