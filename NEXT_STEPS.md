# Next Steps

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

## STOPPED checkpoint — preferred camera route

Read [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md) sections A-G and the current-session [evidence index](EVIDENCE/2026-10-11_focused_player_setoutfit_index.md). This supersedes the earlier FUN_005059e0 investigation recommendation below; that path is DISPROVED as an outfit method/getter.

Do not continue investigation until explicitly resumed. No game edits, DLL/source/build creation, privilege escalation, or security changes.

DISPROVED: record 0x2e46ec declares global `player`; it is a DoWeaponSelection local. Read RUNTIME_CAMERA_ROUTE_PROOF.md first. Current explicitly authorized pass: enumerate references to unnamed global slot 0x055f; inspect SetInitialOutfit, ChangeInitialOutfit, StartChangeUniform, HandlePlayerSwitches and their direct callers; reuse the existing 0x004b8bf0 -> 0x004c60a0 -> 0x0055da90 callback evidence. Stop at criterion A or B, or document the precise unresolved contract. No OpenProcess, privilege changes, game edits or DLL build.

Campaign/Pilot/Nightdress camera choices are 0/1/2 while the immediate wrapper calls runtime NORMAL=0. SS is runtime 3 and has a separate guarded stock workflow. F5 restoration is not verified. ah operand 0xaf is a VM member index, not a native byte offset. OpenProcess error 5 is recorded; do not retry through escalation. Generic CODE dispatch evidence is background; do not broaden the VM.

Historical next steps below are superseded context.

## Current instructions — runtime DLL resumption

The current user request supersedes the historical documentation-only pause below. Read [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md) first. Target VelvetSwap.dll, x86/Win32, with the requested F1-F5 controls. This pass reconciles documentation and identity; no game edits or implementation code.

Immediate next milestone: verify the native-to-script SetOutfit invocation contract and live Player_Character lifetime. Reuse the completed archived MirPtr work; start at the exact FUN_005059e0 indirect-call boundary and its recorded object update path, without repeating whole registration/loader/input discovery. Script file offset 0x0020ef0b is not callable native code; initial outfit IDs differ from the reported runtime modes. Require receiver/method/typed arguments/ABI/thread evidence before DLL invocation code, then resolve asset availability and full-state restoration for F1-F5.

Keep all source, builds, logs, and generated documentation inside this repository. Historical instructions below are context, not the current plan.

CONFIRMED milestone: the user successfully tested THE LOST Part 1 with a fresh level start. In aio\levelstructure.aio [level1] (ID=TL_1; SceneName=Levels\\Lost1.scene), Outfit=1 produced Pilot Violette and Outfit=0 produced Campaign Violette. This configuration directly controls the tested outfit selection at level load; no replay.exe patch was required.

Do not execute further investigation or change game files until explicitly instructed. The current request authorizes documentation updates only.

1. Preserve the successful test record as the baseline. The exact test time and whether the original configuration was restored are unspecified.
2. If the user requests another configuration edit, first verify the current file and preserve its original contents/hash before that authorized edit. Do not overwrite or restore anything automatically.
3. If broader outfit control is requested, test only the requested additional level/outfit with a fresh level start and record the setting and visible result. Existing configuration documents Outfit 0=Campaign, 1=Pilot, 2=Nightdress; these additional combinations are not all runtime-confirmed by this test.
4. If SS-uniform behavior is requested, investigate/test the documented Outfit=1 and LoadSSUniform relationship within the authorized scope.
5. Investigate checkpoint behavior or the exact compiled-script symbol/dispatch mappings only if needed for the next requested feature. Preserve CONFIRMED / LIKELY / SPECULATIVE distinctions.
6. Consider executable patching or runtime redirection only for a requested behavior that configuration cannot provide and only after explicit authorization. The tested Pilot-to-Campaign change needs neither.

The prior loader investigation remains documented in FINDINGS.md and FUNCTIONS.csv. FUN_0061c160 is a confirmed generic loader; Include_Scene::LoadIncludedScene remains the LIKELY internal selector. Further generic loader analysis is unnecessary for the successful configuration change.
