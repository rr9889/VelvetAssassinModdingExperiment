# Runtime outfit swap checkpoint

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

## Current checkpoint — 2026-10-12 02:15:38 UTC / 2026-10-11 22:15:38 America/Toronto

Author: Codex. User authorized a focused camera-wrapper pass, documentation, and then STOP. No game edits, source files, builds, outfit invocation, privilege escalation, or security changes. The earlier generic VM pass is preserved below as background; do not broaden it on resumption.

### A. Simplest established script route

CONFIRMED static records: `Third_Person_Camera::SetInitialOutfit(number iNewInitialOutfit)`, name file offset `0x00245d36`, bytecode file offset `0x00245d60`, 143 bytes. It accesses the camera's `ah` object, stores the supplied normal-outfit choice, and calls the Player_Character SetOutfit method with `OUTFIT_NORMAL`, `false`, `true`. The wrapper argument is a normal-outfit selector; it does not forward that number as the runtime SetOutfit mode.

LIKELY reconstructed semantics, supported by the wrapper, the 108-byte Player_Character SetInitialOutfit body at `0x0020f87d`, and the SetOutfit body:

```text
camera.SetInitialOutfit(choice):
    camera.ah.m_iNormalModeOutfit = choice
    camera.ah.SetOutfit(OUTFIT_NORMAL, hideequipment=false, bForce=true)
```

The wrapper uses an inline expansion referencing `ah.SetInitialOutfit` (operand `0x8048`), then explicitly invokes `ah.SetOutfit` (operand `0x8046`, underlying member index `0x46`). The high bit is an encoded VM reference/call flag, not an x86 vtable slot or byte offset.

Repeated calls: no initialization-only guard appears in the 143-byte wrapper; `bForce=true` supports forced reapplication. LIKELY usable after initialization if the referenced meshes and script instance remain valid. Runtime safety, repeated-call behavior, animation interaction, and safe scheduling are NOT verified. It does not load missing outfit scenes.

### B. Camera acquisition candidate

DISPROVED acquisition premise: record `0x002e46ec` is not a global named `player`; it is local index 1 in DoWeaponSelection. Its type `0x31`, class ID `0x0516` and default `0xffffffff` describe that local. CONFIRMED: the function copies global slot `0x055f` into it. The later RUNTIME_CAMERA_ROUTE_PROOF.md supersedes this checkpoint's former interpretation; SESSION_LOG.md preserves the historical error.

CONFIRMED bytecode: `HandlePlayerSwitches`, body `0x0033a9e7`, 429 bytes, reads global operand `0x000a`, obtains an object through member operand `0x0087`, checks a Character method operand `0x803d` against global `0x0266`, and assigns it to global `0x055f` on the matching branch. Third_Person_Camera::GetCharacterType returns the same operand `0x0266`. LIKELY: the slot caches a controlled camera reference. Its declaring namespace/name and concrete live value remain unresolved; the assignment is conditional and does not establish clearing in menus.

Static native candidate, NOT an approved/live-verified GetCurrentCamera/GetCurrentPlayer implementation:

1. `0x004aafb0`: no-argument helper returns VM host address `0x008ec408` in EAX; caller-style no-argument ABI. It returns a host, not a camera.
2. Read the host's context pointer at `host+4` (`0x008ec40c`); check for NULL before lookup. The host/context relation is assembly-supported.
3. `0x004c4030`: `VMValue* __thiscall Lookup(VMHost* ECX, MirString* name)`; uses the context's name-to-key slot `+0xa8` and value slot `+0x98`. CONFIRMED helper mechanics; SPECULATIVE application to slot `0x055f` because no owning namespace/global name is recovered. Do not use `player` based on the disproved declaration. Name lookup requires the game's narrow string object and can throw for missing names.
4. Only after verifying that the returned value has the expected namespace-reference type, unwrap it through virtual slot `+0x40`. For the checked REF<NAMESPACE> vtable `0x007de8cc`, this slot is `0x004cdc00`: `Namespace* __thiscall Unwrap(RefNamespace* ECX)`, returning `[ECX+0x18]` in EAX. This offset is assembly-derived for this reference type, not a camera field offset.
5. Validate the resulting namespace's concrete camera type, `ah`, wrapper binding, and relationship to the currently controlled actor. No exact validated native routine for those complete checks was established.

This candidate ends in a script namespace, not a recovered native C++ Third_Person_Camera layout. Live validation, NULL/menu behavior, replacement on level load, and ownership/lifetime remain unresolved. Do not cache it across level transitions based on static evidence.

### C. ah access

CONFIRMED: camera member name `ah` at script file offset `0x0023c5fa`, declared class ID `0x04ef` (Player_Character). Camera SetInitialOutfit/GetInitialOutfit/GetOutfit records consistently access member operand `0x00af` before Player_Character fields/methods. LIKELY mapping: camera namespace member index `0xaf` (175) is `ah`; a full camera member-table ordinal decoder was not completed.

Use an existing namespace member accessor/name lookup for `ah`; no fixed native `camera+offset` has been recovered. `0xaf` must never be used as a native byte displacement. GetInitialOutfit body `0x00245ed2` (23 bytes) reads `ah` then member `0x33`; GetOutfit body `0x0028d0dd` (46 bytes) reads `ah` and its runtime mode member `0x35` through the existing inline getter.

### D. Existing wrappers and simpler boundaries

CONFIRMED script-facing wrapper: SetInitialOutfit above, one number. No direct DLL-callable native function for it has been verified. Bounded native defined-string searches for `Third_Person_Camera`, `SetInitialOutfit`, `SetOutfit`, and `GetPlayer` returned no matches; this is not proof that indirect native callers do not exist. The already-read native Character interface binding does not expose a named outfit setter.

CONFIRMED additional stock script entry points:

- `Third_Person_Camera::ChangeInitialOutfit`: name `0x00245fca`, body `0x00245ff7`, 172 bytes, one number. It writes `ah.m_iNewNormalModeOutfit` (member `0x32`) and `ah.m_iLastNormalModeOrSSOutfit` (`0x34`), then uses a player-global guard and requests an animation/state transition. LIKELY: intended runtime outfit-change workflow. It is not a confirmed direct immediate caller of SetInitialOutfit; completion/apply edge remains unresolved.
- `Third_Person_Camera::StartChangeUniform`: name `0x0024619a`, body `0x002461c6`, 235 bytes, no arguments. It uses guards and requests an animation/state transition. It is not a verified explicit "set SS" operation.
- Exact qualified `Third_Person_Camera::SetOutfit` and `::ToggleUniform` records were not found in the bounded script-name search. Player_Character::ToggleUniform does exist, body `0x0020f9d5`, 289 bytes; it is a toggle with mode/normal-outfit restrictions, not an unconditional SS setter.

### E. Outfit mapping

| Requested look | Camera wrapper choice | Runtime SetOutfit mode | Evidence / confidence |
|---|---|---|---|
| Campaign | 0 | NORMAL=0 | CONFIRMED named float constants; LIKELY wrapper/mesh behavior |
| Pilot | 1 | NORMAL=0 | CONFIRMED named float constants; LIKELY wrapper/mesh behavior |
| Nightdress | 2 | NORMAL=0 with nightdress/vision mesh groups selected as the normal groups | CONFIRMED constants and configuration mesh names; LIKELY reconstructed branch behavior |
| SS | Not a verified SetInitialOutfit choice | SS_UNIFORM=3 | CONFIRMED runtime constant; exact stock camera transition/apply contract unresolved |

Runtime modes 1 and 2 are VISION and MORPHINE, not Pilot and a plain Nightdress appearance selector. Do not pass 3 to the camera's initial-outfit setter expecting SS. F5 needs both normal choice and runtime mode plus relevant equipment/camouflage/transition state; no complete restoration contract is verified.

### F. Generic invocation status

The camera wrapper reduces the desired outfit operation to one existing script method. A native-to-script call boundary is still needed from a DLL; no simpler typed native outfit helper was established. Do not extend generic VM analysis yet. Prefer resolving how stock code obtains/binds/calls this camera method.

Preserved background: CODE@mtl vtable `0x007e0434`, call slot `+0xb4` (`0x007e04e8`) = `0x00463ce0`. CONFIRMED assembly: ECX is CODE*, one invocation-context stack argument, RET 4; it passes `CODE+0x24` and that context to `0x00461d40` (ECX remains CODE*, RET 8). The meaning of CODE+0x24 as embedded execution scope is LIKELY; do not mislabel it as the script file offset or assume it is a bytecode address.

The wrapper pushes `[CODE+0x3c]` and CODE* onto global VM state at `0x008e9b90`, including stack begin/end `0x008e9b94/0x008e9b98`, and removes them on return when context+4 is zero. The executor also writes global `0x008e9b88`. Thread safety and required Windows thread ID are unresolved; an arbitrary hotkey worker thread is not validated. Previously inspected context fields: arg pointers begin +8; argument count +0x28; return VM-value pointer +0x2c for value-return paths. These facts do not make the complete invocation context/lifetime contract safe.

### G. Remaining blocker and exact resume point

No first outfit-changing DLL test is ready: resolve unnamed global slot `0x055f` and validate its live camera relationship, bind the existing one-number wrapper or stock ChangeInitialOutfit entry, and establish a safe execution thread/lifetime and loaded mesh prerequisites. Native scalar-value construction and complete F5 restoration are still unverified if no typed helper removes those needs.

On explicit resumption, start with the correction in RUNTIME_CAMERA_ROUTE_PROOF.md, enumerate references to unnamed slot `0x055f`, and examine the named camera wrappers/direct callers and established stock callback phase only. The local `player` declaration cannot justify a native global-name lookup. Do not revisit FUN_005059e0 as an outfit path, generic loaders, input discovery, or whole-VM dispatch.

FAILED external observation: replay process PID 33896 was already present; read-only OpenProcess(0x1010) returned Windows error 5 (access denied). No process memory was read, no target invocation performed, and no privilege/security change attempted. No live Player_Character or camera pointer was validated.

### Preservation and stop status

All material earlier-session findings, failed calls, and evidence IDs are indexed in FINDINGS.md, SESSION_LOG.md, and [the evidence index](EVIDENCE/2026-10-11_focused_player_setoutfit_index.md); [retained inline records](EVIDENCE/2026-10-11_focused_player_setoutfit_session.json) preserve their actual responses and scoped shell output. No fresh evidence query was used to create those files. The old recommended FUN_005059e0 investigation is DISPROVED/superseded. Investigation stops after this documentation checkpoint; the session was not closed or queried again for documentation.

## Historical reconciliation — 21:34 local (superseded by the checkpoint above)

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
