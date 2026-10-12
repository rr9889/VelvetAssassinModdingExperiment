# Next Steps

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
