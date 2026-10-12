# Reverse Engineering Rules & Verification Framework

**Baseline**: This document defines how all reverse-engineering work on Velvet Assassin is conducted, verified, and measured. It prevents instruction drift, ensures correctness-first prioritization, and provides agents with an objective PASS/FAIL signal.

Derived from: Anthropic's "Decompilation as Collaboration" project findings.

---

## Core Principles

1. **Precise instructions are mandatory**
   - Every task states explicit success criteria
   - Ambiguity is resolved BEFORE work begins
   - No agent interpretation; only following defined rules

2. **Correctness > Productivity**
   - Wrong code/analysis costs more time than restarting
   - Verification harness is the source of truth
   - Throughput is secondary to PASS signal

3. **Machine-checkable acceptance**
   - Every deliverable has an objective test
   - Human review is secondary; harness is primary
   - "Looks good" is never a valid PASS

4. **Instructions refresh hourly** (for multi-hour sessions)
   - Agents forget rules as context grows
   - Periodic re-statement prevents drift
   - Copy these rules at the start of each task

5. **Throw away bad code early**
   - If initial implementation is wrong, restart, don't salvage
   - Cost of restart < cost of debugging incorrect baseline
   - Clear FAIL signal triggers fresh approach

---

## Task Definition Template

Every RE task **must include**:

```markdown
# Task: [Name]

## Objective
[One sentence. What are we learning/building?]

## Success Criteria (Machine-Checkable)
- [ ] Criterion 1: [Testable condition with objective pass/fail]
- [ ] Criterion 2: [Measurable outcome]
- [ ] Criterion 3: [Verification method specified]

## Constraints
- [Rule 1: What is NOT allowed]
- [Rule 2: Scope boundaries]
- [Rule 3: Tool/method restrictions]

## Inputs
- File: [Path, hash, size]
- Tool: [Ghidra version, Codex CLI version, etc.]
- Context: [Prior findings it depends on]

## Outputs
- [Exact format expected]
- [Location/branch it goes to]
- [Test procedure to validate]

## Verification Harness
[Pseudocode or command that objectively measures success]

Example:
\`\`\`bash
# PASS if output contains exactly:
grep "Player_Character::SetOutfit at 0x0020eeeb" FINDINGS.md
grep "1436 bytes" FINDINGS.md
sha256sum src\live3d.mir.bin | grep "404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099"
\`\`\`

## Failure Recovery
[If harness returns FAIL, what is the next step?]
- Option A: [Restart with modified approach]
- Option B: [Escalate for human review]
- Option C: [Pivot to alternative method]
```

---

## Reverse-Engineering Rules (Agent Checklist)

These rules apply to **all** RE work. Violating any rule = FAIL, restart required.

### Category 1: File Integrity

**RULE-R1**: Never modify original game files unless explicitly authorized
- ✓ Read-only analysis only
- ✓ All edits go to `BACKUPS/` or new branches
- ✗ Do not overwrite `replay.exe`, `src\live3d.mir.bin`, etc.
- **Test**: `sha256sum` original files before/after task
- **Fail Signal**: Hash mismatch

**RULE-R2**: Every backup must be verifiable
- ✓ Before any patch: `sha256sum > BACKUPS/HASH_LOG.txt`
- ✓ Restore path documented
- ✓ Rollback tested
- ✗ "Backup" that doesn't hash-verify = invalid
- **Test**: `diff -u original restored` returns empty
- **Fail Signal**: Diff returns differences

**RULE-R3**: Document file metadata before analysis
- ✓ Path, size, SHA-256, timestamp logged
- ✓ Tool versions logged (Ghidra, Codex, etc.)
- ✓ All findings reference file identity
- ✗ Findings without metadata = non-reproducible
- **Test**: `FINDINGS.md` contains file path, size, hash, tool versions
- **Fail Signal**: Any missing metadata

---

### Category 2: Findings Integrity

**RULE-R4**: Separate CONFIRMED, LIKELY, SPECULATIVE strictly
- ✓ CONFIRMED = observed in assembly/bytecode, cross-verified
- ✓ LIKELY = reconstruction based on evidence, unverified at runtime
- ✓ SPECULATIVE = hypothesis, no evidence yet
- ✗ Mixing levels (e.g., "LIKELY x86 code calls script function" without assembly proof)
- **Test**: Every claim in FINDINGS.md is labeled with one of the three
- **Fail Signal**: Any unlabeled claim or mixed categories

**RULE-R5**: Evidence links are mandatory
- ✓ "Function X at 0x123456 (ref: Ghidra session, Evidence ID: ev_abc123)"
- ✓ "String found at 0x456789 in live3d.mir.bin (file offset, verified by hex dump)"
- ✗ "SetOutfit is important" without address/reference
- **Test**: `grep -E "(0x[0-9a-fA-F]{6,8}|Evidence ID|file offset)" FINDINGS.md` returns every claim
- **Fail Signal**: Claims without evidence links

**RULE-R6**: Runtime behavior is never assumed from static analysis alone
- ✓ "Assembly shows X loads into ECX"
- ✓ "Script bytecode branches on outfit_id"
- ✓ "User reported: Level 1 with Outfit=0 displayed Campaign Violette"
- ✗ "SetOutfit is definitely called at runtime" (without user test or debugger proof)
- **Test**: Any claim about runtime behavior has "User reported:", "Debugger observed:", or "Tested in-game:"
- **Fail Signal**: Speculation treated as confirmed

---

### Category 3: Scope & Boundaries

**RULE-R7**: Stay within authorized scope
- ✓ Authorized: Read-only analysis of game files
- ✓ Authorized: Documentation of findings
- ✓ Authorized: Python prototypes (not run against live game)
- ✗ Unauthorized: Modify game without explicit task authorization
- ✗ Unauthorized: Distribute patches without testing
- **Test**: Task explicitly states "authorize" or "perform" for any write operation
- **Fail Signal**: Write operation without auth line in task definition

**RULE-R8**: Don't chase unknown rabbit holes
- ✓ Follow referenced function chains (A → B → C confirmed)
- ✓ Investigate when evidence points to new finding
- ✗ "Let's check 20 more functions just in case"
- **Test**: Investigation stops at defined boundary (e.g., "upstream from LoadScene")
- **Fail Signal**: Unbounded exploration, findings proliferate without focus

**RULE-R9**: Document uncertainty explicitly
- ✓ "We did not find X. Limitations: strings search covers Ghidra-recognized data only."
- ✓ "Runtime dispatch is indirect; static xrefs may be incomplete."
- ✓ "This analysis covers replay.exe only; live3d.mir.bin inspection is pending."
- ✗ Silence on limitations (reader assumes complete analysis)
- **Test**: FINDINGS.md includes "Limitations:" section
- **Fail Signal**: Limitations missing or vague

---

### Category 4: Tool & Method Correctness

**RULE-R10**: Tool versions and capabilities are logged
- ✓ "Ghidra 12.1.4, x86 architecture, analysis complete"
- ✓ "Codex CLI search: found 3 matches, all logged"
- ✓ "Hex dump via hexdump(1); first 1KB verified"
- ✗ "Used Ghidra" (which version? what analysis level?)
- **Test**: `SESSION_LOG.md` contains tool version, date, configuration for every operation
- **Fail Signal**: Tool details missing

**RULE-R11**: Decompiler output is clearly labeled
- ✓ "Ghidra decompilation (pseudocode, not source):"
- ✓ "Type recovery: INCOMPLETE (see Ghidra warnings below)"
- ✓ "String: found at 0x007f4798 via static search, may be unreachable at runtime"
- ✗ Pasting decompilation without caveats
- **Test**: Every decompilation excerpt is preceded by tool name + disclaimer
- **Fail Signal**: Raw decompilation pasted without context

**RULE-R12**: Cross-verification for critical findings
- ✓ String found via Ghidra search AND confirmed in hex dump
- ✓ Function address confirmed via multiple xrefs
- ✓ Outfit change tested in-game with config edit
- ✗ "Ghidra says X, so X is true"
- **Test**: Critical findings (e.g., SetOutfit location, outfit mapping) verified via 2+ independent methods
- **Fail Signal**: Single-method verification for mission-critical functions

---

### Category 5: Communication & Clarity

**RULE-R13**: No hedging on findings
- ✓ "CONFIRMED: X is true" or "LIKELY: Y is probable" or "SPECULATIVE: Z is possible"
- ✗ "Probably maybe possibly SetOutfit is at this address"
- **Test**: Findings use only: CONFIRMED, LIKELY, SPECULATIVE (no "probably", "might", "seems")
- **Fail Signal**: Hedging language detected

**RULE-R14**: Every finding has a timestamp and author
- ✓ "2026-10-11 14:43:20 UTC — agent:copilot — FUN_0061c160 identified"
- ✗ Undated findings
- **Test**: `grep -E "[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}" FINDINGS.md` returns all entries
- **Fail Signal**: Undated findings

**RULE-R15**: Task authorization is explicit and logged
- ✓ Task begins with: "User authorized: [specific action]"
- ✓ Any deviation from authorized scope is rejected
- ✓ Session log documents what was authorized vs. what was performed
- ✗ Proceeding without explicit authorization
- **Test**: `SESSION_LOG.md` starts with: "AUTHORIZED: [...]"
- **Fail Signal**: Missing authorization log

---

## Verification Harnesses (By Task Type)

### Harness: Static Analysis Task

```bash
#!/bin/bash
# Verify reverse-engineering findings

REPO="rr9889/VelvetAssassinModdingExperiment"
BRANCH="main"

echo "=== Verifying Static Analysis Findings ==="

# R1: Original files unchanged
echo "[R1] Checking file integrity..."
sha256sum replay.exe | grep "9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132" && echo "✓ PASS" || echo "✗ FAIL"
sha256sum src/live3d.mir.bin | grep "404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099" && echo "✓ PASS" || echo "✗ FAIL"

# R3: File metadata logged
echo "[R3] Checking metadata documentation..."
grep -q "replay.exe.*5,164,544 bytes" CURRENT_STATE.md && echo "✓ PASS" || echo "✗ FAIL"
grep -q "live3d.mir.bin.*3,459,973 bytes" FINDINGS.md && echo "✓ PASS" || echo "✗ FAIL"

# R4: Confidence levels present
echo "[R4] Checking CONFIRMED/LIKELY/SPECULATIVE labels..."
grep -E "(CONFIRMED|LIKELY|SPECULATIVE):" FINDINGS.md | wc -l | awk '{if($1>5) print "✓ PASS"; else print "✗ FAIL"}'

# R5: Evidence links present
echo "[R5] Checking evidence links..."
grep -E "(0x[0-9a-fA-F]{6,8}|Evidence ID)" FINDINGS.md | wc -l | awk '{if($1>3) print "✓ PASS"; else print "✗ FAIL"}'

# R9: Limitations documented
echo "[R9] Checking uncertainty documentation..."
grep -q "Limitations:" FINDINGS.md && echo "✓ PASS" || echo "✗ FAIL"

# R12: Cross-verification for SetOutfit
echo "[R12] Cross-verifying SetOutfit findings..."
grep -q "Player_Character::SetOutfit" FINDINGS.md && \
grep -q "0x0020ef0b" FINDINGS.md && \
grep -q "1436 bytes" FINDINGS.md && \
echo "✓ PASS (3/3 references found)" || echo "✗ FAIL"

# R14: Timestamps present
echo "[R14] Checking timestamps..."
grep -E "[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}" FINDINGS.md | wc -l | awk '{if($1>1) print "✓ PASS"; else print "✗ FAIL"}'

# R15: Authorization logged
echo "[R15] Checking authorization log..."
grep -q "AUTHORIZED:" SESSION_LOG.md && echo "✓ PASS" || echo "✗ FAIL"

echo ""
echo "=== Summary ==="
echo "If all checks return ✓ PASS, the task meets RE standards."
echo "If any return ✗ FAIL, the task must be restarted or escalated."
```

### Harness: Code Patch Task

```bash
#!/bin/bash
# Verify that a patch is correct and safe

echo "=== Verifying Code Patch ==="

# R1: Backup exists and is valid
echo "[R1] Checking backup..."
sha256sum BACKUPS/pre-hotkey-mod-full/replay.exe.original | grep -q "[0-9a-f]\{64\}" && \
  echo "✓ PASS (backup SHA-256 logged)" || echo "✗ FAIL"

# R2: Rollback path verified
echo "[R2] Testing rollback..."
cp BACKUPS/pre-hotkey-mod-full/replay.exe replay.exe.test
sha256sum replay.exe.test | grep "9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132" && \
  echo "✓ PASS (rollback restores original)" || echo "✗ FAIL"
rm replay.exe.test

# R7: Scope authorization present
echo "[R7] Checking authorization..."
grep -q "authorize.*patch replay.exe" NEXT_STEPS.md || grep -q "User authorized" SESSION_LOG.md && \
  echo "✓ PASS (patch authorized)" || echo "✗ FAIL"

# R15: Authorization logged before patch
echo "[R15] Checking patch authorization log..."
grep -B5 "patch deployed" SESSION_LOG.md | grep -q "AUTHORIZED" && \
  echo "✓ PASS" || echo "✗ FAIL"

echo ""
echo "All checks must PASS before deploying patch."
```

### Harness: In-Game Test Task

```bash
#!/bin/bash
# Verify that in-game test results are properly logged

echo "=== Verifying In-Game Test Results ==="

# R6: Runtime behavior properly attributed
echo "[R6] Checking attribution of runtime observations..."
grep -q "User reported:" CURRENT_STATE.md && echo "✓ PASS" || echo "✗ FAIL"
grep -q "Tested in-game:" CURRENT_STATE.md && echo "✓ PASS" || echo "✗ FAIL"

# R14: Test timestamp and details logged
echo "[R14] Checking test log completeness..."
grep -q "2026-10-" CURRENT_STATE.md && \
grep -q "Level:" CURRENT_STATE.md && \
grep -q "Outfit=" CURRENT_STATE.md && \
echo "✓ PASS (all test details present)" || echo "✗ FAIL"

# R9: Limitations of test documented
echo "[R9] Checking test limitations..."
grep -q "scope\|limitation" CURRENT_STATE.md && echo "✓ PASS" || echo "✗ FAIL"

echo ""
echo "Test results are valid only if all checks PASS."
```

---

## Instruction Refresh (Hourly)

**For multi-hour sessions**, re-state these rules at the start of each hour:

```markdown
## Instruction Refresh — [Time]

**Core Rules (Non-Negotiable)**:
1. RULE-R1: Never modify original files without authorization
2. RULE-R4: CONFIRMED/LIKELY/SPECULATIVE must be labeled
3. RULE-R6: Runtime behavior requires user test or debugger proof
4. RULE-R12: Critical findings must be cross-verified

**Today's Task**: [Task name and objective]

**Authorized Actions**:
- [ ] Read-only analysis
- [ ] Documentation
- [ ] Running verification harness

**Forbidden Actions**:
- [ ] Modify game files
- [ ] Distribute patches
- [ ] Make runtime assumptions from static analysis alone

**Deliverable**: [What will be produced]

**Success Signal**: [Harness passes ALL checks]

**Proceed?**
```

---

## Failure Modes & Recovery

| Failure Mode | Symptom | Recovery |
|---|---|---|
| **Drift** | Rules ignored after hour 2 | Refresh instructions (hourly) |
| **Bad Foundation** | Initial analysis wrong, built upon | RESTART; don't salvage |
| **Scope Creep** | 20 functions analyzed instead of 3 | Reject findings, re-scope, restart |
| **Unverifiable** | Claims with no evidence links | Add evidence links or mark SPECULATIVE |
| **Mixed Confidence** | LIKELY treated as CONFIRMED | Reclassify, document uncertainty |
| **Unauthorized Write** | Patch applied without R15 check | Restore backup, require explicit authorization |

---

## Metrics (Track Over Time)

Record these per session:

```markdown
## Session Metrics — [Date]

- Tasks completed: N
- Tasks PASS on first attempt: M / N
- Average rework cycles per failed task: X
- Instructions refreshed: Y times
- Rule violations caught: Z
- Critical findings (cross-verified): K
```

**Target**: >80% first-attempt PASS rate by week 4.

---

## Checklist: Before Committing Findings

- [ ] **R1**: Original files hashes verified, unchanged
- [ ] **R2**: Backups created, rollback tested
- [ ] **R3**: File metadata (path, size, hash, tool version) documented
- [ ] **R4**: Every claim labeled CONFIRMED, LIKELY, or SPECULATIVE
- [ ] **R5**: Evidence links (address, offset, Evidence ID) present
- [ ] **R6**: Runtime claims attributed to user test or debugger
- [ ] **R7**: Task authorization documented
- [ ] **R8**: Scope boundary respected, no excessive exploration
- [ ] **R9**: Limitations explicitly stated
- [ ] **R10**: Tool versions logged
- [ ] **R11**: Decompiler output labeled with disclaimers
- [ ] **R12**: Critical findings cross-verified (2+ methods)
- [ ] **R13**: No hedging language (only CONFIRMED/LIKELY/SPECULATIVE)
- [ ] **R14**: Timestamps and authorship logged
- [ ] **R15**: Authorization line in SESSION_LOG.md

**All 15 checks pass? Commit. Any fail? Restart or escalate.**

---

## Next Steps for This Project

**Apply these rules to all future RE work on Velvet Assassin:**

1. Level Editor Phase 1 (Reverse .scene format)
   - ✓ Task defined with explicit success criteria
   - ✓ Verification harness written
   - ✓ Rules checklist reviewed
   - ✓ Ready for execution

2. Hotkey Mod Implementation
   - ✓ Authorization: User confirmed "Ctrl+Q hotkey swap"
   - ✓ Scope: "Input hook → script bytecode → SetOutfit call"
   - ✓ Harness: "In-game outfit cycles on hotkey press"
   - ✓ Rollback: Backup verified, restore documented

3. Future Tasks
   - Every task must have a harness BEFORE work starts
   - Every session must refresh these rules
   - Every finding must pass the 15-point checklist

---

## References

- **Source**: Anthropic's "Decompilation as Collaboration" project (2024)
- **Key Insight**: "Correctness is so much more important than productivity"
- **Evidence**: Early salvage attempts cost 3x more than restarting with correct approach

---

**Last Updated**: 2026-10-12
**Status**: ACTIVE — Apply to all Velvet Assassin RE work
