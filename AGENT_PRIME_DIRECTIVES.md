# RE GROUND TRUTH PROTOCOL: Agent Prime Directives

**Foundation**: This document defines how reverse-engineering work achieves maximum fidelity. It establishes an unambiguous hierarchy of truth and prevents the common trap of over-relying on decompiler output.

**Core Principle**: Assembly is law. Decompilation is a hint.

---

## AGENT PRIME DIRECTIVES (Non-Negotiable)

### DIRECTIVE 1: ASSEMBLY IS ABSOLUTE AUTHORITY

**Statement**:
Decompiled C is ONLY a semantic seed. The ultimate ground truth is the original binary's assembly output. If the generated C does not compile to matching assembly, it is wrong.

**Application**:
```
❌ WRONG: "Ghidra says SetOutfit is at 0x0020ef0b, so it is"
✅ RIGHT: "Assembly at 0x0020ef0b shows [instruction sequence]. Reconstructed C must compile to identical assembly."
```

**Verification Workflow**:
1. Export original assembly at target address (e.g., via Ghidra, IDA, objdump)
2. Generate candidate C/Rust source
3. Compile with matching compiler + flags
4. Diff output assembly against original
5. If match < 95%, FAIL → try different approach
6. Only if match ≥ 95%, mark as VERIFIED

**Example**:
```
# Original assembly (at 0x0060daa0):
0x0060daa0: push ebp
0x0060daa1: mov ebp, esp
0x0060daa3: mov ecx, [ebp+0x8]
0x0060daa6: test ecx, ecx
0x0060daa8: jz 0x0060dac0
...

# Candidate C:
void FUN_0060daa0(int param) {
    if (param == 0) goto label_dac0;
    ...
}

# After compilation:
0x0060daa0: push ebp
0x0060daa1: mov ebp, esp
0x0060daa3: mov ecx, [ebp+0x8]
0x0060daa6: test ecx, ecx
0x0060daa8: jz 0x0060dac0
...

✓ MATCH — Mark as VERIFIED
```

**Test Signal**:
```bash
# Binary diff score
diff <(objdump -d original.bin | grep -A50 "0x0060daa0") \
     <(objdump -d reconstructed.bin | grep -A50 "0x0060daa0") \
  | wc -l
# Score: lines of diff / total lines
# PASS if score < 5% (minor register allocation differences acceptable)
```

---

### DIRECTIVE 2: BOUNDED RECONSTRUCTION

**Statement**:
Never attempt to reconstruct an entire file at once. Focus strictly on one function at a specific address (e.g., 0x409092) per iteration.

**Application**:
```
❌ WRONG: "Let me reverse-engineer all 20 functions that load scenes..."
✅ RIGHT: "This iteration: verify FUN_0061c160 at 0x0061c160 only. Document dependencies as external."
```

**Workflow**:
1. Select ONE function by address
2. Export its assembly (start → end markers clear)
3. Generate candidate source for that ONE function
4. Treat external calls as opaque: `external_func_at_0x123456(params)`
5. Compile and verify assembly match
6. Log result in `docs/FUNCTION_RECONSTRUCTIONS.md`
7. Move to next function only after this one is VERIFIED or marked DEFERRED

**Example Iteration**:
```markdown
## Iteration 1: FUN_0061c160

**Target**: Scene loader at 0x0061c160
**Size**: ~120 bytes
**Dependencies**: 
  - Calls FUN_00502020 (external, not reconstructed yet)
  - Calls FUN_0061a750 (external, not reconstructed yet)

**Candidate Source**:
\`\`\`c
void FUN_0061c160(char* scene_id, int flags) {
    actor_t* actor = get_current_actor();
    if (actor->child_count > 0) {
        error("Actor already has children");
        return;
    }
    resource_t* res = FUN_00502020(scene_id, 3);
    if (res->state == 1) {
        FUN_0061a750(actor, res);
    }
}
\`\`\`

**Assembly Match**: 95% ✓ VERIFIED
**Status**: CLOSED — move to FUN_00502020

---

## Iteration 2: FUN_00502020

**Target**: Resource lookup at 0x00502020
...
```

**Test Signal**:
```bash
# Function size and boundary check
objdump -d original.bin | sed -n '/0x0061c160:/,/^$/p' | wc -l
# Must match reconstructed function size
```

---

### DIRECTIVE 3: STRUCTURAL SKEPTICISM

**Statement**:
Treat all inherited logic as UNVERIFIED. Re-derive against the current binary corpus.

**Application**:
```
❌ WRONG: "Prior analysis said SetOutfit takes an int outfit_id, so that's the signature"
✅ RIGHT: "I will verify SetOutfit's signature by examining the assembly at 0x0020ef0b and deriving the calling convention from the instructions."
```

**Verification Workflow**:
1. Start fresh for each address — do not inherit assumptions from prior analysis
2. Extract assembly
3. Identify calling convention indicators:
   - Argument loading (ECX, EDX, stack)
   - Stack frame setup
   - Return value placement (EAX, EDX:EAX, etc.)
4. Cross-verify via xref analysis (what calls this function, and what does it pass?)
5. Document both independent derivation and cross-verification results

**Example**:
```markdown
### SetOutfit Signature (FUN_0020ef0b)

**Assembly-Derived** (Directive 3: Skepticism):
- Entry: mov ecx, [esp+4] → first arg in ECX
- mov edx, [esp+8] → second arg in EDX (outfit_id)
- Call pattern: this + outfit → SetOutfit(this, outfit_id)

**Cross-Verification** (xref analysis):
- Found 3 callers, all pass (actor_ptr, int) → consistent
- No contradictions

**Confidence**: VERIFIED via assembly + xref consensus
```

**Test Signal**:
```bash
# Xref consistency check: do all callers match derived signature?
codex search "call FUN_0020ef0b" replay.exe
# For each match, verify:
#   - ECX contains actor pointer
#   - EDX contains integer (0, 1, or 2)
```

---

## TARGET ENVIRONMENT CONTEXT (Fill Before Starting)

**Every task must declare**:

```markdown
## Target Environment

**Architecture**: x86 (32-bit), Little-Endian
**Compiler**: Microsoft Visual C++ (MSVC) [version TBD from decompilation hints]
**Optimization**: Likely -O2 (moderate optimization, inlining not aggressive)
**Calling Convention**: cdecl (C declaration, push args R→L, callee cleans stack)
**Platform**: Windows 32-bit (Win32 API calls expected)
**Execution/Testing**: Velvet Assassin `replay.exe` or DREAMM emulator
**Platform Quirks**:
  - SSE/SIMD not detected (older game)
  - Legacy COM-style object layout likely (vtable pointer at +0)
  - No ASLR (assumes fixed addresses)
```

**Why This Matters**:
- Calling convention determines how arguments are passed (ECX vs stack)
- Compiler version affects inline function choices and code density
- Architecture determines instruction set (x86 ≠ x64 ASM)
- Optimization level affects code patterns (loop unrolling, inlining)

---

## GROUND TRUTH HIERARCHY (Strict Priority)

### Rank 1: Original Binary Assembly Export (ABSOLUTE AUTHORITY)

**Source**: Disassembler output (Ghidra, IDA, objdump) applied to `replay.exe` or `live3d.mir.bin`

**Status**: Ground truth for function boundaries, instruction sequences, register usage

**Usage**:
```bash
# Extract assembly for FUN_0061c160
objdump -d replay.exe | sed -n '/0x0061c160:/,/^[0-9a-f]*\s*<[^>]*>:$/p'
```

**Trust Level**: 100% — This is the law.

---

### Rank 2: Leaked/Debug Builds with Symbols (ROSETTA STONE)

**Source**: If available: debug PDB files, symbol tables, or leaked source

**Status**: Provides function names, struct field names, type information — enormously helpful semantic hints

**Usage**:
```
If symols available:
  - Function name: SetOutfit (vs Ghidra's generic FUN_0020ef0b)
  - Parameters: self, outfit_id (vs decompiler's p_1, p_2)
  - Struct members: knows actor->outfit, actor->mesh_groups

This is a "Rosetta Stone" — massive acceleration, but NOT ground truth.
```

**Trust Level**: 80% — Names and structs are usually right, but calling conventions and optimization quirks need verification.

**Note**: For Velvet Assassin, no leaked symbols known. This rank is hypothetical; proceed with Rank 3 + 1.

---

### Rank 3: Ghidra/IDA Decompiled C (SEMANTIC SEED ONLY)

**Source**: Decompiler output (`FUN_0061c160`, pseudo-C, variable names like `DAT_008c0000`)

**Status**: Useful for understanding control flow and high-level intent, but HIGHLY SUSPECT for details

**Usage**:
```
✓ Good for: "This function branches on some value and calls two other functions"
✗ Bad for: "This function signature is void SetOutfit(int p_1, int p_2)"

Always re-derive against assembly (Rank 1).
```

**Trust Level**: 40% — Use only as a reading aid, never as final truth.

**Common Decompiler Errors**:
- Type recovery is fragile (int vs pointer confusion)
- Inlining hides true function boundaries
- Aggressive optimization produces misleading control flow
- SSA form may not match source intent

---

### Rank 4: Raw Retail Assets (FINAL VERIFICATION)

**Source**: Game config files (`aio\levelstructure.aio`), scene data, asset paths

**Status**: Ground truth for game behavior and data structures

**Usage**:
```markdown
**Verified by in-game test** (from CURRENT_STATE.md):
- Outfit=0 → Campaign Violette
- Outfit=1 → Pilot Violette
- Outfit=2 → Nightdress

This validates the RE hypothesis: "Outfit field is an int, values 0/1/2 map to visual models."
```

**Trust Level**: 95% — User-reported in-game results are as close to truth as we get without source code.

---

## TOOLING & WORKFLOW PROTOCOL

### Orchestration Script: `binary-recons` (Pseudocode)

**Purpose**: Automate the assembly-diff loop to verify candidates.

```python
#!/usr/bin/env python3
# binary-recons.py
# Orchestration for bounded function reconstruction

import subprocess
import difflib
import json
from pathlib import Path

class BinaryReconstructor:
    def __init__(self, original_binary: Path, target_address: str):
        self.original = original_binary
        self.target_addr = target_address  # e.g., "0x0061c160"
        self.original_asm = self._extract_original_asm()
    
    def _extract_original_asm(self) -> str:
        """Extract assembly for target function from original binary."""
        result = subprocess.run(
            ["objdump", "-d", str(self.original)],
            capture_output=True, text=True
        )
        lines = result.stdout.split('\n')
        
        # Find start of function
        start_idx = None
        for i, line in enumerate(lines):
            if self.target_addr in line:
                start_idx = i
                break
        
        if start_idx is None:
            raise ValueError(f"Address {self.target_addr} not found")
        
        # Extract until next function
        end_idx = len(lines)
        for i in range(start_idx + 1, len(lines)):
            if re.match(r'^[0-9a-f]+\s+<[^>]+>:$', lines[i]):
                end_idx = i
                break
        
        return '\n'.join(lines[start_idx:end_idx])
    
    def try_candidate(self, c_source: str, compiler_flags: str = "-O2") -> dict:
        """
        Compile candidate C code and compare assembly.
        
        Returns:
            {
                "match_score": float (0-100),
                "candidate_asm": str,
                "diff": str,
                "status": "VERIFIED" | "FAILED"
            }
        """
        # Compile candidate
        with open("candidate.c", "w") as f:
            f.write(c_source)
        
        result = subprocess.run(
            ["gcc", "candidate.c", "-c", "-o", "candidate.o"] + compiler_flags.split(),
            capture_output=True
        )
        
        if result.returncode != 0:
            return {
                "match_score": 0,
                "status": "COMPILE_FAILED",
                "error": result.stderr.decode()
            }
        
        # Extract compiled assembly
        result = subprocess.run(
            ["objdump", "-d", "candidate.o"],
            capture_output=True, text=True
        )
        candidate_asm = result.stdout
        
        # Diff against original
        diff = list(difflib.unified_diff(
            self.original_asm.split('\n'),
            candidate_asm.split('\n'),
            lineterm=''
        ))
        
        # Score: lines matching / total lines
        total_lines = len(self.original_asm.split('\n'))
        diff_lines = len([l for l in diff if l.startswith(('+', '-'))])
        match_score = 100 * (1 - (diff_lines / (2 * total_lines)))
        
        status = "VERIFIED" if match_score >= 95 else "FAILED"
        
        return {
            "match_score": match_score,
            "candidate_asm": candidate_asm,
            "diff": '\n'.join(diff),
            "status": status
        }
    
    def log_result(self, function_name: str, result: dict):
        """Log result to FUNCTION_RECONSTRUCTIONS.md."""
        entry = f"""
## {function_name} ({self.target_addr})

**Match Score**: {result['match_score']:.1f}%
**Status**: {result['status']}

```diff
{result['diff'][:500]}  # First 500 chars of diff
```
"""
        with open("docs/FUNCTION_RECONSTRUCTIONS.md", "a") as f:
            f.write(entry)

# Main orchestration
if __name__ == "__main__":
    recon = BinaryReconstructor(
        original_binary=Path("replay.exe"),
        target_address="0x0061c160"
    )
    
    # Try candidate
    candidate = """
    void FUN_0061c160(char* scene_id, int flags) {
        actor_t* actor = get_current_actor();
        if (actor->child_count > 0) {
            error("Already has children");
            return;
        }
        resource_t* res = FUN_00502020(scene_id, 3);
        if (res->state == 1) {
            FUN_0061a750(actor, res);
        }
    }
    """
    
    result = recon.try_candidate(candidate)
    recon.log_result("FUN_0061c160", result)
    
    if result['status'] == "VERIFIED":
        print("✓ VERIFIED — Commit this candidate")
    else:
        print(f"✗ FAILED ({result['match_score']:.1f}%) — Try different approach or defer")
```

### Workflow Integration

1. **Generate candidate** (via local LLM, e.g., Qwen):
   ```bash
   ollama run qwen:7b <<EOF
   Target function at 0x0061c160 from Velvet Assassin replay.exe:
   [paste original assembly]
   
   Generate C code that compiles to identical assembly.
   Assume x86, cdecl calling convention.
   EOF
   ```

2. **Test candidate** (via `binary-recons`):
   ```bash
   python3 binary-recons.py \
       --original replay.exe \
       --address 0x0061c160 \
       --candidate candidate.c
   ```

3. **If VERIFIED**:
   ```bash
   git add docs/FUNCTION_RECONSTRUCTIONS.md
   git commit "Verify FUN_0061c160 (SetOutfit wrapper) — 97% assembly match"
   ```

4. **If FAILED**:
   ```bash
   # Log as DEFERRED, move to next function
   echo "FUN_0061c160: DEFERRED (match_score=42%, likely compiler-specific inlining)" \
     >> docs/DEFERRED_FUNCTIONS.md
   ```

---

## MEMORY PROTOCOL

### memory/MEMORY.md Structure

**Purpose**: Persistent fact store across sessions; prevent re-analysis of already-verified findings.

```markdown
# Verified Facts (Velvet Assassin)

## Architecture & Environment
- [VERIFIED] Architecture: x86, 32-bit, Little-Endian
- [VERIFIED] Compiler: MSVC (inferred from code patterns)
- [VERIFIED] Calling Convention: cdecl (push args R→L)
- [VERIFIED] Platform: Windows 32-bit

## Functions (Verified via Assembly Match)
- [VERIFIED 97%] FUN_0061c160 @ 0x0061c160: Scene loader
- [VERIFIED 95%] FUN_0060daa0 @ 0x0060daa0: LoadScene wrapper
- [DEFERRED] FUN_00502020 @ 0x00502020: Resource lookup (match_score=42%, unclear")

## Data Structures (Verified via Xref + Semantics)
- [VERIFIED] Outfit field: int at actor+0x80 (values 0/1/2 map to Campaign/Pilot/Nightdress)
- [VERIFIED] Actor has child_count at actor+0x14
- [HYPOTHESIZED] Resource has state field (inferred from assembly, not verified)

## Script Bytecode
- [VERIFIED] Player_Character::SetOutfit @ live3d.mir.bin + 0x0020ef0b
- [VERIFIED] Takes int outfit_id parameter (0/1/2)
- [VERIFIED] Modifies visible mesh groups (confirmed by in-game test)

## Cross-Verified Findings (Assembly + Semantics + In-Game Test)
- Outfit=0 → Campaign Violette (verified in-game, match config)
- Outfit=1 → Pilot Violette (verified in-game, match config)
- Outfit=2 → Nightdress (verified in-game, match config)
```

**Before Starting Any Task**:
```bash
# Read existing memory
cat memory/MEMORY.md

# Log new findings as they are verified
echo "- [VERIFIED 96%] FUN_0061a750 @ 0x0061a750: Actor hierarchy builder" >> memory/MEMORY.md
```

---

## OUTPUT DISCIPLINE

### Artifact Management

**Rule 1: Never Commit Raw Assets**
```bash
❌ git add replay.exe  # Too large, versioning bloat
❌ git add live3d.mir.bin  # Proprietary, binary, not source
✅ git add tools/binary-recons.py  # Automation script
✅ git add docs/FUNCTION_RECONSTRUCTIONS.md  # Analysis results
```

**Rule 2: Verify by Cryptographic Hash**
```bash
# Before/after verification
sha256sum replay.exe > CHECKSUMS.sha256
# Later:
sha256sum -c CHECKSUMS.sha256
# Output: replay.exe: OK
```

**Rule 3: Log ONLY Analysis Results, Not Binaries**
```markdown
## Verified Functions

| Address | Name | Size | Match | Status |
|---|---|---|---|---|
| 0x0061c160 | FUN_0061c160 | 120 | 97% | VERIFIED |
| 0x0060daa0 | FUN_0060daa0 | 50 | 95% | VERIFIED |
| 0x00502020 | FUN_00502020 | 200 | 42% | DEFERRED |
```

**Rule 4: Prefer Rust/Python for Orchestration**
```bash
✅ binary-recons.py (orchestration, language-agnostic)
✅ reconstruct.rs (Rust bindings to objdump, llvm-dis)
❌ Large C++ projects (hard to maintain, build dependencies)
```

---

## INTEGRATION WITH EXISTING RE_RULES_AND_VERIFICATION.md

**How these directives extend the earlier rules**:

| Earlier Rule | Enhanced By | Directive |
|---|---|---|
| RULE-R6: Runtime behavior verified by test | Directive 1: Use assembly as ground truth | Assembly proof + in-game test = 100% confidence |
| RULE-R12: Cross-verification required | Directive 3: Structural skepticism | Re-derive every finding independently |
| RULE-R14: Timestamps & authorship | MEMORY.md protocol | Log "verified at X date via assembly match" |

---

## EXAMPLE: APPLYING ALL DIRECTIVES

**Task**: Verify `Player_Character::SetOutfit` signature and calling convention.

### Step 1: Directive 1 — Assembly is Authority

```bash
# Extract assembly at target address
objdump -d replay.exe | sed -n '/0x0020ef0b:/,/^$/p' > setoutfit_original.asm

# Sample output:
# 0x0020ef0b: push ebp
# 0x0020ef0c: mov ebp, esp
# 0x0020ef0e: sub esp, 0x100
# 0x0020ef14: mov ecx, [ebp+0x8]    <- First argument
# 0x0020ef17: mov edx, [ebp+0xc]    <- Second argument
# 0x0020ef1a: test edx, edx
# 0x0020ef1c: jz 0x0020ef50          <- Branch on second arg
```

**Derived Signature**:
```c
void SetOutfit(int arg1, int arg2);
// OR, from xref analysis (see Step 3):
void SetOutfit(actor_t* self, int outfit_id);
```

### Step 2: Directive 2 — Bounded Reconstruction

Generate candidate for just this function:

```c
void SetOutfit(actor_t* self, int outfit_id) {
    if (outfit_id < 0 || outfit_id > 2) return;
    
    self->current_outfit = outfit_id;
    
    if (outfit_id == 0) {
        show_meshes(self, CAMPAIGN_MESHES);
    } else if (outfit_id == 1) {
        show_meshes(self, PILOT_MESHES);
    } else {
        show_meshes(self, NIGHTDRESS_MESHES);
    }
}
```

Compile and verify against original assembly.

### Step 3: Directive 3 — Structural Skepticism

Re-derive the signature by analyzing callers:

```bash
# Find all callers of 0x0020ef0b
codex search "call FUN_0020ef0b" replay.exe

# Expected pattern: each caller loads ECX (this pointer) and EDX (outfit_id)
# Verify this matches Directive 1 assembly
```

**Result**: Signature matches across Assembly (Step 1) + Callers (Step 3) → Confidence: HIGH

### Step 4: Log to memory/MEMORY.md

```markdown
- [VERIFIED 96%] Player_Character::SetOutfit @ 0x0020ef0b
  - Signature: void SetOutfit(actor_t* self, int outfit_id)
  - Assembly match: 96%
  - Xref consensus: 3/3 callers match calling convention
  - In-game test: Outfit values 0/1/2 confirmed
  - Status: CLOSED
```

---

## SUCCESS CRITERIA (This Directive Set)

✅ **Directive 1**: Every function verified via assembly diff ≥ 95%
✅ **Directive 2**: Each iteration focuses on ONE function by address
✅ **Directive 3**: No inherited assumptions; all re-derived
✅ **Tooling**: `binary-recons` automated, all results logged
✅ **Memory**: MEMORY.md kept current with VERIFIED vs DEFERRED status
✅ **Output**: Only code + analysis committed, no raw binaries

---

## NEXT TASK: Apply These Directives

**Immediate**: Verify Player_Character::SetOutfit using Directives 1-3

```markdown
## Task: Verify SetOutfit Function

**Objective**: Confirm SetOutfit signature and implementation via assembly match

**Directives Applied**:
- Directive 1: Assembly match ≥ 95%
- Directive 2: Single function, address 0x0020ef0b
- Directive 3: Re-derive signature from assembly + xrefs

**Harness**:
\`\`\`bash
# Extract and compile candidate
python3 binary-recons.py --address 0x0020ef0b --candidate setoutfit.c

# Expected result:
# Match Score: 95.0%
# Status: VERIFIED
\`\`\`

**Authorized Actions**:
- ✓ Analyze assembly
- ✓ Xref analysis
- ✓ Compile candidate source
- ✓ Log to memory/MEMORY.md

**Forbidden**:
- ✗ Modify game files
- ✗ Assume decompiler output is correct
- ✗ Skip assembly verification

**Proceed?**
```

---

**Status**: READY FOR USE
**Last Updated**: 2026-10-12
**Applies To**: All binary reconstruction work on Velvet Assassin
