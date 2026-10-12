# Camera route proof — 2026-10-12 02:39 UTC

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

## Objective and scope

User authorized: prove global player -> live Third_Person_Camera -> safe SetInitialOutfit invocation -> correct execution thread. Branch: runtime-outfit-swap. Read-only target analysis and repository documentation; no DLL/source/build creation, game changes, privilege escalation, or security changes. Questions are dependent and were investigated sequentially.

Inputs: matching open REA/Ghidra 12.1.4 session for replay.exe, SHA-256 9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132. Script SHA-256 404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099. Native addresses are preferred-image VAs at image base 0x00400000; script locations are file offsets. No runtime address/base observation is claimed.

Acceptance: identify the actual global reference; observe its concrete live camera/current-control identity; observe a correctly bound one-number wrapper call and normal-outfit result; observe its thread and stock update phase. Static agreement alone cannot pass the live criteria. No implementation harness or outfit-changing test was created.

## 1. Global player — FAILED / DISPROVED inherited declaration claim

The prior checkpoint wrongly called record 0x002e46ec a global named `player`. It is local index 1 in DoWeaponSelection's twelve-entry local-variable table. The function declaration starts at 0x002e46a0; its body name is at 0x002e48a5, body at 0x002e48ba, size 1257 bytes. The first assignment copies global operand 0x055f into local operand 1. Argument `dt` precedes `player`; weapon and other locals follow. Flags alone do not establish global scope. The exact retained bytes and parsed metadata are in [local evidence](EVIDENCE/2026-10-11_camera_proof_local.json).

Therefore `Lookup("player")` is not justified by that record. This disproves that evidence for the name; it does not prove that a dynamically named global could never exist. Complete top-level global-name mapping was not decoded.

CONFIRMED bytecode observations: HandlePlayerSwitches body 0x0033a9e7 assigns its Character reference to global operand 0x055f on a branch comparing Character method operand 0x803d against global operand 0x0266. Third_Person_Camera::GetCharacterType name/body 0x0023a99b/0x0023a9c5, 23 bytes, returns exactly global operand 0x0266. This newly corroborates the camera type filter without inventing the enum's numeric value/name.

LIKELY route: controlled Actor reference -> Character conversion/member 0x87 -> camera-type check -> cached global slot 0x055f -> camera namespace -> ah. Exact Character method ordinal identification, global slot's declaring scope/name, concrete live type, stale-reference behavior and menu/load clearing remain unresolved. It is not yet a verified native pointer chain.

Preserved native helper facts: host helper 0x004aafb0 returns 0x008ec408; name lookup 0x004c4030 reads host+4; checked reference unwrapper 0x004cdc00 returns [ECX+0x18]. These are not sufficient to establish which namespace owns slot 0x055f or whether GetValue already unwraps its value. Do not call an unconditional second unwrap.

## 2. Camera wrapper — established static semantics, live safety OPEN

Preferred wrapper remains Third_Person_Camera::SetInitialOutfit(number), name/body 0x00245d36/0x00245d60, 143 bytes. Existing reconstruction: ah normal choice = argument; ah.SetOutfit(NORMAL,false,true). ah operand 0xaf is a corroborated VM member index, never a native displacement. Campaign/Pilot/Nightdress choices 0/1/2 remain distinct from runtime SetOutfit modes. No new runtime call or repeated-call safety observation was obtained.

CONFIRMED native stock one-argument call boundary: 0x0055da90, caller-cleanup/cdecl `void InvokeOne(CallbackOwner* owner, VMValue* argument)`. Assembly loads owner from the first stack argument, callable from owner+0x34, passes a stack invocation context to callable virtual slot +0xb4, then returns with plain RET. Context byte0=1, byte1=1, +4=0, argument pointer +8, count +0x28=1, return pointer +0x2c=0. It increments/decrements global invocation depth 0x008e9b84 and can clear shared VM stacks when depth returns to zero. Evidence: ev_63d079e0393f7e915b60a26b517ccef7fafa4baae7d10a54a96bdeec7012dd1b; pseudocode ev_21250d270488be27265349ce6768358fc80695a6d92364e3a0d7f15a68c828e2.

This is a callback-owner API, not a verified typed camera outfit API. Do not fabricate an owner with a CODE pointer at +0x34 or assume it is safe to substitute the camera method. Binding the actual camera instance, owner validity, number value construction/ownership and stack cleanup requirements must still be established. It reduces the manual-context problem only if a valid existing owner/binding can be obtained.

0x004abc90 is a frame-argument constructor, not a mutex or thread guard. Assembly constructs an outer frame value and an embedded NUMERIC<float> at +0x14, with float payload at outer+0x28; the frame caller stores delta time there. No camera argument constructor contract follows from this. Evidence ev_f29f5c15fc101a13683d7451f1bde6603c7ca67639abe162daea81c80439d515.

## 3. Correct stock thread/phase — CONFIRMED static path, live ID OPEN

The PE entry VA is 0x00650325: CALL security initialization, JMP 0x00650145. The startup body calls 0x0040f630 at 0x006502b7 with the four WinMain-style arguments (image base/HINSTANCE 0x00400000, NULL previous instance, command line, show mode). Evidence ev_07ef7d37cb0d06b5bcc4e76c9bf259c15ec0fc5ee161ea055931752a16f943c3; ev_4c7db36d86b6a273cd41d769b8180a9c6f1f7ddd0bc0e37ddb871e63fce5ed8f.

0x0040f630 calls application/server loop 0x004bcfd0 at 0x0040f8dd with ECX=0x008ec488. That loop has the two resolved frame-call sites 0x004bd86e and 0x004bd9ba targeting 0x004b8bf0. Evidence ev_eb2f7631fa3c0365a9f4ec6e4395ea714ea89bf634592d4b1fc56b4f02eeecd3; ev_aef204e3b68aed559aeb6ffd33cb7dd6f671f867fde194dbdced41b37590957c. Application-loop intent/control flow is also shown by pseudocode ev_616d0a80886602e8cb544be19092dc4e34c189e69125b4e482ac6c65c73fbb41; its full body assembly was not newly read.

0x004b8bf0 constructs the frame argument; at 0x004b8d25 sets ECX=0x008ec408 and calls host update 0x004c60a0 at 0x004b8d2a. On return it clears current CODE global 0x008e9b88 at 0x004b8d39. Evidence ev_7ccb5de7db34de937523846b681a0d299999d530073ca9fc76798731b51429a3.

Host update checks host+8, passes the argument through thunk 0x0055dc60 to dispatcher 0x0055dc10. The dispatcher traverses callback owners from global 0x008ec5cc and calls 0x0055da90 for each. Host assembly ev_49356be3f865dec974317bd683c9d7cc621a559f5dc0a1964749cd0d4ac8ee02; dispatcher pseudocode ev_d5481ee94d8c60eae30aad76f0cc87c30bf1870c63b3cd3faa5cf4093610fc0a. The dispatcher link layout/thunk are decompiler-supported; not promoted to an independently assembly-verified callback registration contract.

Required execution context candidate: the existing WinMain/application-server update thread, at its stock callback phase with valid world/VM state. This is stronger than an arbitrary hotkey worker or a guessed UI/render thread. Actual Windows thread ID, exclusive ownership across other call paths, a safe injection/drain point, and camera-specific reentry safety are not observed. Static call reachability does not prove a live safe DLL invocation.

## 4. Runtime observation and remaining proof

FAILED historical observation: read-only OpenProcess returned error 5. It was not retried; no privilege/security change. CONFIRMED capability limit: REA observe_native_calls is unavailable on Windows (macOS-only). No process was attached, launched, injected or called in this pass. A question requesting an existing readable debugger session/capture was sent; no reply is assumed.

Exact next evidence needed:

1. On the stock update thread, identify the namespace owning serialized slot 0x055f and verify its live value after HandlePlayerSwitches against the controlled Actor's Character and concrete camera type. Record NULL/loading/level-transition behavior.
2. Resolve SetInitialOutfit by name on that live camera, and establish that the callable's bound execution scope is that same instance; validate ah's Player_Character base/concrete instance.
3. Capture a stock wrapper invocation through CODE's existing boundary: receiver/callable identity, the one numeric argument, context and call stack/thread ID. Observe normal choice and resulting outfit; repeated-call safety requires actual repeat observations under stated conditions.
4. Compare the captured thread/call stack with 0x004b8bf0 -> 0x004c60a0 stock phase. Decide how a DLL request reaches that phase only after this evidence exists.

Do not resume broad VM parsing, unrelated input/scene systems, or FUN_005059e0. The full requested chain is NOT PROVED. The weakest premise has been corrected and a stock execution path identified; live identity, binding and safe invocation remain the blocker.

## Evidence preservation

[Canonical REA bundle](EVIDENCE/2026-10-11_camera_proof_rea_bundle.json) preserves the matching session's retained records, including this pass. [Evidence index](EVIDENCE/2026-10-11_camera_proof_index.md) lists IDs new to the prior preserved session. [Local byte evidence](EVIDENCE/2026-10-11_camera_proof_local.json) carries the script identity and exact bounded ranges. Prior custom evidence files remain historical, including the now-disproved declaration interpretation.
