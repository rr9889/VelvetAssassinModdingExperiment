# Current State

## R0 recheck live result — 2026-10-12T15:24:48.975406+00:00 — STOP A

**RUNTIME-VERIFIED:** slot `0x055f` is safely readable through revalidated record-0 R0. Fresh real replay.exe PID11040 was created after a recorded zero-process baseline; exactly one new distinct `VelvetSwap-R0Recheck.dll` loading session is captured. User confirms fresh process / only new DLL. Main base `0x00400000`, image size `0x00585e84`, timestamp `0x49ef137b`, executable SHA-256 `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132` VERIFIED. DLL SHA-256 `ce20030d388be94eba554f4f823269072ffcc2dc4c32f1cde7163aac01f5bf60` matches the build and loaded-module-file identity log. Win32/x86 Release; **4/4 CTest passed**, all 31 original and 48 R0/storage assertions. [Build](EVIDENCE/2026-10-12_r0_recheck_build.json), [deployment](EVIDENCE/2026-10-12_r0_recheck_deployment.json).

Nine independent valid samples: R0 **`0x0370BD00`**, raw-equal to host context, readable; exact VMACHINE vtable `0x0081FC04`, virtual +0x98 **`0x00459940`**. Seven samples classified **RECEIVER_STABLE_CODE_TURNOVER** and two **FULL_CHAIN_STABLE**. The only changed acquisition recheck field was **current CODE**, in 7/9; CODE vtable, captured-CODE reference begin/end/count, record0 address, R0, host, R0 vtable and getter stayed equal in all nine. First CODE transition: `0x2725239C -> 0x27065F50`, with table `0x27970D38..0x27970D78`, count2 and record0 at `0x27970D38`. All final receiver/storage/selection rechecks stayed equal. The snapshot rule therefore distinguishes current-CODE turnover from receiver invalidation for these samples. It supplies guarded read observations, not ownership/atomicity/ABA guarantees. The preceding opaque failure's specific field is still UNRESOLVED; current-CODE turnover is only LIKELY for that older sample.

Active mode **1**, **OWN_STORAGE_COMMON**; field90 `0`, field9c/control `0x0370BDA0`, control+0x14 **0**, fallback2c `0`. Storage begin/end **`0x2725FE18 / 0x272685A8`**, span `0x8790`, count **2169 (`0x879`)**, stride `0x10`. Index1375 is in bounds; full record **`0x27265408`** is readable. Flags dword **`0x001C0001`** (native low-byte flag mask2 clear; extra bytes have no assigned meaning/tag). Control zero selects direct return. Stored-pointer field **`0x27265414`** contains **`0x27631348`** in all nine safe independent samples. Live formula: `D(D(R0+0xc0)+0x55fc) = D(0x27265414) = 0x27631348`, conditioned on all implemented gates. STATIC-CONFIRMED operand1375/storage semantics and RUNTIME-VERIFIED live addresses remain separate evidence tiers.

Slot first word/vtable **`0x007E0534`** is RUNTIME-VERIFIED; **slot type and reference-payload layout UNRESOLVED** in this probe. It does not match the only validated slot REF_NAMESPACE payload gate, so no payload/reference candidate was read. **Third_Person_Camera/current-control identity UNRESOLVED**; pointer stability is not class or current-control proof. Update-thread handoff also **UNRESOLVED**. v0.1 **NO-GO**. **Single immediate acquisition blocker:** Unvalidated type/reference-payload mapping from slot value 0x27631348 (first word/vtable 0x007E0534) to the live Third_Person_Camera/current-control object. Safe game/update-thread execution remains an additional required GO condition; it is not claimed to be the sole remaining blocker.

**Exact next bounded step:** Resolve only the existing static type and reference-payload contract for vtable 0x007E0534, then correlate its referenced object with Third_Person_Camera/current-control using established script/type evidence. No invocation or writes. This pass is STOPPED; no further type RE, outfit invocation, build v0.1 or live session follows. Zero game writes, game calls, hooks, executable/asset edits; legacy CODE+0x24 scope experiment disabled. HEAD `0ade08b`, branch `runtime-outfit-swap`, dirty on entry/completion; previous work and historical evidence preserved, no commit/push.

Frozen [live log](EVIDENCE/2026-10-12_r0_recheck_live.log), [summary](EVIDENCE/2026-10-12_r0_recheck_live.json), [per-record fields](EVIDENCE/2026-10-12_r0_recheck_live_samples.json), and authoritative [final report](EVIDENCE/2026-10-12_r0_recheck_final_report.json). Log SHA-256 `2acd83a60fb8cae197a442a23fe296d32e90bd87898c9a4a8f770671bff24d42`. 180 log groups / **163 independent Capture calls** (17 passive_change duplicates excluded), 159 passive samples spanning **162.965 seconds**; startup/F1/F2/F3 each present once. Physical press count is not inferred from edges; user completion and same-process F1 supplementation are recorded. Extra passive duration completed the missing F1 capture requirement; no acquisition expansion occurred. Observer remains read-only/pinned until process exit; immutable evidence is a cutoff of its append log.


## R0 field recheck pass prepared — 2026-10-12T15:19:17.361429+00:00

STATIC-CONFIRMED layout preserved. Field-by-field acquisition rechecks replace the opaque aggregate rejection. Receiver snapshot rule accepts FULL_CHAIN_STABLE, or RECEIVER_STABLE_CODE_TURNOVER only after initial CODE/bounds validation and independently unchanged record-0 R0, live host alias, exact VMACHINE vtable 0x0081fc04 and getter 0x00459940. Captured CODE table and current-CODE global are distinguished. Final storage/receiver fields are independently rechecked; current CODE is not a lifetime owner of the revalidated host receiver. Guarded reads remain non-atomic and do not prove ownership or writable lifetime.

Win32/x86 Release; **4/4 CTest passed**, all 31 original and 48 R0/storage assertions. Fixture mutations exist only in the test executable, not in the DLL. Distinct DLL: `build/VelvetSwap-r0-recheck/Release/VelvetSwap-R0Recheck.dll`; SHA-256 `ce20030d388be94eba554f4f823269072ffcc2dc4c32f1cde7163aac01f5bf60`. [Build record](EVIDENCE/2026-10-12_r0_recheck_build.json), [final test log](EVIDENCE/2026-10-12_r0_recheck_tests_final.log). All earlier attempts preserved; final logs are authoritative.

RUNTIME-VERIFIED live result PENDING: no replay.exe process exists in the recorded predeployment inventory. Exactly one fresh process/manual load and startup/F1/F2/F3 plus >=30 seconds gameplay are required. Previous authoritative STOP B is preserved below; no new R0, mode or slot value claimed yet. v0.1 NO-GO pending this diagnostic capture; camera identity and update-thread handoff remain UNRESOLVED. No game writes, game calls, hooks, executable/asset edits, broad RE, commit or push. Legacy CODE+0x24 scope read remains disabled.


## CODE record-0 live result — 2026-10-12T14:55:36.567526+00:00 — STOP B

RUNTIME-VERIFIED: the corrected `code_r0_v1` observation DLL loaded into real replay.exe PID 32020 at 14:52:02 UTC; exact executable identity VERIFIED, main base 0x00400000. Frozen capture contains 89 log records from 87 independent Capture calls; 66 passive records span 68.356 seconds. Startup and F1/F2/F3 snapshots exist (log counts 1/10/5/5); physical once-each presses and controllable gameplay are user-reported, not inferred from edge counts.

RUNTIME-VERIFIED raw observation: CODE `0x27A2DAF8`, exact CODE vtable `0x007e0434`; reference begin/end `0x27134478/0x271344b8`, count 2, record 0 at `0x27134478`; R0 `0x037cbd00`, readable. Its captured pointer word equals captured host context `0x037cbd00`. R0 vtable `0x0081fc04` and virtual +0x98 entry `0x00459940` matched the STATIC-CONFIRMED accessor. This was **one independent sample logged twice**, as passive_sample and passive_change. Other logged CODE values were NULL; do not interpret unavailable R0 as a NULL payload read.

UNRESOLVED accepted chain: the aggregate post-read chain check rejected that sample and logged `R0_chain_changed_during_sample`, `chain_consistent=0`, `comparison=UNAVAILABLE`. Thus raw pointer-word equality is observed, while revalidated acquisition/equality is not accepted. The exact failed comparison was not logged. Current-CODE turnover is LIKELY given neighboring NULL-CODE records, not proved as the specific failure. Mode, +0x90/+0x9c/control/+0xc0/+0xc4/fallback fields and slot 0x055f were not read after this gate. No stored slot pointer/type/reference candidate was obtained; camera/current-control identity and safe update-thread handoff remain UNRESOLVED.

**STOP condition B; v0.1 NO-GO. Single current blocker:** no accepted revalidated CODE -> record0 -> R0 chain before the mode/storage reads. **Exact next step:** add passive field-by-field recheck diagnostics without weakening executable/type/readability/null/bounds/SEH gates; identify the changed field, then establish a consistent receiver/storage snapshot in a separately authorized pass. No more capture, analysis, or implementation in this pass.

The initial [capture summary](EVIDENCE/2026-10-12_code_r0_live.json) incorrectly derived label C from zero accepted-chain samples. Readable non-null R0 exists, so C does not apply. Original summary and raw evidence remain immutable; [final result](EVIDENCE/2026-10-12_code_r0_final_report.json) is authoritative for this classification. [Live log](EVIDENCE/2026-10-12_code_r0_live.log), [per-record fields](EVIDENCE/2026-10-12_code_r0_live_samples.json); log SHA-256 `66575df2a36c42ab023b0d9125b4356b18192dcaf54d6d9f36c7558f1900aa9b`.

Build/tests: Win32/x86 Release; **4/4 CTest passed**, 31 original plus 34 new fixture assertions. [Build evidence](EVIDENCE/2026-10-12_code_r0_build.json), [tests](EVIDENCE/2026-10-12_code_r0_tests.log). New probe uses current CODE, guarded record-0 extraction, exact getter gate, and conservative branch selection; its production/default CODE+0x24 probe is DISABLED. No game-state writes, game-function calls, hooks, asset/executable changes, REA analysis, commit, or push. HEAD 0ade08b, runtime-outfit-swap; dirty source/docs/evidence plus preserved earlier work.

Deployment limitation: two earlier manual attempts loaded the previous bound-scope DLL. [Mismatch diagnostic](EVIDENCE/2026-10-12_code_r0_loader_mismatch.json) and [distinct-name copy verification](EVIDENCE/2026-10-12_code_r0_named_copy.json) preserve the correction. `VelvetSwap-CodeR0.dll` has the same tested SHA-256 as the new DLL. Corrected load reused PID32020, which already had the older observer; do not claim a further fresh process or exclusive instrumentation. Only one new R0 session was captured. Earlier probe payload data was not analyzed or used; historical evidence remains unchanged.


## CODE record-0 observation prepared — 2026-10-12T14:37:41.383146+00:00

STATIC-CONFIRMED source implementation: guarded current CODE -> reference begin/end +0x18/+0x1c -> 0x20-byte record 0 -> receiver pointer at record+0x18. Exact CODE vtable and accessor entry gates precede typed layout use. R0 is compared by pointer value with host context; similarity of vtables does not establish equality. Namespace offsets and branch predicates follow the preserved FUN_00459940 assembly. Invalid/null/unreadable bounds or unresolved control/mirror branches do not enable slot value reads. Mode 4/6 local NULL is recorded separately from an unknown fallback return; no fallback call or unwrap call occurs. Mode 7 uses its validated redirected storage fields. Rechecked selection fields are logged; samples remain non-atomic.

The production/default capture does not read the historical CODE+0x24 scope/payload. Historical fixture assertions remain intact with explicit fixture opt-in, alongside a new default-disabled check. Slot-value REF_NAMESPACE payload metadata is read only on its own exact type match and does not prove Third_Person_Camera/current-control identity.

Build: Win32/x86 Release, MSVC 19.29.30159/v142/SDK 10.0.19041.0; **4/4 CTest passed**, 31 original plus 34 new R0 fixture assertions. DLL: `build/VelvetSwap-code-r0/Release/VelvetSwap.dll`; SHA-256 `bb7ba09144353843678008aebd57595276334256bea1827540eb92926aa9df28`. [Build evidence](EVIDENCE/2026-10-12_code_r0_build.json), [tests](EVIDENCE/2026-10-12_code_r0_tests.log).

RUNTIME-VERIFIED results are PENDING at this preparation checkpoint. One fresh-process manual gameplay capture has been requested: startup/F1/F2/F3 and 25–30 seconds ordinary gameplay. New log currently contains only REJECTED test hosts. No new live R0, equality, mode, slot value, or camera identity is claimed. v0.1 remains NO-GO; capture is required to classify stop A/B/C. No game-state writes, game-function calls, hooks, assets/executable edits, new REA analysis, commit, or push. Earlier uncommitted storage evidence and unrelated untracked files preserved.


## CODE record-0 observation prepared — 2026-10-12T14:37:11.460205+00:00

STATIC-CONFIRMED source implementation: guarded current CODE -> reference begin/end +0x18/+0x1c -> 0x20-byte record 0 -> receiver pointer at record+0x18. Exact CODE vtable and accessor entry gates precede typed layout use. R0 is compared by pointer value with host context; similarity of vtables does not establish equality. Namespace offsets and branch predicates follow the preserved FUN_00459940 assembly. Invalid/null/unreadable bounds or unresolved control/mirror branches do not enable slot value reads. Mode 4/6 local NULL is recorded separately from an unknown fallback return; no fallback call or unwrap call occurs. Mode 7 uses its validated redirected storage fields. Rechecked selection fields are logged; samples remain non-atomic.

The production/default capture does not read the historical CODE+0x24 scope/payload. Historical fixture assertions remain intact with explicit fixture opt-in, alongside a new default-disabled check. Slot-value REF_NAMESPACE payload metadata is read only on its own exact type match and does not prove Third_Person_Camera/current-control identity.

Build: Win32/x86 Release, MSVC 19.29.30159/v142/SDK 10.0.19041.0; **4/4 CTest passed**, 31 original plus 34 new R0 fixture assertions. DLL: `build/VelvetSwap-code-r0/Release/VelvetSwap.dll`; SHA-256 `bb7ba09144353843678008aebd57595276334256bea1827540eb92926aa9df28`. [Build evidence](EVIDENCE/2026-10-12_code_r0_build.json), [tests](EVIDENCE/2026-10-12_code_r0_tests.log).

RUNTIME-VERIFIED results are PENDING at this preparation checkpoint. One fresh-process manual gameplay capture has been requested: startup/F1/F2/F3 and 25–30 seconds ordinary gameplay. New log currently contains only REJECTED test hosts. No new live R0, equality, mode, slot value, or camera identity is claimed. v0.1 remains NO-GO; capture is required to classify stop A/B/C. No game-state writes, game-function calls, hooks, assets/executable edits, new REA analysis, commit, or push. Earlier uncommitted storage evidence and unrelated untracked files preserved.


## Slot 0x055f storage contract â€” 2026-10-12T14:19:20.313020+00:00 â€” STOP

CONFIRMED native getter: x86 thiscall, one uint32 index, EAX pointer result, RET4; receiver+0xcc selects storage. Own array begin/end are +0xc0/+0xc4, records stride 0x10, stored value pointer at record+0x0c. Mode 7 uses receiver+0x90; modes 4/6 fall back through receiver+0x2c when the local value is NULL. The control at receiver+0x9c -> +0x14 and record flag bit 2 can select a checked mirror-reference unwrap; never treat the direct formula as unconditional.

CONFIRMED operand edge: opcode 0x40 first selects CODE reference record 0 from CODE+0x18/+0x1c (stride 0x20), then FUN_00460540 masks member operand 0x055f with 0x7fff, unchanged as 1375, and calls that record's +0x18 receiver through virtual +0x98. Thus 0x055f is namespace-relative at this edge; it is not the initial CODE record index or a native byte displacement.

**UNRESOLVED / single blocker:** the relevant CODE reference record 0 +0x18 binding to the concrete namespace/state owning global 0x055f. Host context is compatible (historical 0x0354bd00, vtable 0x0081fc04, getter 0x00459940; static RTTI VMACHINE derives from NAMESPACE), but equality with that CODE record receiver is not established. Constructor/deconstructor checks did not supply a populated record binding. The prior NULL CODE+0x24 payload was not reused or recaptured.

**NO-GO for next live capture; STOP condition B.** RuntimeProbe unchanged; no build/tests applicable, no new runtime capture, game calls, writes, or hooks. Next bounded evidence step: resolve only the reference-record population/binding edge, then derive the complete guarded passive formula. No implementation before that mapping is proved.

Full branch formulas and instruction addresses: [storage contract](RUNTIME_SLOT_055F_STORAGE_CONTRACT.md). Immutable [result](EVIDENCE/2026-10-12_slot_055f_storage_result.json), [new native records](EVIDENCE/2026-10-12_slot_055f_storage_native.json), [retained records](EVIDENCE/2026-10-12_slot_055f_storage_retained.json), [local bytes](EVIDENCE/2026-10-12_slot_055f_storage_local.json). Historical evidence preserved.


## Bound scope live result â€” 2026-10-12T13:57:52Z, Codex â€” STOP

CONFIRMED: real replay.exe PID 31512, exact SHA-256 and loaded x86 PE identity VERIFIED, image base `0x00400000`; observer TID 18272 is not a game-thread identification. One manual loading session produced startup/F1/F2/F3 snapshots (counts 1/24/12/4), 172 passive scope records spanning 177.091 seconds, and VM depth/current-CODE variation. Incidental F4/F5 records remain observation-only; no SS or restoration work was performed.

CONFIRMED embedded objects: CODE `0x2769E77C` -> REF `0x2769E7A0`; CODE `0x27AEEE04` -> REF `0x27AEEE28`; CODE `0x36960B74` -> REF `0x36960B98`. Each listed CODE passed `0x007e0434`; each reference first word matched `0x007de8cc`. All 244 exact-type-gated scope records returned payload candidate `0x00000000` from `REF+0x18`. The other 6 scope records lacked an exact reference match and did not enable payload reads. Successful payload reads stayed null while binding addresses and ordinary VM samples changed; the null pointer was never followed. No readable/non-null payload or surrounding pointed-to type metadata exists in this capture.

CONFIRMED: the missing guarded read is fixed. DISPROVED for these samples: the first callback's CODE scope supplies a non-null bound namespace through this validated payload offset. The embedded REF_NAMESPACE marker interpretation itself is supported, not disproved. UNRESOLVED: why the reference is null, payload namespace identity, slot ownership/storage/live VM value, camera/current-control identity, and safe game/update-thread handoff. A stable null pointer is not camera proof.

STOP at the precise null-payload boundary: Task 3's concrete-namespace prerequisite was not met, so no slot access, accessor invocation, layout guess, or further REA analysis was attempted. This establishes neither a live camera nor a concrete namespace owning slot 0x055f. v0.1 is **NO-GO**. Single current acquisition blocker: a verified passive mapping from this null CODE scope to the actual namespace/state owning global slot `0x055f`. Thread handoff remains a separate required GO condition after acquisition; it is not claimed to be the only remaining v0.1 issue.

Exact next implementation step, after that missing mapping is supplied: extend guarded passive observation to the validated namespace/state receiver's indexed storage for slot 0x055f, then record the VM value and its verified reference type. The preceding evidence step is the existing `FUN_00459940` scope-mode/global-storage mapping for the actual receiver. Never call the accessor from the observer or substitute guessed offsets; do not resume this investigation automatically.

Immutable evidence: [live log](EVIDENCE/2026-10-12_bound_scope_live.log), [capture summary](EVIDENCE/2026-10-12_bound_scope_live.json), [critical result](EVIDENCE/2026-10-12_bound_scope_result.json). Capture SHA-256 `3056ff7a58bf982e7445e7842f475f55c194a97cf7290083da8b0a6cff71687e`. [Build record](EVIDENCE/2026-10-12_bound_scope_build.json) and [tests](EVIDENCE/2026-10-12_bound_scope_tests.log): Win32 Release, 3/3 passed; original assertions retained with new typed-read safety checks. Zero game-state writes, game-function calls, hooks, executable or asset edits. Samples are non-atomic worker observations, not callback-entry traces or lifecycle proof.

Repository: HEAD `973c385`, branch `runtime-outfit-swap`, dirty from this source/document/evidence pass plus pre-existing untracked files; no commit/push. Historical evidence preserved. The computer-use launch attempt was rejected with "Computer Use was not approved to use replay"; no alternate launch was attempted. The user's subsequently completed manual capture supersedes the earlier pending-capture blocker. The immutable [attempt record](EVIDENCE/2026-10-12_bound_scope_attempt.json) describes the earlier checkpoint only. Investigation STOPPED after this critical result and documentation.



## Bound namespace probe prepared â€” 2026-10-12, Codex

CONFIRMED: entry HEAD `973c385`, branch `runtime-outfit-swap`; checkpoints `536114e`, `56a18b8`, `973c385` present. Entry worktree already had untracked build, transcript, and spatial-analysis files; preserved. No commit or push.

CONFIRMED implementation: exact replay.exe identity gate and existing guarded reads retained. After exact rebased CODE-vtable validation, `binding+0x24` is recorded as the embedded object address; its first word is tested against rebased REF_NAMESPACE vtable `0x007de8cc`. Only a match enables guarded `embedded_ref+0x18` observation as `boundNamespacePayloadCandidate`. A guarded payload first-word read records readability only, without asserting namespace/camera type. `[BINDING_SCOPE]` separates these fields and records every one-second passive sample. Zero game-state writes, game function calls, or hooks.

CONFIRMED: new `build/VelvetSwap-bound-scope/Release/VelvetSwap.dll` is Win32/x86 Release; all three CTest cases pass, including 31 guarded-read/identity assertions. Existing tests retained. [Build evidence](EVIDENCE/2026-10-12_bound_scope_build.json), [test log](EVIDENCE/2026-10-12_bound_scope_tests.log). Old DLL/log/evidence untouched; historical first-live log hash still `c35e5516534355e868aff4e3f20cd9911f00f7c844076fb1e624db17e8c0ce48`.

UNRESOLVED / PENDING at this preparation checkpoint: one user-operated real-game capture using the proven x86 Cheat Engine manual procedure. New log contains only REJECTED isolated test sessions; no new live address or payload claimed. Current gating blocker is the missing new-contract live capture. Next step: enter controllable gameplay, load the new DLL, decline optional exported calls, capture startup/F1/F2/F3 plus 15â€“20 seconds of ordinary gameplay; freeze evidence, then use only verified passive namespace/slot mappings. Do not inspect slot ownership before that payload exists. Camera identity and update-thread handoff remain UNRESOLVED; v0.1 is NO-GO. This preparation record does not claim stop condition A, B, or C.


## First real v0.0 live capture â€” 2026-10-12T04:01:25Z (Codex)

CONFIRMED: VelvetSwap.dll v0.0 loaded in real `replay.exe` PID 3220. The log reports `identity=VERIFIED` for `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132` at base `0x00400000`, module size `0x00585E84`, PE timestamp `0x49EF137B`. The immutable capture has 53 snapshots, SHA-256 `c35e5516534355e868aff4e3f20cd9911f00f7c844076fb1e624db17e8c0ce48`: [live log](EVIDENCE/2026-10-12_v0.0_first_live.log), [parsed summary](EVIDENCE/2026-10-12_v0.0_first_live.json). Active append log: `build/VelvetSwap-v0.0/logs/VelvetSwap.log`.

CONFIRMED observed fields: HOST context `0x0357BD00`, context vtable `0x0081FC04`, accessor `0x00459940`; VM depth varied 0â€“8, callback head `0x26E1EE5C`, candidate owner `0x26E1EE48`; stored binding `0x279BF664`, CODE vtable `0x007E0434` (`matches_CODE=1`), raw `scope_word=0x007DE8CC`. Host/callback candidate values were stable in captured snapshots. F1/F2/F3 snapshot edges appear 13/7/7 times; do not treat that as exactly one keypress each.

LIKELY: binding candidate participates in VM execution because `current_code` sometimes equals `0x279BF664` at nonzero depth. The raw scope word matches the known REF_NAMESPACE vtable, consistent with an embedded reference. This is not the referenced namespace pointer and does not establish camera identity. CONFIRMED unresolved: REF_METADATA payload, slot `0x055f`, active camera/current-control identity, outfit state, and actual update-thread context. Pointer stability alone is not camera proof.

Next single blocker: identify the concrete namespace payload of the CODE-bound reference and determine whether it owns slot `0x055f`; the live capture did not read that payload. v0.1 outfit writes remain NO-GO. No hooks, game-function calls, game-state writes, game-file edits, or new REA analysis were performed.


## Runtime instrumentation v0.0 / live-load preparation â€” 2026-10-12T03:58:37.188543+00:00, Codex

CONFIRMED: observation-only VelvetSwap.dll built as Win32/x86 Release with MSVC 19.29.30159, v142, Windows SDK 10.0.19041.0. Three isolated CTest cases pass. DLL: `build/VelvetSwap-v0.0/Release/VelvetSwap.dll`; log: `build/VelvetSwap-v0.0/logs/VelvetSwap.log`. [Phase evidence](EVIDENCE/2026-10-11_v0.0_runtime_instrumentation.json) preserves DLL identity, validation logs and original REA evidence IDs.

CONFIRMED implementation: exact replay.exe disk SHA-256 and loaded x86 PE identity gate; rebasing; null/range/readable-page/SEH guarded passive host/context and VM/callback metadata reads; F1-F5 edge-triggered logging/snapshots while the game is foreground; one-second passive samples and one-minute heartbeats. No game function invocation, outfit writes or hooks. DLL pins itself until process exit; its own VelvetSwapStop export requests asynchronous observation stop. Do not manually unmap it during startup or observation.

LIKELY interpretation only: callback head minus 0x14 is an owner candidate, with stored binding at +0x34. CODE-vtable matches allow a raw +0x24 word read; this is not a camera identity. REF_NAMESPACE matches permit the documented +0x18 payload read; namespace payload is not automatically the current camera. Slot 0x055f, camera and native outfit fields log UNRESOLVED. Observer thread ID and passive changes do not identify the update thread or callback frequency.

CONFIRMED: Cheat Engine 7.7 is installed at `C:\Program Files\Cheat Engine`; `cheatengine-i386.exe` exists. No download/install, privilege change, game-file edit, REA call or game memory access was performed for the loader search. Superseded by the real-game capture documented below: the original common-tool check preceded manual loading and found no game process at that time.

Superseded: live loading and capture are complete. See the first real v0.0 live capture section below; the remaining namespace-payload blocker is stated there. v0.1 remains NO-GO. Prior static-investigation and no-source instructions below are historical and superseded by this expressly authorized observation-only build/load phase.

## Latest checkpoint â€” 2026-10-11 22:15:38 America/Toronto / 2026-10-12 02:15:38 UTC

1. CONFIRMED: matching replay.exe session/hash; branch runtime-outfit-swap; no game edits, code, build, or outfit test.
2. CONFIRMED static: Third_Person_Camera contains Player_Character member `ah`; wrapper name/body offsets 0x245d36/0x245d60.
3. LIKELY semantics: SetInitialOutfit(choice) writes ah normal choice and calls SetOutfit(NORMAL,false,true); runtime repeated-call safety untested.
4. DISPROVED: record 0x2e46ec is a global named `player`; it is local index 1 in DoWeaponSelection. CONFIRMED: its body reads global slot 0x055f; HandlePlayerSwitches conditionally writes that slot under a camera-type operand comparison.
5. SPECULATIVE: native acquisition through slot 0x055f. Its declaring namespace/name, concrete live value and lifetime are unresolved; Lookup("player") is not supported by the local declaration. See RUNTIME_CAMERA_ROUTE_PROOF.md.
6. FAILED / DISPROVED: FUN_005059e0 is a generic inspection path; it is not an outfit setter or current-player getter.
7. CONFIRMED generic CODE call boundary 0x00463ce0 -> 0x00461d40 mutates global VM state; thread safety remains unresolved.
8. FAILED live read: OpenProcess error 5; current camera/Player_Character pointer, safe native wrapper call, and full F1-F5 contract remain unresolved.

See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md) sections A-G for the preferred camera route, limitations, and exact resume boundary. ChangeInitialOutfit/StartChangeUniform are stock script candidates; no direct native outfit helper was verified. Investigation is STOPPED after documentation. Earlier checkpoints below are historical.

## Reconciled runtime DLL milestone â€” 2026-10-11 21:34 America/Toronto

See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md) for the authoritative eight-line milestone, identity checks, archived focused work, and next bounded task. Target: VelvetSwap.dll, x86/Win32, F1=Campaign, F2=Pilot, F3=Nightdress, F4=SS, F5=restore previous/original.

CONFIRMED: branch runtime-outfit-swap; clean worktree before this documentation pass; executable/script hashes match documented identities. REA/Ghidra binding was verified without analysis queries. The close request was rejected by client approval policy; session remains open.

UNRESOLVED immediate blocker: live Player_Character acquisition and a verified native-to-script SetOutfit invocation contract (method, typed arguments, ABI, lifetime, safe thread). SetOutfit's file offset is not a native address. LIKELY archived correction: three script arguments and Normal/Vision/Morphine/SS runtime modes; Campaign/Pilot use a separate normal-outfit field. Earlier native-signature milestone examples are unsupported.

Historical checkpoints below retain their original scope and timestamps. Fresh-level config success is not proof of runtime hot swapping. No game files or implementation code were changed in this resumption.

## Current milestone â€” successful in-game outfit test

Recorded: 2026-10-11 15:13:55 America/Toronto / 19:13:55 UTC. This is the documentation timestamp; the exact test time was not supplied.

CONFIRMED â€” user-reported in-game test for THE LOST Part 1:

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

## Latest milestone â€” 2026-10-11 15:00 America/Toronto

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

## Current executable verification â€” CONFIRMED

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
