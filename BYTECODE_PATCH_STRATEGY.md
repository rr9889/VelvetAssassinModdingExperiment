# Bytecode Patch Strategy: Runtime Outfit Swap

> SUPERSEDED PLAN: Current scope and checkpoint are [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md): VelvetSwap.dll, x86/Win32, F1-F5, no game edits in this pass. Bytecode insertion, compiler support, and CLI examples below are unimplemented proposals, not proved capabilities. SetOutfit has reported runtime mode semantics distinct from initial Campaign/Pilot/Nightdress IDs. Resolve its existing live object/VM invocation contract before choosing any patch strategy. Generate all files inside the repository.

## Overview

Target: `src\live3d.mir.bin` — compiled script bytecode containing `Player_Character::SetOutfit` and game logic.

Goal: Inject hotkey detection + outfit cycling without modifying the native `replay.exe`.

---

## Architecture

### Current State
- **Script VM Runtime**: Game loads `live3d.mir.bin` bytecode at startup
- **SetOutfit Function**: Located at file offset `0x0020ef0b` (1436 bytes)
- **Input System**: Likely has a frame-update loop that checks keyboard state
- **Compiled Script Format**: Length-prefixed function names + bytecode bodies

### Injection Strategy

1. **Intercept the frame-update loop** (likely called every game tick)
2. **Check for hotkey** (detect if user pressed designated key)
3. **Advance outfit state** (0 → 1 → 2 → 0)
4. **Call SetOutfit** with new outfit ID
5. **Clear hotkey flag** to prevent repeat triggers

---

## Files to Modify

### Primary Target
- **File**: `src\live3d.mir.bin`
- **Current SHA-256**: `404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099`
- **Size**: 3,459,973 bytes
- **Format**: Length-prefixed function records with bytecode bodies

### Key Functions to Hook/Extend

| Function | File Offset | Bytecode Size | Purpose |
|---|---|---|---|
| `Player_Character::SetOutfit` | `0x0020ef0b` | 1436 bytes | Changes outfit and visible mesh groups — **TARGET CALLEE** |
| `Player_Character::Init` | `0x002099e4` | 66 bytes | Character initialization — possible hook point |
| Frame update loop | TBD | TBD | Main game loop tick — **HOOK POINT** |

---

## Bytecode Modification Approach

### Step 1: Locate the frame-update entry point

Use **Codex CLI** to search `live3d.mir.bin` for:
- Function names containing "Update", "Tick", "Frame", "Process"
- References to keyboard/input state
- Character update loop patterns

```bash
# Example Codex search
codex search "Update" --file "src\live3d.mir.bin" --format script
codex search "Input" --file "src\live3d.mir.bin" --format script
codex search "Tick" --file "src\live3d.mir.bin" --format script
```

### Step 2: Understand the bytecode format

The script uses length-prefixed records:
```
[4-byte length] [function name (null-terminated UTF-16LE or ASCII)] [bytecode body]
```

Example from research:
- Function name offset: `0x0020eeeb`
- Bytecode offset: `0x0020ef0b`
- Bytecode size: 1436 bytes
- Next function starts at: `0x0020eeeb + 1436 = 0x0020f55f` (approx)

### Step 3: Extract bytecode for SetOutfit

**Goal**: Understand the calling convention and state management.

```bash
# Extract raw bytecode around SetOutfit
codex extract --file "src\live3d.mir.bin" --offset 0x0020ef0b --size 1436 --output SetOutfit.bin
codex decompile --file SetOutfit.bin --format script --output SetOutfit_decompiled.txt
```

Check for:
- Parameter count and types (likely: `self` pointer, `outfit_id` int)
- Mesh group visibility toggles (Campaign/Pilot/Nightdress conditionals)
- State update/commit operations

### Step 4: Inject hotkey detection + cycling logic

Create a new script function or patch an existing frame-update function:

**Pseudocode for injection point:**

```mir
function Update_OutfitCycler()
    // Every frame
    if IsKeyPressed(CTRL_Q):
        current_outfit = GetPlayerOutfit()
        next_outfit = (current_outfit + 1) % 3  // Cycle 0→1→2→0
        Player_Character::SetOutfit(next_outfit)
        ClearKeyPress(CTRL_Q)  // Prevent spam
```

**Bytecode equivalent**: Requires hand-assembly or a script compiler.

### Step 5: Hook into the main update loop

Find where the game calls frame updates and inject a call to the new function:

```mir
// In main game loop / frame update:
Call Update_OutfitCycler()
// ... existing frame logic ...
```

---

## Implementation Tools

### Codex CLI Integration
- **Purpose**: Inspect, extract, and decompile bytecode
- **Commands**:
  - `codex search` — Find function names and patterns
  - `codex extract` — Export bytecode regions
  - `codex decompile` — Reverse bytecode to readable form
  - `codex compile` — Reassemble modified script (if available)

### IDA/Ghidra Plugins (Alternative)
If Codex doesn't support script bytecode well:
- Load `live3d.mir.bin` as a binary in Ghidra
- Create manual annotations for function boundaries
- Export as pseudocode for analysis

### Python Script for Patching
Create a helper script to:
1. Read `live3d.mir.bin`
2. Locate function boundaries (length-prefixed headers)
3. Insert new bytecode at a safe offset
4. Recalculate length fields and references
5. Write patched binary

```python
# Pseudo-code structure
def patch_bytecode(filename, insertion_offset, new_bytecode):
    with open(filename, 'rb') as f:
        data = f.read()
    
    # Parse length-prefixed structure around insertion point
    length_field = struct.unpack('<I', data[insertion_offset:insertion_offset+4])[0]
    
    # Insert new bytecode
    patched = data[:insertion_offset] + new_bytecode + data[insertion_offset:]
    
    # Update length fields if necessary
    # ...
    
    with open(filename + '.patched', 'wb') as f:
        f.write(patched)
```

---

## Testing Plan

### Pre-Patch Verification
1. **Hash the original**: `sha256sum src\live3d.mir.bin`
2. **Backup**: Copy to `BACKUPS\live3d.mir.bin.original`
3. **Verify game still runs** with unmodified file

### Post-Patch Testing
1. **Load patched game**
2. **Start a level** (e.g., THE LOST Part 1)
3. **Press hotkey** (Ctrl+Q) during gameplay
4. **Verify outfit changes** (Pilot ↔ Campaign ↔ Nightdress ↔ Pilot)
5. **Check for crashes** or script errors

### Rollback Procedure
```bash
cp BACKUPS\live3d.mir.bin.original src\live3d.mir.bin
```

---

## Key Questions for Codex CLI

1. **Can Codex decompile script bytecode?**
   - What output format does it produce?
   - Does it support re-compilation?

2. **What is the exact bytecode instruction set?**
   - VM opcode reference documentation?
   - Function call convention (parameter passing, return values)?

3. **How are function references encoded?**
   - Absolute offsets? Function IDs? Symbol table?
   - Will patching change reference addresses?

4. **Input system integration:**
   - Where does keyboard state live?
   - What function reads it?
   - Can we hook it without native code?

---

## Next Steps

1. **Run Codex on `live3d.mir.bin`**
   ```bash
   codex analyze "src\live3d.mir.bin"
   codex search "Update\|Tick\|Input" --file "src\live3d.mir.bin"
   ```

2. **Extract SetOutfit bytecode** and understand its signature
   
3. **Locate frame-update entry point** where we can inject hotkey check

4. **Write/generate bytecode** for outfit cycling logic

5. **Patch the binary** and test in-game

---

## Risk Mitigation

- ✅ Always backup original files before patching
- ✅ Verify SHA-256 before and after patches
- ✅ Test in a fresh level load each time
- ✅ Document all offsets and modifications
- ✅ Keep rollback copy in BACKUPS directory
- ✅ Log all bytecode patches with before/after hex dumps

---

## References

- **FINDINGS.md**: Script function offsets and roles
- **FUNCTIONS.csv**: Complete function address table
- **CURRENT_STATE.md**: Verified test results (config-based outfit change)
