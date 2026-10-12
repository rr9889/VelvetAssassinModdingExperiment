# Next Steps

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
