# Global operand 0x055f storage contract — 2026-10-12

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


Result: **NO-GO for another live capture. STOP condition B.** The accessor and operand decoder are established; the CODE reference-record binding to the owning namespace remains unproved. RuntimeProbe was not extended. No build, tests, game-function calls, game-state writes, hooks, or new runtime capture were performed in this pass.

Repository entry: `runtime-outfit-swap`, HEAD `0ade08b`; tracked files initially clean, pre-existing untracked build/spatial files preserved. Native target: replay.exe SHA-256 `9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132`, PE/x86, preferred base `0x00400000`. Script SHA-256 `404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099`. These are static addresses and historical live observations, not new live pointer observations.

## 1. Exact FUN_00459940 contract

**CONFIRMED calling convention:** x86 `__thiscall`, receiver in ECX, one 32-bit unsigned index on the stack, pointer-sized result in EAX, callee removes four argument bytes (`RET 4`). Evidence: retained assembly `ev_4f1f3c80f5bf9536de1330ebc4455f6fc6749844a600909f8ce20869c4e9f4dd`, entry `0x00459940`, argument load `0x0045994d`, returns through `0x00459a81`. Pseudocode `ev_ecd780849417c19e756ae48c3f7f3e900916e225843d579b5ef578baf175d6e5` agrees.

The function is an indexed NAMESPACE-family accessor, inherited in the `mtl::VMACHINE` table `0x0081fc04` at virtual offset `+0x98`. Static MSVC RTTI identifies VMACHINE and NAMESPACE in its hierarchy. This establishes a compatible host-context type, **not** equality between host context and the interpreter's global receiver. The native getter does not transform the index or search names. Its result can be an original stored pointer, a fallback result, or an unwrapped mirror reference; `VMValue*` must not imply that every branch returns the original wrapper.

Define `D(a)` as a 32-bit native memory load, `U(a)` as a byte load, `R` as the receiver, and `k` as the argument. The aliases below describe instructions; they are not recovered field names.

| Field/expression | CONFIRMED instruction evidence |
|---|---|
| `mode = D(R+0xcc)` | `0x00459943`; compares against 4, 6, 7 via SUB/JZ/JNZ through `0x0045995b` |
| `begin = D(R+0xc0)` | `0x00459993`, `0x004599b1`, `0x00459a53`, `0x00459a71` |
| `end = D(R+0xc4)` | `0x0045999d`, `0x00459a5d`; subtraction then arithmetic shift by 4 |
| `record = begin + k*0x10` | SHL by 4 at `0x004599b9`, `0x00459a09`, `0x00459a77` |
| `stored = D(record+0x0c)` | `0x004599bc`, `0x00459a7a` |
| `record_flags = U(record+4)`; bit mask 2 | `0x00459a0c` |
| `fallback_receiver = D(R+0x2c)` | `0x004599c3`; indirect call through its vtable `+0x98` at `0x004599cf` |
| `Q = D(R+0x90)` in mode 7 | `0x0045995d`; then Q storage at `+0xc0/+0xc4`, value at record `+0x0c` |
| `P = D(R+0x9c)`; `D(P+0x14)` controls the special branch | `0x004599d6`, `0x004599dc` |

**Bounds:** the native code requires nonzero begin and unsigned `k < uint32(SAR32(end-begin,4))`; otherwise it calls `0x0064f6b5`. It does not store a separate count field. For a passive reader, additionally reject reversed or unaligned bounds, address overflow, and unreadable/null pointers; require `(end-begin)%0x10 == 0` and `k < (end-begin)/0x10`. These extra validations are observer safety checks, not claimed native instructions.

Every storage-selection branch:

1. **Mode 7:** set `Q=D(R+0x90)`, validate Q's array, return `D(D(Q+0xc0)+k*0x10+0x0c)`. It bypasses R's special-control and record-flags checks. No null check on Q itself appears in this path.
2. **Mode 4 or 6:** validate R's array. If `D(begin+k*0x10+0x0c)==0`, call `fallback_receiver->vtable[+0x98](k)` and return that result. The same k is passed unchanged. The fallback's concrete type/implementation is not established here. If the stored value is nonzero, continue into the common path below.
3. **All other modes, and nonzero local values in modes 4/6:** load `P=D(R+0x9c)`. If `D(P+0x14)==0`, return the stored array pointer after bounds validation. If that control word is nonzero, validate R's array and inspect record flag bit 2. A set bit bypasses the special conversion and returns the stored pointer.
4. **Control nonzero and flag bit 2 clear:** pass the stored pointer to `0x00651941` with source type descriptor `0x008b00bc` (MIRRORCLASS), destination `0x008b8c20` (`REF<MIRRORCLASS,NO_CHECK>`), zero displacement/flag arguments. The native push/call sequence is `0x00459a13..0x00459a35`; the descriptor names are preserved from static executable bytes. If that checked-cast result is nonzero, reload the stored pointer and call its virtual `+0x40` (`0x00459a39..0x00459a50`); return that virtual result. If the check fails, return the original stored pointer.

The reloads use `FUN_00459730` on `R+0xbc`. Its assembly independently validates a begin/end pair at that subobject's `+4/+8` and returns `begin+k*0x10` (`ev_74855cf8ff6e7070467554f93cd54e533af39d3447484235b257cc0857ac0f7a`). This confirms the record formula; it is not another global lookup.

**Null/failure behavior:** no general null-receiver guard and no uniform “return NULL on failure” exist. A direct stored pointer or the virtual unwrap result can be NULL; modes 4/6 redirect a NULL stored pointer. Invalid bounds invoke `0x0064f6b5`; that helper calls `0x0064f691` with five zero arguments and has a RET (`ev_17b1a82a7d4c5c0529e128f14984da66bff91d54edc17d451934e466ceef9ae7`). Getter code continues to dereference if the failure helper returns. Failure-handler termination/exception policy was not investigated or assumed. Null `P`, Q, or fallback receiver can therefore fault in native code. A passive observer must fail closed rather than reproduce those calls.

The mode 7 redirect and mode 4/6 virtual fallback are the only receiver redirections visible in this accessor. **UNRESOLVED:** their source-level names, which is a parent/global namespace in the relevant execution, and the relevant live mode. No names were invented for those fields.

## 2. Smallest trace of operand 0x055f

The first assignment in DoWeaponSelection, body file offset `0x002e48ba`, contains:

```text
42 01 00 00 00 ff ff ff ff
40 00 00 00 00 5f 05 00 00 ff ff ff ff
0c
```

The opcode 0x40 expression starts at file offset `0x002e48c3`. Its initial operand is 0, followed by member operand `0x0000055f`, followed by terminator `0xffffffff`. The prior local `player` declaration still does not establish a global name.

**CONFIRMED native opcode selection:** interpreter `0x00461d40` uses byte table `0x00463c70[opcode-3]` then pointer table `0x00463ba0[case*4]`. For opcode 0x40, byte at `0x00463cad` is `0x18`; dword at `0x00463c00` is `0x0046223b`. New byte-read evidence: `ev_75193169dc19179541167fa296ad9e6f826d143fb50b258a12354d076bab4609`, `ev_2b6bad8f54bd4cd31f0403617f5a8a061561e552f07909721582aeb62531316c`.

**CONFIRMED receiver fetch:** `0x0046223b..0x0046226f` advances CODE's cursor at `+0x180` by 5, loads its initial operand, bounds-checks CODE's reference array using `+0x18/+0x1c` and shift by 5, then passes:

```text
C = executing CODE (EDI in this handler)
T = D(C+0x18)
reference_count = (D(C+0x1c)-T)/0x20   [well-formed bounds]
i = initial operand, here 0
R0 = D(T + i*0x20 + 0x18)
```

Native instructions: `0x00462242` (T), `0x00462252..0x0046225a` (end/stride/bounds), `0x00462263..0x0046226b` (record and receiver), and `0x0046226f` (resolver call). The full retained interpreter assembly is `ev_784062c529976230a5a80893f19110a7b9c8455f120166f5ade005cb5749ce80`.

**CONFIRMED member-index transformation:** resolver `FUN_00460540`, new assembly `ev_64956711ff9f888def0118a5fef7db81bb85ae94de44f29b1c97d3d8baf0eb26`, fetches each 32-bit member operand through CODE's cursor. `0xffffffff` terminates the chain before masking. Otherwise bit 0x8000 optionally preserves the current receiver on the VM stack; `0x004605d9` computes `k=operand & 0x7fff`, and `0x004605d1..0x004605e2` calls `R->vtable[+0x98](k)` with R in ECX. The returned pointer becomes the next receiver.

Thus **0x055f is namespace-relative member index 1375 at this edge**, unchanged by the mask. It is not the CODE reference-record index (that is 0), not a byte offset, and not evidence of a named global. The resolver performs no name/key search or additional mapping at this edge. Which array supplies the index depends on the receiver's actual accessor and mode.

## 3. Actual receiver and missing identity edge

**CONFIRMED static path:** for the checked opcode 0x40 expression, R0 is the pointer in CODE reference record 0 at `+0x18`. If that pointer and its vtable were passively observed and validated, the native getter/storage branch could be selected without invocation. This path uses CODE's reference array, not CODE's embedded REF_NAMESPACE at `+0x24`.

**CONFIRMED prior live host context:** the frozen bound-scope log contains 78 HOST records with context `0x0354bd00`, vtable `0x0081fc04`, accessor entry `0x00459940`. Static RTTI table locator `0x0081fc00 -> 0x0084adcc` identifies VMACHINE, with NAMESPACE and MIRRORCLASS in its hierarchy. This makes host context a compatible indexed receiver, but the log did not observe R0 or the reference-array record binding. `R0 == host_context` remains **UNRESOLVED**.

The two direct CODE-vtable references resolve to destructor `FUN_004645a0` and constructor `FUN_00465bd0`. Constructor pseudocode sets CODE's reference-array begin/end to zero; it does not show population of record 0 or bind it to host context. These initialization observations are decompiler-supported, not claimed instruction-level field-write proof. No later binding/population routine was followed after identifying this missing edge.

**Single blocker:** establish how the relevant CODE's reference record 0 `+0x18` is bound to the concrete namespace/state owning script global operand 0x055f, and whether that object is the observed VMACHINE host context with getter 0x00459940. Neither pointer equality nor global ownership follows from a compatible vtable alone.

## 4. Conditional storage formulas; complete slot formula NOT proved

For a *validated* receiver executing this getter's direct-return branch:

```text
B = D(R+0xc0)
E = D(R+0xc4)
require B != 0, E >= B, (E-B)%0x10 == 0, 0x055f < (E-B)/0x10
record_address = B + 0x055f*0x10 = B + 0x55f0
stored_pointer = D(record_address+0x0c) = D(B+0x55fc)
```

For mode 7 substitute `R=D(original_receiver+0x90)` for the storage receiver. For modes 4/6, a NULL local stored pointer requires validated fallback handling. The control/cast/unwrap branch above must also be respected when applicable. `record_address` and `B+0x55fc` are **not** the VMValue object; the dword fetched there is its stored pointer. These formulas are confirmed accessor mechanics, **not a proved global-slot acquisition formula** until the missing R0 binding is established.

An object's first word is its vtable/type metadata pointer in the established paths. Record `+4` flags and receiver `+0xcc` mode are not VMValue tags. Exact REF_NAMESPACE vtable `0x007de8cc` permits its own payload read at object `+0x18`, as previously validated. That offset must not be applied to arbitrary slot values or to the separate REF<MIRRORCLASS> conversion without type-specific evidence. Slot type, value, and reference candidate remain **UNRESOLVED**.

## 5. Decision and preservation

RuntimeProbe **not extended**; build/tests **not applicable** for this documentation/static-evidence pass. Next live capture **NO-GO**. Stop condition **B**: the single reference-record receiver binding is missing. Next bounded evidence step: identify the population/binding operation that assigns the relevant CODE record 0's `+0x18` pointer, and connect it to the concrete global namespace. Only then derive the complete guarded passive read and implement it. No more analysis in this pass.

Evidence: [new native records](EVIDENCE/2026-10-12_slot_055f_storage_native.json), [retained records](EVIDENCE/2026-10-12_slot_055f_storage_retained.json), [local bytes](EVIDENCE/2026-10-12_slot_055f_storage_local.json), [result](EVIDENCE/2026-10-12_slot_055f_storage_result.json). Historical null-payload evidence preserved; its experiment was not repeated. REA/Ghidra session closed after the bounded investigation.
