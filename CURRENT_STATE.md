# Current State

## Bound scope live result — 2026-10-12T13:57:52Z, Codex — STOP

CONFIRMED: real replay.exe PID 31512, exact SHA-256 and loaded x86 PE identity VERIFIED, image base `0x00400000`; observer TID 18272 is not a game-thread identification. One manual loading session produced startup/F1/F2/F3 snapshots (counts 1/24/12/4), 172 passive scope records spanning 177.091 seconds, and VM depth/current-CODE variation. Incidental F4/F5 records remain observation-only; no SS or restoration work was performed.

CONFIRMED embedded objects: CODE `0x2769E77C` -> REF `0x2769E7A0`; CODE `0x27AEEE04` -> REF `0x27AEEE28`; CODE `0x36960B74` -> REF `0x36960B98`. Each listed CODE passed `0x007e0434`; each reference first word matched `0x007de8cc`. All 244 exact-type-gated scope records returned payload candidate `0x00000000` from `REF+0x18`. The other 6 scope records lacked an exact reference match and did not enable payload reads. Successful payload reads stayed null while binding addresses and ordinary VM samples changed; the null pointer was never followed. No readable/non-null payload or surrounding pointed-to type metadata exists in this capture.

CONFIRMED: the missing guarded read is fixed. DISPROVED for these samples: the first callback's CODE scope supplies a non-null bound namespace through this validated payload offset. The embedded REF_NAMESPACE marker interpretation itself is supported, not disproved. UNRESOLVED: why the reference is null, payload namespace identity, slot ownership/storage/live VM value, camera/current-control identity, and safe game/update-thread handoff. A stable null pointer is not camera proof.

STOP at the precise null-payload boundary: Task 3's concrete-namespace prerequisite was not met, so no slot access, accessor invocation, layout guess, or further REA analysis was attempted. This establishes neither a live camera nor a concrete namespace owning slot 0x055f. v0.1 is **NO-GO**. Single current acquisition blocker: a verified passive mapping from this null CODE scope to the actual namespace/state owning global slot `0x055f`. Thread handoff remains a separate required GO condition after acquisition; it is not claimed to be the only remaining v0.1 issue.

Exact next implementation step, after that missing mapping is supplied: extend guarded passive observation to the validated namespace/state receiver's indexed storage for slot 0x055f, then record the VM value and its verified reference type. The preceding evidence step is the existing `FUN_00459940` scope-mode/global-storage mapping for the actual receiver. Never call the accessor from the observer or substitute guessed offsets; do not resume this investigation automatically.

Immutable evidence: [live log](EVIDENCE/2026-10-12_bound_scope_live.log), [capture summary](EVIDENCE/2026-10-12_bound_scope_live.json), [critical result](EVIDENCE/2026-10-12_bound_scope_result.json). Capture SHA-256 `3056ff7a58bf982e7445e7842f475f55c194a97cf7290083da8b0a6cff71687e`. [Build record](EVIDENCE/2026-10-12_bound_scope_build.json) and [tests](EVIDENCE/2026-10-12_bound_scope_tests.log): Win32 Release, 3/3 passed; original assertions retained with new typed-read safety checks. Zero game-state writes, game-function calls, hooks, executable or asset edits. Samples are non-atomic worker observations, not callback-entry traces or lifecycle proof.

Repository: HEAD `973c385`, branch `runtime-outfit-swap`, dirty from this source/document/evidence pass plus pre-existing untracked files; no commit/push. Historical evidence preserved. The computer-use launch attempt was rejected with "Computer Use was not approved to use replay"; no alternate launch was attempted. The user's subsequently completed manual capture supersedes the earlier pending-capture blocker. The immutable [attempt record](EVIDENCE/2026-10-12_bound_scope_attempt.json) describes the earlier checkpoint only. Investigation STOPPED after this critical result and documentation.



## Bound namespace probe prepared — 2026-10-12, Codex

CONFIRMED: entry HEAD `973c385`, branch `runtime-outfit-swap`; checkpoints `536114e`, `56a18b8`, `973c385` present. Entry worktree already had untracked build, transcript, and spatial-analysis files; preserved. No commit or push.

CONFIRMED implementation: exact replay.exe identity gate and existing guarded reads retained. After exact rebased CODE-vtable validation, `binding+0x24` is recorded as the embedded object address; its first word is tested against rebased REF_NAMESPACE vtable `0x007de8cc`. Only a match enables guarded `embedded_ref+0x18` observation as `boundNamespacePayloadCandidate`. A guarded payload first-word read records readability only, without asserting namespace/camera type. `[BINDING_SCOPE]` separates these fields and records every one-second passive sample. Zero game-state writes, game function calls, or hooks.

CONFIRMED: new `build/VelvetSwap-bound-scope/Release/VelvetSwap.dll` is Win32/x86 Release; all three CTest cases pass, including 31 guarded-read/identity assertions. Existing tests retained. [Build evidence](EVIDENCE/2026-10-12_bound_scope_build.json), [test log](EVIDENCE/2026-10-12_bound_scope_tests.log). Old DLL/log/evidence untouched; historical first-live log hash still `c35e5516534355e868aff4e3f20cd9911f00f7c844076fb1e624db17e8c0ce48`.

UNRESOLVED / PENDING at this preparation checkpoint: one user-operated real-game capture using the proven x86 Cheat Engine manual procedure. New log contains only REJECTED isolated test sessions; no new live address or payload claimed. Current gating blocker is the missing new-contract live capture. Next step: enter controllable gameplay, load the new DLL, decline optional exported calls, capture startup/F1/F2/F3 plus 15–20 seconds of ordinary gameplay; freeze evidence, then use only verified passive namespace/slot mappings. Do not inspect slot ownership before that payload exists. Camera identity and update-thread handoff remain UNRESOLVED; v0.1 is NO-GO. This preparation record does not claim stop condition A, B, or C.


## First real v0.0 live capture — 2026-10-12T04:01:25Z (Codex)

CONFIRMED: VelvetSwap.dll v0.0 loaded in real `replay.exe` PID 3220. The log reports `identity=VERIFIED` for `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132` at base `0x00400000`, module size `0x00585E84`, PE timestamp `0x49EF137B`. The immutable capture has 53 snapshots, SHA-256 `c35e5516534355e868aff4e3f20cd9911f00f7c844076fb1e624db17e8c0ce48`: [live log](EVIDENCE/2026-10-12_v0.0_first_live.log), [parsed summary](EVIDENCE/2026-10-12_v0.0_first_live.json). Active append log: `build/VelvetSwap-v0.0/logs/VelvetSwap.log`.

CONFIRMED observed fields: HOST context `0x0357BD00`, context vtable `0x0081FC04`, accessor `0x00459940`; VM depth varied 0–8, callback head `0x26E1EE5C`, candidate owner `0x26E1EE48`; stored binding `0x279BF664`, CODE vtable `0x007E0434` (`matches_CODE=1`), raw `scope_word=0x007DE8CC`. Host/callback candidate values were stable in captured snapshots. F1/F2/F3 snapshot edges appear 13/7/7 times; do not treat that as exactly one keypress each.

LIKELY: binding candidate participates in VM execution because `current_code` sometimes equals `0x279BF664` at nonzero depth. The raw scope word matches the known REF_NAMESPACE vtable, consistent with an embedded reference. This is not the referenced namespace pointer and does not establish camera identity. CONFIRMED unresolved: REF_METADATA payload, slot `0x055f`, active camera/current-control identity, outfit state, and actual update-thread context. Pointer stability alone is not camera proof.

Next single blocker: identify the concrete namespace payload of the CODE-bound reference and determine whether it owns slot `0x055f`; the live capture did not read that payload. v0.1 outfit writes remain NO-GO. No hooks, game-function calls, game-state writes, game-file edits, or new REA analysis were performed.


## Runtime instrumentation v0.0 / live-load preparation — 2026-10-12T03:58:37.188543+00:00, Codex

CONFIRMED: observation-only VelvetSwap.dll built as Win32/x86 Release with MSVC 19.29.30159, v142, Windows SDK 10.0.19041.0. Three isolated CTest cases pass. DLL: `build/VelvetSwap-v0.0/Release/VelvetSwap.dll`; log: `build/VelvetSwap-v0.0/logs/VelvetSwap.log`. [Phase evidence](EVIDENCE/2026-10-11_v0.0_runtime_instrumentation.json) preserves DLL identity, validation logs and original REA evidence IDs.

CONFIRMED implementation: exact replay.exe disk SHA-256 and loaded x86 PE identity gate; rebasing; null/range/readable-page/SEH guarded passive host/context and VM/callback metadata reads; F1-F5 edge-triggered logging/snapshots while the game is foreground; one-second passive samples and one-minute heartbeats. No game function invocation, outfit writes or hooks. DLL pins itself until process exit; its own VelvetSwapStop export requests asynchronous observation stop. Do not manually unmap it during startup or observation.

LIKELY interpretation only: callback head minus 0x14 is an owner candidate, with stored binding at +0x34. CODE-vtable matches allow a raw +0x24 word read; this is not a camera identity. REF_NAMESPACE matches permit the documented +0x18 payload read; namespace payload is not automatically the current camera. Slot 0x055f, camera and native outfit fields log UNRESOLVED. Observer thread ID and passive changes do not identify the update thread or callback frequency.

CONFIRMED: Cheat Engine 7.7 is installed at `C:\Program Files\Cheat Engine`; `cheatengine-i386.exe` exists. No download/install, privilege change, game-file edit, REA call or game memory access was performed for the loader search. Superseded by the real-game capture documented below: the original common-tool check preceded manual loading and found no game process at that time.

Superseded: live loading and capture are complete. See the first real v0.0 live capture section below; the remaining namespace-payload blocker is stated there. v0.1 remains NO-GO. Prior static-investigation and no-source instructions below are historical and superseded by this expressly authorized observation-only build/load phase.

## Latest checkpoint — 2026-10-11 22:15:38 America/Toronto / 2026-10-12 02:15:38 UTC

1. CONFIRMED: matching replay.exe session/hash; branch runtime-outfit-swap; no game edits, code, build, or outfit test.
2. CONFIRMED static: Third_Person_Camera contains Player_Character member `ah`; wrapper name/body offsets 0x245d36/0x245d60.
3. LIKELY semantics: SetInitialOutfit(choice) writes ah normal choice and calls SetOutfit(NORMAL,false,true); runtime repeated-call safety untested.
4. DISPROVED: record 0x2e46ec is a global named `player`; it is local index 1 in DoWeaponSelection. CONFIRMED: its body reads global slot 0x055f; HandlePlayerSwitches conditionally writes that slot under a camera-type operand comparison.
5. SPECULATIVE: native acquisition through slot 0x055f. Its declaring namespace/name, concrete live value and lifetime are unresolved; Lookup("player") is not supported by the local declaration. See RUNTIME_CAMERA_ROUTE_PROOF.md.
6. FAILED / DISPROVED: FUN_005059e0 is a generic inspection path; it is not an outfit setter or current-player getter.
7. CONFIRMED generic CODE call boundary 0x00463ce0 -> 0x00461d40 mutates global VM state; thread safety remains unresolved.
8. FAILED live read: OpenProcess error 5; current camera/Player_Character pointer, safe native wrapper call, and full F1-F5 contract remain unresolved.

See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md) sections A-G for the preferred camera route, limitations, and exact resume boundary. ChangeInitialOutfit/StartChangeUniform are stock script candidates; no direct native outfit helper was verified. Investigation is STOPPED after documentation. Earlier checkpoints below are historical.

## Reconciled runtime DLL milestone — 2026-10-11 21:34 America/Toronto

See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md) for the authoritative eight-line milestone, identity checks, archived focused work, and next bounded task. Target: VelvetSwap.dll, x86/Win32, F1=Campaign, F2=Pilot, F3=Nightdress, F4=SS, F5=restore previous/original.

CONFIRMED: branch runtime-outfit-swap; clean worktree before this documentation pass; executable/script hashes match documented identities. REA/Ghidra binding was verified without analysis queries. The close request was rejected by client approval policy; session remains open.

UNRESOLVED immediate blocker: live Player_Character acquisition and a verified native-to-script SetOutfit invocation contract (method, typed arguments, ABI, lifetime, safe thread). SetOutfit's file offset is not a native address. LIKELY archived correction: three script arguments and Normal/Vision/Morphine/SS runtime modes; Campaign/Pilot use a separate normal-outfit field. Earlier native-signature milestone examples are unsupported.

Historical checkpoints below retain their original scope and timestamps. Fresh-level config success is not proof of runtime hot swapping. No game files or implementation code were changed in this resumption.

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
