# INVESTIGATION TASK TEMPLATE

> RECONCILIATION: See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md). SetOutfit examples below are templates, not completed verification or authorization. Its 0x0020ef0b location is script bytecode, not a native VA; the shown cdecl/ECX/EDX signatures, assembly percentages, and test results are unsupported examples. For the next task, use the saved native VM call boundary and evidence-backed object/ABI/thread criteria; do not compile an imagined x86 SetOutfit implementation to validate custom VM bytecode. Current pass creates documentation only, no harness or implementation code.

Use this template for EVERY reverse-engineering task. Copy, fill in ALL fields, commit to repo BEFORE starting work.

---

## [TASK NAME]

### OBJECTIVE
**One sentence. What are we learning/verifying?**

Example: "Verify Player_Character::SetOutfit signature and confirm it controls outfit state at runtime."

---

### TARGET
**Exact address(es) or file(s) being investigated.**

```
Address: 0x0020ef0b
File: src\live3d.mir.bin (file offset 0x0020ef0b)
Size: 1436 bytes
Hash: [sha256 if known]
Compiler Context: x86, cdecl, MSVC
```

---

### EVIDENCE SOURCES
**What will we examine? Rank by trust (Rank 1 = highest).**

- [ ] **Rank 1 (Assembly)**: Original binary assembly at 0x0020ef0b (objdump output)
- [ ] **Rank 2 (Symbols)**: [If available: debug PDB, leaked source, symbol table]
- [ ] **Rank 3 (Decompilation)**: Ghidra decompiled C (semantic seed only)
- [ ] **Rank 4 (Semantics)**: Xref analysis — who calls this function, with what arguments?
- [ ] **Rank 5 (In-Game)**: User test or debugger observation

**Example**:
```
- Rank 1: objdump -d replay.exe | grep -A30 "0x0020ef0b"
- Rank 3: Ghidra decompilation of FUN_0020ef0b
- Rank 4: Cross-references (callers at 0x123456, 0x234567)
- Rank 5: User reported: "Pressing hotkey cycles outfit in-game"
```

---

### VERIFICATION HARNESS
**Machine-checkable test. Must return PASS or FAIL. No subjective judgment.**

```bash
#!/bin/bash

echo "=== VERIFICATION HARNESS: SetOutfit Signature ==="

# Check 1: Assembly extracted
if [ -f setoutfit_original.asm ]; then
    echo "[PASS] Assembly file exists"
else
    echo "[FAIL] Assembly file missing"
    exit 1
fi

# Check 2: Candidate compiles
if gcc -c setoutfit_candidate.c -o setoutfit_candidate.o 2>/dev/null; then
    echo "[PASS] Candidate compiles"
else
    echo "[FAIL] Candidate compilation failed"
    exit 1
fi

# Check 3: Assembly match >= 95%
DIFF_LINES=$(diff <(objdump -d setoutfit_original.o) \
                   <(objdump -d setoutfit_candidate.o) | wc -l)
TOTAL_LINES=$(objdump -d setoutfit_original.o | wc -l)
MATCH_PCT=$(echo "scale=1; 100 * (1 - $DIFF_LINES / (2 * $TOTAL_LINES))" | bc)

if (( $(echo "$MATCH_PCT >= 95" | bc -l) )); then
    echo "[PASS] Assembly match: ${MATCH_PCT}%"
else
    echo "[FAIL] Assembly match: ${MATCH_PCT}% (need >= 95%)"
    exit 1
fi

# Check 4: Signature consistent with callers
if grep -q "mov ecx, \[ebp+0x8\]" setoutfit_original.asm; then
    echo "[PASS] First argument in ECX (this pointer)"
else
    echo "[FAIL] Unexpected argument passing"
    exit 1
fi

if grep -q "mov edx, \[ebp+0xc\]" setoutfit_original.asm; then
    echo "[PASS] Second argument in EDX (outfit_id)"
else
    echo "[FAIL] Unexpected argument passing"
    exit 1
fi

# Check 5: In-game test (if applicable)
if [ -f in_game_test_log.txt ]; then
    if grep -q "Outfit=0.*Campaign Violette" in_game_test_log.txt; then
        echo "[PASS] In-game behavior matches outfit_id=0"
    else
        echo "[FAIL] In-game test failed"
        exit 1
    fi
fi

echo ""
echo "=== RESULT: ALL CHECKS PASSED ==="
exit 0
```

**CRITICAL**: Harness must be executable and return exit code 0 on PASS, 1 on FAIL.

---

### CONSTRAINTS
**What is NOT allowed. What are the boundaries?**

- ✗ Do not modify `replay.exe` or `src\live3d.mir.bin`
- ✗ Do not assume decompiler output is correct without assembly verification
- ✗ Do not make runtime claims without in-game test or debugger
- ✗ Do not exceed 2-hour time limit; if harness fails, restart (don't debug endlessly)
- ✓ Read-only analysis only
- ✓ Can generate C/Rust candidates for verification
- ✓ Can run binaries in emulator/debugger
- ✓ Can execute external tools (Ghidra, objdump, etc.)

---

### AUTHORIZED ACTIONS
**What am I permitted to do?**

- [ ] Analyze binary with Ghidra
- [ ] Extract assembly with objdump
- [ ] Search for xrefs with Codex CLI
- [ ] Generate C candidates with LLM
- [ ] Compile candidates locally
- [ ] Run in-game tests
- [ ] Document findings in repo
- [ ] Other: _______________

---

### SUCCESS CRITERIA (Harness Checks)

**PASS (All of the following)**:
1. [ ] Assembly match ≥ 95%
2. [ ] Calling convention verified (argument passing correct)
3. [ ] Xref analysis confirms signature across all callers
4. [ ] In-game behavior matches hypothesis [if applicable]
5. [ ] No contradictions between Rank 1-5 evidence

**FAIL (Any of the following)**:
1. [ ] Compilation error
2. [ ] Assembly match < 95%
3. [ ] Xref inconsistency
4. [ ] In-game test contradicts analysis
5. [ ] Time limit exceeded (2 hours)

---

### FAILURE MODE & RECOVERY

**If Harness Returns FAIL**:

| Failure | Action |
|---|---|
| **Compilation error** | Review candidate syntax; try different approach; escalate if unclear |
| **Assembly match < 95%** | Try 1 alternative candidate. If still fails, mark DEFERRED and move to next task |
| **Xref inconsistency** | Re-verify caller analysis; check if function is inlined; mark SPECULATIVE |
| **In-game contradicts** | Retest in-game with fresh level; verify config state; if persists, RESTART |
| **Time limit exceeded** | STOP. Mark task DEFERRED. Do NOT continue debugging. |

**RESTART Condition**:
- If first candidate fails harness AND retry doesn't improve within 30 min, RESTART
- Do NOT attempt to "salvage" bad approach
- Document failure in memory/MEMORY.md as DEFERRED
- Move to next task

---

### INPUTS
**What do I have before starting?**

- File: `replay.exe` (SHA-256: 9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132)
- Tool: Ghidra 12.1.4
- Tool: Codex CLI
- Tool: gcc (or clang)
- Context: [Link to prior findings]
  - FINDINGS.md: SetOutfit confirmed as outfit state controller
  - CURRENT_STATE.md: User test confirmed outfit mapping (0=Campaign, 1=Pilot, 2=Nightdress)
  - memory/MEMORY.md: No prior analysis of SetOutfit implementation

---

### OUTPUTS
**What will be produced?**

**On PASS**:
```
docs/FUNCTION_RECONSTRUCTIONS.md
  - Entry: "SetOutfit @ 0x0020ef0b: VERIFIED 97%"
  - Candidate source: setoutfit_candidate.c
  - Assembly diff: setoutfit.diff
  - Status: CLOSED

memory/MEMORY.md
  - [VERIFIED 97%] SetOutfit @ 0x0020ef0b
    - Signature: void SetOutfit(actor_t* self, int outfit_id)
    - Assembly match: 97%
    - Calling convention: cdecl (ECX=this, EDX=outfit_id)
    - Xref consensus: 3/3 callers match
```

**On FAIL**:
```
docs/DEFERRED_FUNCTIONS.md
  - Entry: "SetOutfit @ 0x0020ef0b: DEFERRED (match_score=42%, compiler inlining suspected)"

memory/MEMORY.md
  - [DEFERRED] SetOutfit @ 0x0020ef0b
    - Reason: Assembly match < 95% on 2 attempts
    - Next investigator: Try different compiler flags or check for inlining artifacts
```

---

### AUTHORIZATION
**User approval to proceed.**

```
User authorized: [Specific action]
Date: [ISO 8601 timestamp]
Scope: [What is permitted]
Escalation contact: [How to ask for help]
```

**Example**:
```
User authorized: Analyze SetOutfit assembly and verify signature via candidate compilation
Date: 2026-10-12T01:00:00Z
Scope: Read-only analysis, local compilation, no game modifications
Escalation: If assembly match < 90%, mark DEFERRED and move to next function
```

---

### MEMORY CHECKPOINT
**Before starting, read prior findings. Log new discoveries here.**

**Prior state** (from memory/MEMORY.md):
```
- [VERIFIED] Architecture: x86, 32-bit, cdecl
- [VERIFIED] Outfit field exists (0/1/2 mapping)
- [HYPOTHESIZED] SetOutfit controls mesh visibility
```

**New findings** (will add after task):
```
- [VERIFIED 97%] SetOutfit @ 0x0020ef0b signature matches cdecl
- [VERIFIED] Xref analysis: 3 callers, all use (actor_ptr, int) pattern
- [VERIFIED in-game] Outfit changes persist after SetOutfit call
```

---

## TASK COMPLETION CHECKLIST

Before committing findings, verify:

- [ ] Harness written and executable
- [ ] Harness returns PASS or FAIL (no ambiguity)
- [ ] All evidence sources checked (Rank 1-5)
- [ ] Assembly match verified (>= 95% or DEFERRED)
- [ ] Xref analysis cross-verified
- [ ] In-game test performed (if applicable)
- [ ] Time limit respected (2 hours max)
- [ ] memory/MEMORY.md updated
- [ ] Findings committed with [VERIFIED] or [DEFERRED] label
- [ ] Session log updated with timestamp and result

---

## EXAMPLE: Filled Template

### SetOutfit Signature Verification

**OBJECTIVE**
Verify Player_Character::SetOutfit at 0x0020ef0b accepts (actor_t*, int outfit_id) and controls visible mesh groups.

**TARGET**
```
Address: 0x0020ef0b
File: src\live3d.mir.bin (compiled script bytecode)
Size: 1436 bytes
Context: x86 32-bit, cdecl calling convention
```

**EVIDENCE SOURCES**
- [x] Rank 1: Assembly from objdump (extracted to setoutfit_original.asm)
- [x] Rank 3: Ghidra decompilation (semantic reference only)
- [x] Rank 4: Xref analysis (found 3 callers: 0x123456, 0x234567, 0x345678)
- [x] Rank 5: User in-game test (outfit changed from 1→0, confirmed Campaign Violette)

**VERIFICATION HARNESS**
```bash
#!/bin/bash
# setoutfit_harness.sh

echo "=== Verifying SetOutfit ==="

# 1. Assembly exists
[ -f setoutfit_original.asm ] || { echo "FAIL: no assembly"; exit 1; }

# 2. Candidate compiles
gcc -c setoutfit_candidate.c -o setoutfit.o 2>/dev/null || { echo "FAIL: compile error"; exit 1; }

# 3. Assembly match >= 95%
DIFF=$(diff setoutfit_original.asm <(objdump -d setoutfit.o) | wc -l)
TOTAL=$(wc -l < setoutfit_original.asm)
PCT=$(echo "scale=0; 100 * (1 - $DIFF / (2 * $TOTAL))" | bc)
[ "$PCT" -ge 95 ] || { echo "FAIL: only ${PCT}% match"; exit 1; }

# 4. Xrefs consistent
grep -q "mov ecx" setoutfit_original.asm || { echo "FAIL: no ECX load"; exit 1; }
grep -q "mov edx" setoutfit_original.asm || { echo "FAIL: no EDX load"; exit 1; }

# 5. In-game test
grep -q "Outfit=0.*Campaign" in_game_test.log || { echo "FAIL: in-game mismatch"; exit 1; }

echo "PASS: SetOutfit verified (${PCT}% assembly match)"
exit 0
```

**CONSTRAINTS**
- ✗ No replay.exe modification
- ✗ No assuming Ghidra is correct
- ✗ No runtime claims without in-game test
- ✓ Read-only analysis
- ✓ Local compilation allowed

**AUTHORIZED ACTIONS**
- [x] Analyze with Ghidra
- [x] Extract assembly
- [x] Xref search
- [x] Compile candidates
- [x] In-game testing
- [x] Document findings

**SUCCESS CRITERIA**
- [x] Assembly match ≥ 95%
- [x] Calling convention verified
- [x] Xref consensus (3/3 callers match)
- [x] In-game behavior confirms

**FAILURE RECOVERY**
If match < 95%: Try 1 alternative candidate. If still fails: Mark DEFERRED, move to next function.

**INPUTS**
- replay.exe + Ghidra session
- Prior findings: Outfit mapping (0/1/2) confirmed in-game

**OUTPUTS (On PASS)**
```
docs/FUNCTION_RECONSTRUCTIONS.md
  SetOutfit @ 0x0020ef0b: VERIFIED 97%

memory/MEMORY.md
  [VERIFIED 97%] SetOutfit signature: void SetOutfit(actor_t* self, int outfit_id)
```

**AUTHORIZATION**
```
User authorized: Verify SetOutfit signature via assembly match + in-game test
Date: 2026-10-12T01:00:00Z
Scope: Read-only, local compilation only
```

**RESULT**
```
✓ PASS — SetOutfit verified
  - Assembly match: 97%
  - Signature: void SetOutfit(actor_t* self, int outfit_id)
  - Xref consensus: 3/3 callers use (actor_ptr, int)
  - In-game confirmed: Outfit=0 → Campaign Violette
  - Status: CLOSED
```

---

## TEMPLATE USAGE WORKFLOW

1. **Copy this template into a new file**:
   ```bash
   cp INVESTIGATION_TASK_TEMPLATE.md task_[FUNCTION_NAME]_[ADDRESS].md
   ```

2. **Fill in ALL sections**:
   - OBJECTIVE (one sentence)
   - TARGET (exact address)
   - EVIDENCE SOURCES (all that apply)
   - VERIFICATION HARNESS (executable script)
   - CONSTRAINTS (what's forbidden)
   - INPUTS (what you have)
   - OUTPUTS (what will be produced)

3. **Commit BEFORE starting work**:
   ```bash
   git add task_setoutfit_0x0020ef0b.md
   git commit "Add investigation task: Verify SetOutfit signature"
   ```

4. **Run harness during investigation**:
   ```bash
   bash setoutfit_harness.sh
   # Returns: PASS or FAIL (exit 0 or 1)
   ```

5. **On PASS**: Commit results and update memory/MEMORY.md
6. **On FAIL**: Mark DEFERRED or retry per recovery plan

---

## QUICK REFERENCE: Common Harnesses

### Assembly Match Harness
```bash
#!/bin/bash
ORIG=$1
CAND=$2
DIFF=$(diff <(objdump -d "$ORIG") <(objdump -d "$CAND") | wc -l)
TOTAL=$(objdump -d "$ORIG" | wc -l)
PCT=$(echo "100 * (1 - $DIFF / (2 * $TOTAL))" | bc)
[ "$PCT" -ge 95 ] && echo "PASS" && exit 0 || echo "FAIL: ${PCT}%" && exit 1
```

### Xref Consistency Harness
```bash
#!/bin/bash
FUNC=$1
codex search "call $FUNC" replay.exe | while read LINE; do
    # Verify each caller loads ECX (this) and EDX (arg)
    grep -q "mov ecx\|mov edx" <(echo "$LINE") || exit 1
done && echo "PASS" || echo "FAIL"
```

### In-Game Test Harness
```bash
#!/bin/bash
# Verify in-game test results match hypothesis
grep -q "$1" in_game_test_log.txt && echo "PASS" || echo "FAIL: $1 not found in test log"
```

---

**Status**: READY FOR USE
**Last Updated**: 2026-10-12
**Apply To**: Every reverse-engineering investigation task
