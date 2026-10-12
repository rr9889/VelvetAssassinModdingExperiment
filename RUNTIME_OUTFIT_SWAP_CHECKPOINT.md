# Runtime outfit swap checkpoint

Recorded: 2026-10-12 01:34 UTC / 2026-10-11 21:34 America/Toronto. Author: Codex.

User authorized: reconcile existing milestone documentation, confirm Git/session/binary identity, and identify the immediate blocker. No game modifications, broad reverse engineering, or implementation code in this pass.

## Current milestone (8 lines)

1. CONFIRMED: branch `runtime-outfit-swap`, HEAD `cd0b32f`; worktree was clean on entry.
2. CONFIRMED: REA opened the documented PE x86 executable with Ghidra 12.1.4; session and filesystem hashes matched.
3. CONFIRMED, user reported: fresh THE LOST Part 1 loads Pilot with initial Outfit=1 and Campaign with initial Outfit=0.
4. CONFIRMED in prior records: SetOutfit is a 1436-byte script body at file offset `0x0020ef0b`, not a native function address.
5. LIKELY, archived report: SetOutfit takes a number and two booleans; runtime modes are Normal=0, Vision=1, Morphine=2, SS=3.
6. LIKELY, archived report: Campaign/Pilot use a separate normal-outfit field; SetOutfit also affects camouflage state.
7. UNRESOLVED: live Player_Character acquisition, method binding, VM argument/ABI contract, and safe game-thread execution.
8. Target: `VelvetSwap.dll`, x86/Win32; next milestone is one verified native-to-script SetOutfit invocation contract.

## Requested controls

| Key | Requested behavior | Current implementation evidence |
|---|---|---|
| F1 | Campaign | Needs normal-outfit selection plus the verified runtime invocation path |
| F2 | Pilot | Needs normal-outfit selection plus the verified runtime invocation path |
| F3 | Nightdress | Exact runtime operation and resource prerequisites remain unresolved |
| F4 | SS uniform | Reported runtime SS mode=3; boolean meanings and resource prerequisites remain unresolved |
| F5 | Restore previous/original outfit | Needs capture and restoration of the relevant mode, normal outfit, and dependent state |

The table describes required behavior, not callable APIs. Initial configuration Outfit IDs must not be used as runtime SetOutfit IDs. F5 fallback details can be settled when the actual state contract is known.

## Immediate blocker

There is no verified DLL-callable contract connecting the live Player_Character script instance to SetOutfit. Missing pieces are the object acquisition/lifetime contract, method identity, typed argument construction, native dispatcher calling convention and cleanup, and a safe execution point on the owning game/VM thread. A hotkey polling loop cannot supply those missing pieces.

The known LoadScene adapters (`FUN_00612ab0`, `FUN_00609f60`, `FUN_00606840`) establish script-to-native wrapper invocation. They do not establish a native-to-script SetOutfit entry. `FUN_006140e0` is registration code, not a proved SetOutfit dispatcher. Do not cast `0x0020ef0b` to a function pointer, assume ECX/EDX arguments, write a hypothetical actor+0x80 field, or treat SS resource loading as a verified disguise switch.

## Reuse completed focused work

The later pass survived in [ARCHIVE/copypaste of last session.txt](ARCHIVE/copypaste%20of%20last%20session.txt), but its large documentation patch failed (see [archived lessons](ARCHIVE/GENERAL_KNOWLEDGE/LESSONS_LEARNED.md), "Applied state versus intended state"). Reuse this record instead of repeating its queries.

| Saved location | Existing observation | Boundary still unresolved |
|---|---|---|
| Archive lines 636-638 | Report of three SetOutfit arguments, runtime modes, separate normal outfit, IsCamouflaged side effect | Raw script decoding behind this report is absent from the saved excerpt; retain LIKELY, not runtime-confirmed |
| `FUN_004f6a00`; `ev_1173251668b273dfd1ed0376c2f6422c2fc9fad2f7ae479f0693717536125e57` | Saved pseudocode returns a constructed object subobject at +0x1c | This is not established as retrieval of the current live player |
| `FUN_004ff7b0`; `ev_5db63bc257cf57979d7e9515dfbf73592a7e9da0badc9869e46335b65609c8ca` | Saved MirPtr update code checks binding/version state and auxiliary object fields | Mapping to Player_Character and SetOutfit remains unverified |
| `FUN_005059e0`; `ev_70d4df4b96e5cd7cd67b343f7dbd1414d6bf1a3f88a52bfe8b120ab0ef296ecb` | Saved pseudocode calls MirPtr update, checks +0x44/+0x48, then indirectly calls through vtable+8 | Actual callee, argument contract, and relevance to SetOutfit remain unverified; decompiler warns about the indirect jump |

These are historical Ghidra pseudocode observations bound to the matching executable, not assembly-verified APIs or runtime tests. Other registration, singleton, resource, and thread candidates are already present in the archive. No fresh analysis of these functions was performed in this resumption.

## Next bounded task

Reuse the saved `FUN_005059e0` / `FUN_004ff7b0` evidence first. Investigate the exact indirect-call boundary in `FUN_005059e0`, using assembly to establish receiver, arguments, cleanup, and the target object's provenance. Follow only the immediate target/object producer needed to decide whether this can invoke the existing Player_Character method. If it is unrelated, record that bounded result before selecting another evidence-backed candidate.

Success requires an evidence-backed object/method/typed-argument/ABI/thread contract for SetOutfit, or a precise remaining boundary. No DLL implementation should infer a callable address or ABI from the script file offset. Resource availability and full-state restoration are subsequent requirements before enabling all requested hotkeys. Do not restart generic scene-loader, input-system, or whole-VM discovery.

## Identity and scope evidence

- Executable: `C:\Users\Admin\Documents\Velvet Assassin\replay.exe`, 5,164,544 bytes; SHA-256 `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132`.
- Script: `C:\Users\Admin\Documents\Velvet Assassin\src\live3d.mir.bin`; filesystem SHA-256 `404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099` matches prior records.
- REA 6.4.0: binary_session initially returned open=false; open_binary used provider_id=ghidra; subsequent binary_session confirmed PE/x86, matching path/hash, Ghidra 12.1.4. Run ID `ccb75d1f-9e0d-486a-8b79-0b85287d17ac`.
- No analysis query was run. Session analysis activity was not_observed; opening/binding does not prove a new completed Ghidra analysis. close_binary was rejected with "MCP tool call requires approval, but approval policy is never"; session remains open. No alternate close route was attempted.
- No game execution, invocation, asset edits, source code, builds, or tests in this pass. Only repository documentation was reconciled. Configuration restoration remains unreported; executable/script identity does not certify all assets as historically original.
- All future source, build products, logs, findings, and generated documentation belong under this Git repository. The game directory is the analysis/deployment target, never a source/build workspace.

## Superseded instructions

This checkpoint and the current user request supersede the old Ctrl+Q cycling/replay.exe-patch/bytecode-insertion plans in MILESTONE_GUIDE.md, BYTECODE_PATCH_STRATEGY.md, and IMPLEMENTATION_ROADMAP.md. Examples in AGENT_PRIME_DIRECTIVES.md and INVESTIGATION_TASK_TEMPLATE.md claiming native assembly/signatures at script offset `0x0020ef0b`, VERIFIED percentages, actor+0x80, global cdecl assumptions, or three successful runtime outfit tests are illustrative and unsupported. Their useful evidence/scope rules remain applicable, but their examples do not establish facts or authorization. An x86 compiler assembly comparison cannot validate custom VM bytecode as x86 machine code. The shown `codex search/extract/decompile/compile` commands and script compiler support were not established as available capabilities.
