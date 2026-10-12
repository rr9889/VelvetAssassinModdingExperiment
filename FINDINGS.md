# Confirmed

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


## Slot 0x055f storage contract — 2026-10-12T14:19:20.313020+00:00 — STOP

CONFIRMED native getter: x86 thiscall, one uint32 index, EAX pointer result, RET4; receiver+0xcc selects storage. Own array begin/end are +0xc0/+0xc4, records stride 0x10, stored value pointer at record+0x0c. Mode 7 uses receiver+0x90; modes 4/6 fall back through receiver+0x2c when the local value is NULL. The control at receiver+0x9c -> +0x14 and record flag bit 2 can select a checked mirror-reference unwrap; never treat the direct formula as unconditional.

CONFIRMED operand edge: opcode 0x40 first selects CODE reference record 0 from CODE+0x18/+0x1c (stride 0x20), then FUN_00460540 masks member operand 0x055f with 0x7fff, unchanged as 1375, and calls that record's +0x18 receiver through virtual +0x98. Thus 0x055f is namespace-relative at this edge; it is not the initial CODE record index or a native byte displacement.

Conditional native direct-return formula: `value_pointer = D(begin+0x55fc)`, not an inline VMValue address. It requires the correct receiver, valid bounds, and branch selection; the complete global-slot formula is not proved. Record flags are not value type tags.

**UNRESOLVED / single blocker:** the relevant CODE reference record 0 +0x18 binding to the concrete namespace/state owning global 0x055f. Host context is compatible (historical 0x0354bd00, vtable 0x0081fc04, getter 0x00459940; static RTTI VMACHINE derives from NAMESPACE), but equality with that CODE record receiver is not established. Constructor/deconstructor checks did not supply a populated record binding. The prior NULL CODE+0x24 payload was not reused or recaptured.

**NO-GO for next live capture; STOP condition B.** RuntimeProbe unchanged; no build/tests applicable, no new runtime capture, game calls, writes, or hooks. Next bounded evidence step: resolve only the reference-record population/binding edge, then derive the complete guarded passive formula. No implementation before that mapping is proved.

Full branch formulas and instruction addresses: [storage contract](RUNTIME_SLOT_055F_STORAGE_CONTRACT.md). Immutable [result](EVIDENCE/2026-10-12_slot_055f_storage_result.json), [new native records](EVIDENCE/2026-10-12_slot_055f_storage_native.json), [retained records](EVIDENCE/2026-10-12_slot_055f_storage_retained.json), [local bytes](EVIDENCE/2026-10-12_slot_055f_storage_local.json). Historical evidence preserved.


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

## Focused camera / outfit checkpoint — 2026-10-12 02:15:38 UTC, Codex

User requested the existing Third_Person_Camera wrapper as the preferred route, then documentation and STOP. These are static findings unless explicitly attributed otherwise. No game changes, source/build files, outfit calls, privilege escalation, or security modifications.

### CONFIRMED — script declarations and runtime constants

- Third_Person_Camera class ID 0x516; declaration name at script file offset 0x22df41. Member `ah` name at 0x23c5fa has declared class ID 0x4ef (Player_Character). A native C++ camera struct/layout has not been recovered.
- SetOutfit record name/body 0x20eeeb/0x20ef0b, 1436 bytes. Its three named script arguments are `iNewOutfit` (type 0x20), `hideequipment` (type 0x22), `bForce` (type 0x22); declaration names at 0x20ed50/0x20ed6a/0x20ed87. Native arguments are VM values, not three raw x86 integer arguments.
- Named constant records: OUTFIT_NORMAL at 0x1ee3a = float32 0; OUTFIT_VISION at 0x1ee57 = 1; OUTFIT_MORPHINE at 0x1ee74 = 2; OUTFIT_SS_UNIFORM at 0x1ee93 = 3. Separate INITIAL_OUTFIT_CAMPAIGN/PILOT/NIGHTDRESS names at 0x1eecf/0x1eef6/0x1ef1a encode 0/1/2. The recorded substring-search offsets 0x1eed7/0x1eefe/0x1ef22 point inside those INITIAL_ names, not record starts.
- Boolean declarations: `true` name 0x2c80, value 1; `false` name 0x2c94, value 0; first global records after count 0x879. Existing code uses global literal index 0=true and 1=false.
- Player_Character member table was decoded only through the required fields: normal choice 0x33, pending normal choice 0x32, last normal/SS choice 0x34, runtime mode 0x35, show-equipment flag 0x31. These are VM member indices, not native byte offsets. Data record starts: 0x20989a/0x209874/0x2098bd/0x2098e8/0x209856 respectively.
- Mesh field indices 0x18/0x19 Campaign upper/lower; 0x1a/0x1b Pilot; 0x1c/0x1d normal groups; 0x1e/0x1f vision groups; 0x22 SSUniform_Objects. player_exdata.cfg explicitly assigns Nightdress_Arm_R/Arm_L/Body/Head to VisionMode_UpperMeshes. The native mesh-list memory layout is not recovered.

### CONFIRMED observations / LIKELY script semantics — preferred wrapper

- Camera SetInitialOutfit name/body offsets 0x245d36/0x245d60, 143 bytes, one number `iNewInitialOutfit`. It uses member operand 0xaf, inline member operand 0x8048, normal-choice field 0x33, then SetOutfit operand 0x8046 with NORMAL (global 0x2f7), false (global 1), true (global 0), and a three-argument call encoding. Player_Character's own SetInitialOutfit body at 0x20f87d has the corresponding 108-byte operation.
- LIKELY semantics: `ah.m_iNormalModeOutfit = choice; ah.SetOutfit(NORMAL,false,true)`. The name-to-camera-member mapping 0xaf=ah is strongly corroborated by camera getters but not a completed full camera ordinal decoder. Underlying Player_Character SetOutfit member index 0x46 is supported by its own member record order and sibling calls; 0x8046 is the encoded VM operand.
- Camera GetInitialOutfit name/body 0x245ea8/0x245ed2, 23 bytes, reads operand chain 0xaf -> 0x33. Camera GetOutfit name/body 0x28d0ba/0x28d0dd, 46 bytes, reads the inline ah GetOutfit operation and runtime field 0x35.
- Camera ChangeInitialOutfit name/body 0x245fca/0x245ff7, 172 bytes, one number: writes ah fields 0x32 and 0x34, evaluates a player-global guard and requests a state/animation change. LIKELY stock runtime change path; precise completion call to SetInitialOutfit is NOT verified.
- Camera StartChangeUniform name/body 0x24619a/0x2461c6, 235 bytes, no arguments: guarded state/animation request. Not proved to force SS deterministically. Exact camera ::SetOutfit and ::ToggleUniform names were absent from the bounded name search. Player_Character::ToggleUniform body 0x20f9d5 is 289 bytes and contains normal-choice/mode restrictions; it is not an unconditional SS setter.
- No one-shot guard in the 143-byte immediate wrapper; it forces reapplication. Repeated in-game safety, valid scheduling, animation/state interaction and mesh availability remain untested. Normal choice 2 selecting Nightdress groups while mode remains NORMAL is LIKELY; passing SetOutfit mode 2 selects MORPHINE instead.

### Camera acquisition — precise static candidate, not live proof

- DISPROVED global-declaration interpretation: record 0x2e46ec is local index 1 named `player` in DoWeaponSelection, not a recovered global name. CONFIRMED local type 0x31/class 0x516 and body copy from unnamed global slot 0x055f. RUNTIME_CAMERA_ROUTE_PROOF.md supersedes the previous acquisition claim; the error remains in SESSION_LOG.md.
- CONFIRMED corrected HandlePlayerSwitches body at 0x33a9e7, 429 bytes: reads global operand 0x000a; gets member 0x87; compares Character method 0x803d result to global 0x0266; conditionally assigns that object to global 0x055f. Third_Person_Camera::GetCharacterType returns the same operand. LIKELY camera-cache semantics; slot name/namespace, concrete live identity and lifecycle clearing remain incomplete.
- Native host helper 0x004aafb0 returns 0x008ec408 (assembly Evidence `ev_320646b7eed27831e6ae670c00481cfec7645415abf836ea068901739c677633`). Host context is read through +4; native lookup helper 0x004c4030 uses context name-key slot +0xa8 then value slot +0x98 and returns the resulting VM value (`ev_bafd882e8920b81a13d116f63d32faa49c61007ed84719e20521ee184d423e1a`). It is an object lookup helper, not an outfit caller.
- Native reference-unwrapping evidence: checked REF<NAMESPACE> table 0x007de8cc slot +0x40 points to 0x004cdc00 (`ev_8e9e185a95c1b7e50c2f03251d553ab0fd493c95dbc0bef4ec7ee137edf729e0`). Assembly `MOV EAX,[ECX+0x18]; RET` confirms `Namespace* __thiscall Unwrap(RefNamespace*)` for that type (`ev_1121e79a4b3afc02238ea150293c75b6128f1008b55bb8f8ceec3744b9badb72`). Type/lifetime checks are essential before applying it to a fetched live value.
- DISPROVED premise for the former name-lookup candidate: local metadata does not establish a global named player. SPECULATIVE replacement: recover slot 0x055f's owning namespace/key, validate its value and unwrap only as required by its actual type. No verified GetCurrentPlayer/GetCurrentCamera routine or native ah offset exists; NULL/load behavior remains unobserved.

### FAILED / DISPROVED — earlier player/outfit candidates

- FUN_005059e0 receives an existing MirPtr in ECX, prints/debug-inspects it, runs update 0x004ff7b0 and tail-jumps through auxiliary object's vtable+8; no outfit arguments or player return. The auxiliary table 0x007e77bc resolves that slot to 0x004f57b0, which iterates field wrappers and calls their inspection slot. DISPROVED as an outfit method/path or current-player getter. Assembly IDs: `ev_d26c8c97a8c96b5a18e4327f4a5f63be1d2e25318da8bf861e3451dc4538826d`, `ev_94a36f08e99b4a9edf0644740b2e59ea39da8dd3e6db3624c3789fa8bcdb2487`, `ev_9a36c6ee0309923dfdfbd5f095c43bc07023c1ba8a306d1a614c4e3cafa9eb7d`, `ev_09c0cb513cd98c45f58b5e80b0fd02567f757bde074f07cb1b84686f91896a0a`. Update +0x44 caches the object/reference payload copied from bound value+0x18; it was not proved to be a generation counter.
- 0x004aaf80 -> 0x004c4010 returns address 0x008ec440 (=host+0x38), not the camera/player itself (`ev_aee6fdcc9d9e17fd616981cb6528e84451c70484c85f780a8d6d0b8d8c027351`, `ev_b2f8b920db962c5334f56e270ccb0f56e468d0f78e899bfd8e86f37169a88c90`). Do not infer its full object type from that address.
- 0x004f6a00 is an allocation/construction wrapper in the saved archive, not retrieval of the controlled instance. Actor LoadScene adapters operate script-to-native and are not the reverse outfit API.
- Bounded native string searches returned no defined strings for Third_Person_Camera, SetInitialOutfit, GetPlayer, SetOutfit: camera IDs `ev_273852dc08c180796914d45065f44ece82c839e9833ff94b6fc7410050883e7f`, `ev_8ddcdade2ea459383dc91ef815a05ef821246ca922844f950d9ffe3714c3154a`, `ev_6ba6fd6138aea71fcf1c8f097159d979cc1217fd67fd43cbafbdff619cc3ac84`, `ev_1bf6513aa51dd32a196d5008d463fddaaefc42ab6450cb1339fbacae3a539413`. This does not prove absence of indirect native helpers. The saved Character binding names include no outfit setter (`ev_6f9af598d05e7fcaf403011568958e995b2267386c59bd92831163eff5233bbb`).

### CONFIRMED generic boundary — background only, no further VM expansion

- Filesystem RTTI pointer relationships identified CODE@mtl, type descriptor 0x008b7920, locator 0x00830d3c, vtable 0x007e0434. Slot +0xb4 / 0x007e04e8 is 0x00463ce0; REA byte check `ev_cf1790d3209bdb40681f764086560dafc3d501e49b6d5a9279d3521f8ab8ad9c`, and surrounding table check `ev_b364c2b1fc7ad57937089910184665e9785631af98a89ba786a9b98ea941c642` corroborate the pointer. Scalar type tables were also observed: NUMERIC<float> vtable 0x007dc5c4; BOOL vtable 0x007de53c. Native scalar constructors/payload layout/lifetime are NOT established by those names.
- Native instruction records for 0x00463ce0..0x00463d5c show CODE* in ECX; invocation context is one stack pointer; RET 4. At 0x00463d17..0x00463d1e it passes that context and CODE+0x24 to executor 0x00461d40. CODE+0x24 is LIKELY its embedded execution-scope reference, not a bytecode file offset. Key per-instruction IDs and all other relevant IDs are in the evidence index; raw byte check `ev_a5597d352d6099b02abbae25af140c9c26669752627919e081c4869f08bb6e10`.
- Wrapper checks context+4; when zero, pushes CODE's +0x3c payload and CODE* onto the global stack at 0x008e9b90, using 0x008e9b94/0x008e9b98; pops them after the call. Executor additionally writes 0x008e9b88. CONFIRMED shared mutable state; thread safety, serialization, correct thread ID, and safe reentry remain UNRESOLVED.
- Executor 0x00461d40 assembly `ev_784062c529976230a5a80893f19110a7b9c8455f120166f5ade005cb5749ce80` and labeled Ghidra pseudocode `ev_2161fa8f71b4936edcc372fc993ed4359cb1b76eaa82a99b5fe9b9a9681dd011`: ECX is CODE*, two stack pointers, RET 8. Inspected call setup reads argument count at context+0x28 and an array of 32-bit VM-value pointers at context+8. Value-return handling writes a VM-value pointer to context+0x2c when byte0 permits return capture; byte1 selects borrowed versus copied value handling. SetOutfit's bytecode returns void; EAX is not a proved outfit result. Full context semantics/initialization remain unverified.
- Existing name-key helpers: 0x00413730 performs member-name search through virtual count/name slots and returns an index or 0xffffffff; 0x00414230 wraps it and throws when missing. Native name parameters are the game's narrow string objects. Namespace value getter 0x00459940 has scope-mode branches and bounds checks (`ev_ecd780849417c19e756ae48c3f7f3e900916e225843d579b5ef578baf175d6e5`, assembly `ev_4f1f3c80f5bf9536de1330ebc4455f6fc6749844a600909f8ce20869c4e9f4dd`). Hardcoded VM indices do not remove context/type validation.

### FAILED observations, completeness, and next boundary

Read-only OpenProcess(0x1010) for already-running PID 33896 failed with Windows error 5. No executable path or memory was obtained through that handle. No escalation, security changes, debugger attachment, or outfit invocation followed. Live camera/Player_Character validation remains unresolved.

Other failed calls/parser diagnostics and bounded abandoned branches are logged in SESSION_LOG.md. [Evidence index](EVIDENCE/2026-10-11_focused_player_setoutfit_index.md) lists every retained relevant Evidence ID; [session JSON](EVIDENCE/2026-10-11_focused_player_setoutfit_session.json) preserves 232 earlier unique inline records plus the camera follow-up and scoped local outputs. They are custom preservation files, not a fresh REA export. Original transcript remains the source for outputs not retained in persistent bindings.

Exact resume point: enumerate unnamed slot 0x055f references, examine the named camera functions/direct callers and established callback phase. Do not use Lookup("player") based on local metadata. Do not restart FUN_005059e0, generic scene loading, input discovery, or broad VM investigation. No native DLL contract is marked runtime-safe.

## Runtime DLL reconciliation — 2026-10-12 01:34 UTC, Codex

See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md). CONFIRMED: current filesystem executable/script hashes match the documented identities; REA opened replay.exe as PE/x86 with Ghidra 12.1.4 and the same hash. No fresh function analysis was performed.

LIKELY, preserved archived report (ARCHIVE/copypaste of last session.txt lines 636-638): SetOutfit has a number and two boolean arguments; its runtime modes are Normal=0, Vision=1, Morphine=2, SS=3. Campaign/Pilot use a separate normal-outfit field; the mode also affects IsCamouflaged. The raw supporting script decoding is not present in this excerpt, so these are not promoted to runtime-confirmed facts. Do not conflate initial Outfit=0/1/2 with runtime modes.

UNRESOLVED: a verified live Player_Character pointer, method resolution, argument/ABI contract, and game-thread invocation point. Saved MirPtr update/call candidates and Evidence IDs are indexed in the checkpoint; registration and generic LoadScene adapters are not a proved SetOutfit API. Earlier unsupported native-signature examples in planning/template files are superseded.

## Successful in-game test — THE LOST Part 1

Evidence source: the user's explicit report of the successful in-game test. Recorded 2026-10-11 15:13:55 America/Toronto / 19:13:55 UTC; exact execution time was not supplied. This was not a runtime test executed or independently observed by the assistant.

Configuration: `aio\levelstructure.aio`, `[level1]`.

```ini
ID = TL_1
SceneName = Levels\\Lost1.scene
```

| Setting | Observed in-game result | Confidence |
|---|---|---|
| Original Outfit = 1 | Pilot Violette | CONFIRMED |
| Modified Outfit = 0 | Campaign Violette | CONFIRMED |

CONFIRMED: the level was started fresh and the Campaign outfit appeared successfully.

CONFIRMED: `aio\levelstructure.aio` directly controls Violette's outfit selection for THE LOST Part 1 at level load. No `replay.exe` patch was required.

Scope: this verifies the tested level and the Pilot-to-Campaign change. It does not establish checkpoint restoration behavior, all other levels/outfits, or every reconstructed bytecode symbol and native dispatch edge. Those retain their prior confidence labels.

The proposed Outfit 1 -> 0 test below has now been successfully performed by the user. Earlier 'NOT performed' statements are historical. Restoration of the original configuration was not reported and is not assumed.

## Upstream investigation checkpoint — 2026-10-11

This checkpoint supersedes the earlier statement that no upstream model-selection candidate was identified. It does not claim runtime execution was observed.

### Complete observed direct callers and entry references

| Target | Resolved direct callers / call instructions | Additional function-pointer references |
|---|---|---|
| FUN_0060d970 | FUN_0060daa0 at 0x0060dabc; FUN_0060dad0 at 0x0060daec; FUN_0060db00 at 0x0060db1c | None returned by entry xrefs |
| FUN_0060daa0 | FUN_0060fe90 at 0x0060ffa8 | FUN_006140e0 at 0x00614195 |
| FUN_0060dad0 | None resolved | FUN_006140e0 at 0x006141a4 |
| FUN_0060db00 | None resolved | FUN_006140e0 at 0x006141b3 |

CONFIRMED: the registration references are pointer storage/registration, not direct execution calls. Static direct-call enumeration cannot enumerate all runtime script calls. No resolved caller must not be interpreted as an unused wrapper.

REA caller/xref Evidence IDs:

- FUN_0060d970: `ev_583becb18dcbb5d2a218d43c2e0804fea6b93eaa2110165a4235e5eb178c51d3`; `ev_11c70ce693aa531eefb2d379e163e6bcd8f2c331df844d1eb605413ed09d7f42`.
- FUN_0060daa0: `ev_25061d58d2155e8085510da48b01a0f887406d67f1290c9251e760cbb780bd33`; `ev_96f4c78c2f57ea3ec2ab5a7a27bef64a0423f42eeb6ab4637c39c4e385820642`.
- FUN_0060dad0: `ev_5162c08e2af389f926e93ccd50f3a2eb54ec16b0f3adc2067a7584537ebced58`; `ev_dfd3ad3425845060377336fe7d44406f61e0faf3511f04b67abfa6ff28e15a6f`.
- FUN_0060db00: `ev_5064c2af9ab381298c2220d5d89a5b609afc59e012f2befdf259a32c537cc99c`; `ev_3fcc2cab4d2fbc2ccde193b1d834d7ad38cef8988fd6a1f26adfc3879a9f6c58`.

### Native scene-identifier argument flow

CONFIRMED from assembly: the wrappers do not choose a model path. At FUN_0060daa0 entry, the third stack argument (`[ESP+0x0c]`, before its first PUSH) is the script/value object placed into ECX by the wrapper. The first two stack arguments are the result storage and target actor. FUN_0060d970 copies/converts the textual storage at incoming ECX+0x14 using FUN_00411440, then calls FUN_0061c160 with the target actor in ECX and the converted identifier as its first stack argument. The NoCheck/ForceReload wrappers change flags, not the identifier source.

Assembly Evidence: `ev_e92b48c91f0562b05b12e3ddcd6f3ab3718455b4fcca704614e3db41a460efa9` and `ev_4d7dd8754a5f5d8b2dccac0366a9d7249f47f44692df913eb04b4d1ef573b4d0`.

CONFIRMED: FUN_0060fe90 receives a narrow string object in ECX and actor in ESI. It searches the string for `.scene`, converts it to a wide string (FUN_00411540), wraps it in a value object (FUN_00414470), and calls FUN_0060daa0 at 0x0060ffa8. Its callers FUN_00610040 (LoadResource) and FUN_006113d0 (CreateChild) derive this text from their supplied value argument through FUN_004295c0 and FUN_00411440; they do not contain a Violette filename literal or outfit-selection branch. Assembly Evidence: `ev_124abf9b253d189385bd86cbc8a465f83aa3a5f85b146d81094d2a111da10c24`, `ev_5da49d0c0b2f23314bcbcd1d9d533e7ee285d5879a6fc986ad589d3bf64aa1d5`, and `ev_f6c731c8aab82c72994da1e83c8bffdb4d64dd999781f7d3a6c15ce139e92925`.

### Indirect script invocation boundary

CONFIRMED: FUN_00612ab0 allocates a 0x30-byte adapter, stores the registered wrapper pointer at adapter+4, installs vtable 0x007f3b18, and registers it through FUN_006124e0. That vtable's first entry is FUN_00609f60; it selects FUN_00606840 for the non-static adapter path. FUN_00606840 calls the stored pointer and passes argument values from the invocation argument array, beginning at argument-array+8. This explains why script calls do not appear as ordinary direct native callers.

Evidence: registration decompilation `ev_5ea0bfde61cb7c95a83b7cc2312073f931a751ef93cc204a3555905a62636933`; adapter `ev_65117c565c887f6f51efb330afdb761cae461fd20952071629660ab31e54d891`; indirect invocation `ev_fbf596f9281fec20e585591a2b7c85ccae56516831770946fd867cbe1798d9b8`.

### External compiled script identity and model constants

CONFIRMED by read-only filesystem bytes, not a Ghidra script decompilation:

- Path: `C:\Users\Admin\Documents\Velvet Assassin\src\live3d.mir.bin`.
- Size: 3,459,973 bytes.
- SHA-256: `404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099`.
- It contains length-prefixed function names and bytecode bodies. Printable-string extraction can append a printable byte from the following length field; exact names below were checked using their stored lengths. For example, the actual name is Player_Character::Init, NOT InitB.

All offsets in the following table are FILE OFFSETS in this script, not native virtual addresses:

| Constant | Name offset | Exact UTF-16LE path | Path offset |
|---|---|---|---|
| FILENAME_VIOLETTE_ALL_MESHES | 0x0005f5ba | Models/Characters/Violette_All_Meshes.scene | 0x0005f5e2 |
| FILENAME_VIOLETTE_PILOT | 0x0005f63c | Models/Characters/Violette_Pilot.scene | 0x0005f65f |
| FILENAME_VIOLETTE_PILOT_SS | 0x0005f6af | Models/Characters/Violette_Pilot_SS.scene | 0x0005f6d5 |
| FILENAME_VIOLETTE_CAMPAIGN | 0x0005f72b | Models/Characters/Violette_Campaign.scene | 0x0005f751 |
| FILENAME_VIOLETTE_NIGHTDRESS | 0x0005f7a7 | Models/Characters/Violette_Nightdress.scene | 0x0005f7cf |

The corresponding loose model assets exist in Models\Characters. Their existing backup directory was observed but neither created nor changed; this investigation does not establish historical asset originality.

### Initial outfit configuration

CONFIRMED: `aio\levelstructure.aio` lines 3-4 explicitly document:

- Outfit=0: campaign.
- Outfit=1: pilot.
- Outfit=2: nightdress only.
- LoadSSUniform=1 loads/supports the SS uniform and is supported with Outfit=1.

CONFIRMED: `[level1]` uses `Levels\\Lost1.scene` (line 9), Outfit=1 (line 20), LoadSSUniform=0 (line 21). `[level5]` uses Outfit=0 (line 120); `[level11]` uses Outfit=2 (line 270). These are current file contents, not proof of runtime use.

CONFIRMED: `aio\player_exdata.cfg` [OBJECTS] defines SSUniformObjects (line 60), Campaign_UpperMeshes (line 63), Pilot_UpperMeshes (line 65), and VisionMode_UpperMeshes (line 67). The nightdress group's mesh names are Nightdress_Arm_R, Nightdress_Arm_L, Nightdress_Body, and Nightdress_Head.

CONFIRMED: `Levels\Lost1.scene` contains `.\models\characters\violette.scene` at file offset 0x000084ac. This placeholder reference is distinct from the variant filenames. The owning scene property/node was not decoded.

CONFIRMED: relevant compiled-script records exist:

| Function | Name file offset | Bytecode file offset | Bytecode bytes |
|---|---|---|---|
| Include_Scene::LoadIncludedScene | 0x00060605 | 0x0006062a | 423 |
| Include_Scene::LoadIncludedScene2 | 0x00061a90 | 0x00061ab6 | 405 |
| LeveldataManager::getOutfit | 0x0007dc2c | 0x0007dc4c | 225 |
| Player_Character::Init | 0x002099c9 | 0x002099e4 | 66 |
| Player_Character::SetOutfit | 0x0020eeeb | 0x0020ef0b | 1436 |
| Player_Character::SetInitialOutfit | 0x0020f856 | 0x0020f87d | 108 |
| Third_Person_Camera::SetInitialOutfit | 0x00245d36 | 0x00245d60 | 143 |

The compiler source labels include `src\behaviour\a_include_scene.mir`, `src\core\leveldata.mir`, and `src\behaviour2\player_character.mir`. The loose original .mir source files were not found in the observed Script/src inventory.

All addresses below are previously observed Ghidra virtual addresses for the recorded executable. Function names are Ghidra-generated names, not recovered source symbols. Simplified pseudocode is explanatory, not an exact REA transcript.

## Best current candidate

`FUN_0061c160` at `0x0061c160` consumes a scene identifier and loads/instantiates that scene. Its resolved direct caller is `FUN_0060d970`.

Observed chain:

`LoadScene` -> `FUN_0060daa0` -> `FUN_0060d970` -> `FUN_0061c160` -> `FUN_00502020`.

`FUN_0061c160` rejects actors with existing children, optionally checks for recursive scene references, requests a resource through `FUN_00502020(scene_identifier, 3)`, and on resource state 1 calls actor hierarchy construction and initialization routines. Important callees include `FUN_0061a750`, `FUN_0061aef0`, and `FUN_00530aa0`.

`FUN_0061a750` attaches the resource, copies node data/transforms, creates children, and recurses. `FUN_0061aef0` processes scene entity metadata and initializes children recursively. These are generic scene/actor operations; a Violette-specific connection is unproven.

## GetMeshLibID

- Exact string: `GetMeshLibID`
- String/address: `0x007f437c`
- Function: `FUN_006140e0` at `0x006140e0`
- Reference location: `0x006145a6`
- Role: registers mesh-library identifier accessor, `FUN_0060f900`.
- Confidence: CONFIRMED.

## SceneResource

- Exact string: `SceneResource`
- String/address: `0x007eabec`
- Functions/references: `FUN_00537bf0` at `0x00537c14`; `FUN_0053c320` at `0x0053c343`.
- Data pointer: `0x008c630c`.
- Role: resource type construction / diagnostics.
- Confidence: CONFIRMED.

`FUN_00502020` looks up or constructs a resource via `FUN_0053bc40`; that constructor calls `FUN_00537bf0`, which uses the SceneResource type string. `FUN_0053c320` reports resource diagnostics.

## Script-visible scene entry points

All following string references are in `FUN_006140e0`:

| Exact string | String address | Reference | Registered implementation |
|---|---|---|---|
| LoadScene | 0x007f4798 | 0x0061419a | FUN_0060daa0 |
| LoadSceneNoCheck | 0x007f4784 | 0x006141a9 | FUN_0060dad0 |
| LoadSceneForceReload | 0x007f476c | 0x006141b8 | FUN_0060db00 |
| LoadResource | 0x007f475c | 0x006141c7 | FUN_00610040 |
| CreateChild | 0x007f4750 | 0x006141d6 | FUN_006113d0 |
| CreateChilds | 0x007f47fc | 0x0061414c | FUN_00610ea0 |

The three LoadScene wrappers call `FUN_0060d970` with flag pairs `(0,0)`, `(1,0)`, and `(1,1)` respectively. `FUN_0060d970` converts the supplied identifier and calls `FUN_0061c160`.

`FUN_00610040` and `FUN_006113d0` call `FUN_0060fe90`, whose `.scene` branch reaches `FUN_0060daa0`. CreateChild constructs a generic actor; player-specific creation remains unidentified.

## Requested string searches

Searches found no defined-string matches for `Violette_Campaign`, `Violette_Pilot`, `Violette_Pilot_SS`, `Violette_Nightdress`, `Violette_New_Basemesh`, or `Models`.

`Violette` matched `Textures/GUI/LoadingScreens/violette` at `0x007f5a74`, reference `0x00638a79` in `FUN_00638a50`. Decompilation constructs `Textures/GUI/LoadingScreens/violette` + decimal index 1-7 + `_i.dds`, then calls `FUN_00501ea0(..., 3)`. This is loading-screen texture path construction/resource acquisition, not evidence of player-model selection. Direct caller query returned none.

`.scene` at `0x007e3a0c` has references:

- `0x004ae0bc` in `FUN_004ade60`: extension search/removal.
- `0x0052e5eb` and `0x0052e606` in `FUN_0052e5b0`: extension search/removal.
- `0x0060ff50` in `FUN_0060fe90`: scene-loader dispatch.

Other exact matches: `Scene data stored in .scene file:\n ` at `0x007ea40c`, and `Aillegal .scene file.` at `0x008278f3`; no analyzed xrefs observed.

`Characters` matched only `ERROR_NO_WILDCARD_CHARACTERS` at `0x0080fdf4`, excluded as unrelated. `no weapon in basemesh` at `0x0082808c` references `0x00752100` in `FUN_00751c70`; decompilation led to enemy/combat logic and investigation of that branch stopped.

## Loader diagnostics

- `Actor already has children. Currently you can load scenes into end nodes.\n` at `0x007f4f08`, reference `0x0061c189` in `FUN_0061c160`.
- ` is already present in the parent tree. Inifinte loop prevented.` at `0x007f4e90`, reference `0x0061c2ad` in `FUN_0061c160`.

# Likely

## Highest-confidence upstream identifier selector — criterion B

BEST UPSTREAM CANDIDATE: `Include_Scene::LoadIncludedScene` in `src\live3d.mir.bin`, bytecode file offset 0x0006062a. This supersedes FUN_0061c160 as the best selection candidate, while preserving that native function's confirmed generic loader role.

LIKELY reconstructed behavior from the 423-byte script body, nearby declaration records, and configuration semantics:

```text
if the actor matches the special player-include condition:
    if level index is valid:
        initialOutfit = leveldata.getOutfit(levelIndex)
        loadSSUniform = leveldata's SS-uniform setting
        if initialOutfit == CAMPAIGN:
            selectedIdentifier = FILENAME_VIOLETTE_CAMPAIGN
        else if initialOutfit == PILOT:
            selectedIdentifier = loadSSUniform
                ? FILENAME_VIOLETTE_PILOT_SS
                : FILENAME_VIOLETTE_PILOT
        else:
            selectedIdentifier = FILENAME_VIOLETTE_NIGHTDRESS
    else:
        selectedIdentifier = FILENAME_VIOLETTE_ALL_MESHES
LoadIncludedScene2(actor)
```

This is analyst reconstruction, NOT original source or a REA decompilation. The function's local records explicitly name `self`, `iInitialOutfit`, and `bLoadSSUniform`. Its branch bodies assign member slot 0x20 from global slots 0x05b1, 0x05b0/0x05af, 0x05b2, or 0x05ae. It compares initial-outfit values against slots 0x02fc and 0x02fd. Mapping those symbol slots to the adjacent named global constants remains LIKELY until a complete symbol-table decoder or runtime observation corroborates it. The normal invalid-index fallback interpretation is likewise LIKELY.

LIKELY: Include_Scene::LoadIncludedScene2 forwards member slot 0x20 (the selected filename/identifier) into actor method slot 0x800a. Registration order and the native adapter evidence support identifying this as LoadScene. The exact bytecode-to-native runtime dispatch has not been observed.

LIKELY shortest selection-to-loader chain:

`levelstructure.aio Outfit/LoadSSUniform -> LeveldataManager settings -> Include_Scene::LoadIncludedScene -> selected member slot 0x20 -> Include_Scene::LoadIncludedScene2 -> Actor.LoadScene/FUN_0060daa0 -> FUN_0060d970 -> FUN_0061c160`.

LIKELY: Player_Character::SetOutfit controls visible mesh groups after resources are available, rather than choosing the initial scene filename. Its bytecode accesses Campaign/Pilot/NormalMode/Vision/SS group fields and the outfit state. This is a separate candidate for appearance changes, not the best scene-identifier selector.

## Proposed smallest reversible test — NOT performed

After explicit authorization and a verified original backup/hash, change only `[level1] Outfit` in `aio\levelstructure.aio` from 1 to 0; keep LoadSSUniform=0. Start level1 fresh rather than restoring a checkpoint, observe Pilot vs Campaign appearance, then restore the original file exactly. LIKELY expected result: Campaign model selection. No executable patch or model-file replacement is required for this proposed test.

An optional first observational check is to inspect the actual string/value argument entering FUN_0060d970 while creating Violette, together with the actor/call stack. No runtime test or debugger action was performed in this investigation.

## PlayerRoot

String `PlayerRoot`, address `0x007e724c`; function `FUN_004f0310` at `0x004f0310`; reference `0x004f042d`. Role: script/reflection field registration. Confidence: LIKELY.

## Character

String `Character`, address `0x007e766c`; function `FUN_004f12c0` at `0x004f12c0`; reference `0x004f12fd`. Role: character interface registration. Confidence: LIKELY.

## GetCharacterType

String `GetCharacterType`, address `0x007e7604`; function `FUN_004f12c0`; reference `0x004f1560`. Role: character method registration. Confidence: LIKELY.

## m_pCharacter

String `m_pCharacter`, address `0x007e7818`; functions/references: `FUN_004f8e40` at `0x004f8e99`, and `FUN_004fa1a0` at `0x004fa277`. Function entries: `0x004f8e40`, `0x004fa1a0`. Role: character-reference registration. Confidence: LIKELY.

## ActivePlayer

String `ActivePlayer`, address `0x007e7008`; function `FUN_004ef720` at `0x004ef720`; reference `0x004efdc4`. Role: active-player field registration. Confidence: LIKELY.

## Candidate interpretation

`FUN_0061c160` is a relevant loading point if Violette character models are scene resources. The `.scene` processing in `FUN_004ade60` and `FUN_0052e5b0` likely prepares resource/preload identifiers.

Observed callers: `FUN_004c7b50` -> `FUN_004ade60`; `FUN_004bb1b0` -> `FUN_0052e5b0`; `FUN_00500090` -> `FUN_004f12c0`. No direct callers were returned for the other listed registration routines; indirect dispatch may explain this.

# Speculative

The actor-property comparison activating the special replacement branch is not fully decoded. Its relationship to the Lost1 `violette.scene` placeholder is SPECULATIVE. Dynamic script dispatch and symbol slots are not fully validated, so exact live execution of the reconstructed selection chain remains unconfirmed. No evidence establishes that changing only an asset filename or patching FUN_0061c160 is the smallest correct modification.

Earlier speculation that selection originates upstream is now narrowed to the compiled Include_Scene selector and level outfit configuration. The earlier paragraph below is retained as historical context.

The actual model/outfit choice occurs before `FUN_0061c160`, probably from script, scene, or resource configuration. No observed branch selects Campaign, Pilot, Pilot_SS, Nightdress, or New_Basemesh.

No function has yet been confirmed to create Violette specifically or initialize her selected model. Generic scene-loading and actor-creation routines must not be labeled Violette-specific without further evidence.

# Open Questions

Latest checkpoint: stop criterion B has been reached with a high-confidence LIKELY upstream selector; criterion A (fully verified live Violette identifier origin) has not been claimed. Remaining focused questions are exact symbol-slot mappings, the special actor predicate, configuration loading/override behavior, and runtime confirmation of the chosen path. Do not continue until requested.

- Where does the scene-identifier argument originate?
- Is it a path, resource ID, script value, scene property, or resource-table entry?
- Which actor corresponds to Violette, and what connects it to PlayerRoot/ActivePlayer?
- Are the outfit/model names present in external resources or undefined executable bytes?
- Which upstream operation selects the outfit?
- What is the smallest reversible test once that selection point is established?

Limitations: searches cover Ghidra-defined string data, not all artifact bytes. Static xrefs and direct caller lists may omit indirect calls. Decompilation is provider-generated pseudocode; type propagation and removed-block warnings were observed, especially in `FUN_0052e5b0` and registration code. Verify arguments and addresses with read-only assembly where needed. No runtime experiment has established Violette-specific behavior.
