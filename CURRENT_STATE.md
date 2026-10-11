# Current State

## Current milestone — successful in-game outfit test

Recorded: 2026-10-11 15:13:55 America/Toronto / 19:13:55 UTC. This is the documentation timestamp; the exact test time was not supplied.

CONFIRMED — user-reported in-game test for THE LOST Part 1:

- Configuration: `aio\levelstructure.aio`, `[level1]`.
- ID: `TL_1`.
- SceneName: `Levels\\Lost1.scene`.
- Original `Outfit = 1`: Pilot Violette.
- Modified `Outfit = 0`: Campaign Violette.
- The level was started fresh, and the Campaign outfit appeared successfully.
- `aio\levelstructure.aio` directly controls Violette's outfit selection for THE LOST Part 1 at level load.
- No `replay.exe` patch was required.

This confirms the configuration-level selection source for the tested level. The compiled-script selector and symbol mappings remain LIKELY where previously labeled; the test does not independently verify each internal function or dispatch edge.

Only research documentation was changed during this update. The configuration change above was performed and reported by the user; the assistant did not modify game files. Whether the user has restored Outfit=1 was not reported. Earlier statements below about unchanged assets and an unperformed test describe prior checkpoints, not the current user-reported test.

## Latest milestone — 2026-10-11 15:00 America/Toronto

The upstream investigation has stopped at the highest-confidence selector candidate (criterion B), not at a runtime-confirmed Violette load.

- CONFIRMED: the native loader chain and direct callers were rechecked in the matching open Ghidra session.
- CONFIRMED: `src\live3d.mir.bin` contains the Violette model-path constants and an `Include_Scene::LoadIncludedScene` bytecode record.
- LIKELY / BEST UPSTREAM CANDIDATE: `Include_Scene::LoadIncludedScene`, script code file offset `0x0006062a`, named record at `0x00060605` in `src\live3d.mir.bin`. Its 423-byte body branches on initial-outfit and SS-uniform inputs and assigns the model identifier before calling `LoadIncludedScene2`.
- LIKELY: `LoadIncludedScene2`, code file offset `0x00061ab6`, forwards the selected field through the Actor method slot corresponding to LoadScene.
- CONFIRMED: `aio\levelstructure.aio` documents Outfit 0=campaign, 1=pilot, 2=nightdress; LoadSSUniform=1 is supported with Outfit=1. Level1 currently has Outfit=1 and LoadSSUniform=0.
- LIKELY: model path selection comes from these level settings and compiled script constants, rather than a hardcoded Violette choice in FUN_0061c160.
- SPECULATIVE / unresolved: the precise actor-property predicate that activates the player replacement branch, complete script-symbol/index validation, and the observed runtime identifier for Violette.
- `FUN_0061c160` at `0x0061c160` remains the best confirmed generic scene-instantiation point, but is superseded as the selection candidate by the script function above.
- Script offsets are file offsets, NOT replay.exe virtual addresses. No native patch address is implied.
- Game files, script bytecode, scene files, and configuration files remain unchanged. Only the four requested existing research documents were updated.

Proposed shortest reversible behavior test, NOT performed: after explicit authorization and a verified original backup, change only `[level1] Outfit` in `aio\levelstructure.aio` from 1 to 0, leave LoadSSUniform=0, start a fresh level1, inspect the model, then restore the exact original file. LIKELY expected result: Pilot -> Campaign. Runtime/debugger observation can first verify the actual identifier without changing the configuration.

## Current executable verification — CONFIRMED

- Full path: `C:\Users\Admin\Documents\Velvet Assassin\replay.exe`
- File size: 5,164,544 bytes.
- SHA-256: `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132`.
- Verified: 2026-10-11 14:49:48.8452217 America/Toronto / 2026-10-11T18:49:48.8452217Z UTC.
- Read-only filesystem metadata/hash check matches the active REA session hash and target path.
- End-of-investigation recheck: `2026-10-11T19:03:40.9197307Z` (15:03:40.9197307 America/Toronto); file size and SHA-256 unchanged.
- User authorized resumption of READ-ONLY investigation, focused on upstream scene-identifier origin. Game assets remain unchanged.

Recorded: 2026-10-11 14:43:20 America/Toronto (18:43:20 UTC).

- REA/Ghidra binary session successfully opens `replay.exe`.
- Initial Ghidra analysis completed enough for string/xref/decompile work.
- Investigation is currently READ-ONLY.
- Primary objective: player-model / Violette model selection.
- Best candidate currently identified: `FUN_0061c160` at `0x0061c160`.
- CONFIRMED: this function consumes a scene identifier and loads/instantiates that scene.
- LIKELY: this is relevant to Violette model loading if her models are supplied as scenes.
- SPECULATIVE: the actual outfit/model-selection decision happens upstream in script or scene/resource configuration.
- Exact upstream selection source is still unidentified.

Target: `C:\Users\Admin\Documents\Velvet Assassin\replay.exe`

Previously observed identity: PE, x86; SHA-256 `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132`; Ghidra 12.1.4.

This document preserves prior observations; session status and target identity must be verified when resuming. No new reverse engineering was performed to create this workspace.
