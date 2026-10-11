# Session Log

## 2026-10-11 15:13:55 America/Toronto / 19:13:55 UTC — successful user-reported in-game test

- Evidence source: user report; exact test time not supplied. Timestamp records this documentation update.
- CONFIRMED configuration: `aio\levelstructure.aio`, `[level1]`, `ID = TL_1`, `SceneName = Levels\\Lost1.scene`.
- CONFIRMED original state: `Outfit = 1` produced Pilot Violette.
- CONFIRMED modified state: `Outfit = 0` produced Campaign Violette.
- CONFIRMED procedure/result: the level was started fresh and Campaign Violette appeared successfully.
- CONFIRMED conclusion: `aio\levelstructure.aio` directly controls Violette's outfit selection for THE LOST Part 1 at level load.
- CONFIRMED: no `replay.exe` patch was required.
- Internal script-symbol mappings remain LIKELY; the successful configuration test does not independently validate every reconstructed call edge.
- Original configuration restoration was not reported. No claim is made that the current configuration is back at Outfit=1 or that all game assets are historically unmodified.
- Assistant updated only CURRENT_STATE.md, FINDINGS.md, SESSION_LOG.md, FUNCTIONS.csv notes, and NEXT_STEPS.md. No game files modified and no further reverse engineering or runtime experiment performed during this update.

## 2026-10-11 15:00 America/Toronto / 19:00 UTC — upstream checkpoint

- CONFIRMED: enumerated all observed direct callers and entry xrefs for FUN_0060d970 and all three wrappers. FUN_0060d970 has only the three wrapper callers; FUN_0060daa0 has FUN_0060fe90; the other wrappers have registration references but no resolved direct callers.
- CONFIRMED: native assembly shows scene text supplied through a value object, not a wrapper-local model choice. Traced LoadResource/CreateChild argument conversion and the indirect script adapter boundary without revisiting generic loader internals.
- CONFIRMED: read existing src\live3d.mir.bin and relevant local configuration/scene bytes without extracting, modifying, or executing them. Script size 3,459,973 bytes; SHA-256 404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099.
- CONFIRMED: compiled script contains exact Violette variant-path constants; levelstructure.aio documents Outfit and LoadSSUniform controls. Lost1.scene contains the generic violette.scene placeholder.
- LIKELY: highest-confidence upstream scene-identifier selector is Include_Scene::LoadIncludedScene, bytecode file offset 0x0006062a, named record 0x00060605. It branches on iInitialOutfit/bLoadSSUniform and assigns the selected identifier before LoadIncludedScene2.
- LIKELY: the reconstructed selection maps Campaign/Pilot/Pilot_SS/Nightdress/All_Meshes constants to the native LoadScene chain. Full bytecode symbol mappings and runtime dispatch remain unverified.
- SPECULATIVE: exact player predicate linking the scene placeholder to that selection branch.
- Stopped at criterion B: high-confidence upstream candidate. No claim that criterion A (fully verified live identifier origin) is complete.
- Proposed but did NOT perform the shortest reversible configuration test: level1 Outfit 1 -> 0 with LoadSSUniform unchanged, fresh level load, then exact restoration after explicit authorization.
- Updated only CURRENT_STATE.md, SESSION_LOG.md, FINDINGS.md, and FUNCTIONS.csv. No game assets, executable, script, scene, or configuration files patched, renamed, moved, or overwritten. No runtime experiment performed.
- Active Ghidra session retained for future continuation; no close/reopen lifecycle operation performed.
- CONFIRMED end-of-investigation check at `2026-10-11T19:03:40.9197307Z` / 15:03:40.9197307 America/Toronto: replay.exe remains 5,164,544 bytes with SHA-256 `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132`. All four requested documentation files exist; FUNCTIONS.csv parses with six required columns and 44 rows.

## 2026-10-11 14:49:48.8452217 America/Toronto / 18:49:48.8452217 UTC

CONFIRMED: current executable verified through a read-only filesystem check:

- Full path: `C:\Users\Admin\Documents\Velvet Assassin\replay.exe`.
- File size: 5,164,544 bytes.
- SHA-256: `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132`.
- Exact check timestamp: `2026-10-11T18:49:48.8452217Z`.
- Active REA/Ghidra session remains open on this target; session hash matches filesystem hash.
- User authorized READ-ONLY upstream caller/identifier investigation and updates to existing research documentation. No executable or game asset modified.

## 2026-10-11 14:43:20 America/Toronto / 18:43:20 UTC

Documentation checkpoint summarizing the current conversation's prior investigation:

- REA successfully connected to Ghidra 12.1.4.
- `C:\Users\Admin\Documents\Velvet Assassin\replay.exe` opened successfully with provider_id `ghidra`.
- Initial Ghidra analysis completed and string/xref/decompile investigation was performed.
- Requested Violette model-name strings had no defined-string matches; the Violette loading-screen texture match was distinguished from model loading.
- Scene-loading references and script registration were traced.
- Best current candidate identified as `FUN_0061c160` at `0x0061c160`.
- Confirmed generic scene loading/instantiation; Violette-specific relevance remains conditional and upstream model selection remains unidentified.
- No game files patched or modified. No annotations, renames, exports, or runtime changes performed.
- Created documentation only under VA_RESEARCH from findings already visible in the conversation. No additional reverse engineering performed for this checkpoint.
- Investigation is paused by user instruction until explicit authorization to continue.

The timestamp is the documentation checkpoint, not a reconstructed timestamp for each earlier tool call. Original tool transcripts remain in the conversation; these notes are a summary.
