# Implementation Roadmap: Runtime Hotkey Outfit Swap

**Goal**: Press a hotkey during gameplay to cycle Violette's outfit (Campaign ↔ Pilot ↔ Nightdress) without reloading the level.

**Current State**: Configuration-based outfit selection confirmed working. Now: inject runtime hotkey detection and call `Player_Character::SetOutfit` mid-game.

**Timeline**: 6-8 steps, ~30 min per step if tools are ready.

---

## Phase 1: Preparation & Backup (Step 0)

### Step 0.1: Verify Current Working State
```bash
# Start THE LOST Part 1 with current settings
# Verify Outfit = 1 (Pilot) displays correctly
cd "C:\Users\Admin\Documents\Velvet Assassin"
# Run game, load level 1
# Expected: Pilot Violette
```

### Step 0.2: Create Full Backup Package
```bash
# Backup all game files before any patching
mkdir BACKUPS\pre-hotkey-mod-full
cp replay.exe BACKUPS\pre-hotkey-mod-full\
cp src\live3d.mir.bin BACKUPS\pre-hotkey-mod-full\
cp aio\levelstructure.aio BACKUPS\pre-hotkey-mod-full\
cp aio\player_exdata.cfg BACKUPS\pre-hotkey-mod-full\

# Hash backups
sha256sum BACKUPS\pre-hotkey-mod-full\* > BACKUPS\pre-hotkey-mod-full\SHA256.txt
```

### Step 0.3: Document Current Hashes
```bash
# Original file state
sha256sum replay.exe
# Expected: 9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132

sha256sum src\live3d.mir.bin
# Expected: 404f2905e3eb693745d40a4868d6068b784c46661487dbb6cf7cde27cf4ff099

# Save to research repo
echo "Pre-patch hashes:" >> BACKUPS\PATCH_LOG.txt
sha256sum replay.exe >> BACKUPS\PATCH_LOG.txt
sha256sum src\live3d.mir.bin >> BACKUPS\PATCH_LOG.txt
```

---

## Phase 2: Reverse-Engineer Input System (Step 1-2)

### Step 1: Locate Input Handler in replay.exe

**Goal**: Find where keyboard input is polled/processed.

**Tools**: Ghidra (open), Codex CLI

**Procedure**:
```bash
# Using Ghidra (already open):
# 1. Search for string references related to input/keyboard
#    - "WM_KEYDOWN", "GetAsyncKeyState", "keyboard", "input"
# 2. Look for Windows API calls:
#    - GetAsyncKeyState (Win32 keyboard state)
#    - GetMessageW / DispatchMessageW (message loop)
#    - SetWindowsHookEx (global keyboard hook)

# Using Codex CLI:
codex search "GetAsyncKeyState" replay.exe
codex search "keyboard" replay.exe
codex search "input" replay.exe
```

**Expected Result**: 
- Find a frame-update loop or event handler that checks keyboard state
- Likely near main game loop or input manager class
- Document the address (e.g., `0x00410000`)

### Step 2: Identify Safe Injection Point

**Goal**: Find where to hook into the input loop.

**Ghidra Analysis**:
1. Locate the frame-update function (from Step 1)
2. Identify the call stack:
   - Main loop → Update frame → Check input → Update game state
3. Find a location where:
   - We can detect hotkey press without breaking existing logic
   - ECX/EDX/EAX are not critical (for hotkey flag storage)
   - We can make a call to script function or SetOutfit

**Documentation**:
```
Input Handler Location: 0x00410000 (FUN_00410000)
Entry: ESI = input state, stack contains frame context
Safe hook point: 0x00410050 (after input state is loaded, before game state update)
Registers available: EDX (free), EAX (free after XOR)
```

---

## Phase 3: Instrument Game with Hotkey State (Step 3-4)

### Step 3: Add Global Hotkey Flag in Memory

**Goal**: Create a persistent memory location to track hotkey presses.

**Approach**:
1. Find unused data section in `replay.exe` (or create one)
2. Allocate 4 bytes for: `current_outfit` (int32)
3. Allocate 4 bytes for: `next_outfit_queued` (int32, boolean flag)
4. Allocate 4 bytes for: `hotkey_cooldown` (int32, frame counter to prevent spam)

**Ghidra Procedure**:
```
1. Right-click → Create Data → Dword at 0x008D0000 (example unused address)
2. Label: "g_CurrentOutfit"
3. Label: "g_NextOutfitQueued"
4. Label: "g_HotkeyCooldown"
5. Set initial values: 1, 0, 0 (outfit 1 = Pilot, no queue, no cooldown)
```

**Alternative**: Patch `replay.exe` to add a small data section.

### Step 4: Hook Input Loop to Detect Hotkey

**Goal**: Inject x86 code that detects hotkey and sets flag.

**x86 Patch (pseudocode)**:
```asm
; Hook point: 0x00410050 (after input state loaded)
; Hotkey: Ctrl+Q (0x51)

; Save registers
push eax
push ecx
push edx

; Check if Ctrl is pressed (GetAsyncKeyState for VK_CONTROL = 0x11)
mov eax, 0x11
call GetAsyncKeyState    ; Returns 0x8000 if pressed
test eax, 0x8000
jz not_ctrl

; Check if Q is pressed
mov eax, 0x51            ; 'Q' key
call GetAsyncKeyState
test eax, 0x8000
jz not_ctrl

; Ctrl+Q pressed! Set hotkey flag
mov eax, [0x008D0008]    ; Load g_HotkeyCooldown
test eax, eax            ; Check if cooldown active
jnz not_ctrl             ; Skip if on cooldown

mov dword [0x008D0004], 1    ; g_NextOutfitQueued = 1
mov dword [0x008D0008], 10   ; g_HotkeyCooldown = 10 (frames)

not_ctrl:
; Restore registers
pop edx
pop ecx
pop eax
; Continue game loop
```

**Implementation Tool**: 
- Use a binary patcher (custom Python script with `struct` module)
- Or use Ghidra's Python scripting console to inject ASM
- Or manually assemble with an x86 assembler and hex-edit `replay.exe`

**Documentation**:
```
Patch location: 0x00410050
Patch size: ~40 bytes
Memory addresses used: 0x008D0000-0x008D000C
Hotkey: Ctrl+Q
Cooldown: 10 frames (prevents key-repeat spam)
```

---

## Phase 4: Call SetOutfit from Script Layer (Step 5-6)

### Step 5: Analyze SetOutfit Signature in live3d.mir.bin

**Goal**: Understand how to call the script function from native code.

**Codex/Ghidra Analysis**:
```bash
# Extract SetOutfit bytecode
codex extract --file "src\live3d.mir.bin" --offset 0x0020ef0b --size 1436 --output SetOutfit.bin

# Decompile if Codex supports it
codex decompile --file SetOutfit.bin --output SetOutfit_decompiled.txt

# Manual inspection in Ghidra:
# 1. Load src\live3d.mir.bin in Ghidra
# 2. Go to offset 0x0020ef0b
# 3. Mark function boundary
# 4. Decompile → export pseudocode
```

**Expected Findings**:
- Parameter 0: `self` (Player_Character pointer)
- Parameter 1: `outfit_id` (int, 0/1/2)
- Return: void
- Side effects: Updates actor mesh visibility, changes outfit state

**Documentation**:
```
Function: Player_Character::SetOutfit
Location: src\live3d.mir.bin + 0x0020ef0b
Signature: void SetOutfit(int outfit_id)
Called through: Script VM adapter at 0x006140e0 or equivalent
Parameters: ECX = this, EDX or stack = outfit_id
```

### Step 6: Bridge Native Input to Script SetOutfit

**Goal**: Make the native input hook call the script outfit-swap function.

**Two Approaches**:

#### Approach A: Call SetOutfit Through Script VM Adapter (Preferred)
```
1. Native hook detects Ctrl+Q
2. Sets hotkey flag in memory
3. Script VM main loop checks flag
4. If set: call Player_Character::SetOutfit(next_outfit)
5. Clear flag
```

**Implementation**:
- Modify `src\live3d.mir.bin` bytecode to add:
  ```mir
  function Update_CheckHotkey()
      if ReadMemory(0x008D0004) == 1:
          current = GetActivePlayer().GetOutfit()
          next = (current + 1) % 3
          GetActivePlayer().SetOutfit(next)
          WriteMemory(0x008D0004, 0)  // Clear flag
  ```
- Hook into frame-update loop to call `Update_CheckHotkey()` every frame

#### Approach B: Direct Native Call (Advanced)
```
1. Native hook calculates next outfit directly
2. Saves to g_NextOutfitQueued
3. Injects a call to native wrapper → script adapter → SetOutfit
4. (Requires more complex calling convention knowledge)
```

**Recommendation**: Use Approach A (script-based polling of native flag).

---

## Phase 5: Patch live3d.mir.bin Bytecode (Step 7)

### Step 7: Inject Hotkey-Check Function into Script

**Goal**: Add or modify script bytecode to respond to hotkey flag.

**Procedure**:

1. **Find frame-update entry point**:
   ```bash
   codex search "Update\|Tick\|Frame" --file "src\live3d.mir.bin"
   # Likely candidates: PlayerUpdate, CharacterUpdate, GameUpdate, MainLoop
   ```

2. **Extract and analyze frame-update function**:
   ```bash
   # Assuming frame update at offset 0x001A0000 (example)
   codex extract --file "src\live3d.mir.bin" --offset 0x001A0000 --size 500 --output FrameUpdate.bin
   codex decompile --file FrameUpdate.bin --output FrameUpdate_decompiled.txt
   ```

3. **Create new hotkey-check bytecode**:
   ```mir
   // New function to inject
   function Player_Character::CheckHotkeyAndSwapOutfit()
       if g_NextOutfitQueued == 1:
           current_outfit = GetOutfit()
           next_outfit = (current_outfit + 1) % 3
           SetOutfit(next_outfit)
           g_NextOutfitQueued = 0
   ```

4. **Patch the binary**:
   - Option 1: Use Codex to recompile the script
   - Option 2: Hand-assemble bytecode and hex-patch `live3d.mir.bin`
   - Option 3: Write a Python bytecode patcher

5. **Insert hook into frame update**:
   - Find where `FrameUpdate()` is called
   - Add call to `CheckHotkeyAndSwapOutfit()` at the start
   - Recalculate offsets and length fields

**Python Patcher Template**:
```python
import struct

def patch_live3d(input_file, output_file):
    with open(input_file, 'rb') as f:
        data = bytearray(f.read())
    
    # Locate frame-update function (search for signature)
    frame_update_offset = data.find(b"FrameUpdate")
    if frame_update_offset == -1:
        print("Frame update not found")
        return
    
    # Read function header
    func_length_offset = frame_update_offset - 4
    func_length = struct.unpack('<I', data[func_length_offset:func_length_offset+4])[0]
    
    # Inject call to new hotkey function
    # (Exact bytecode depends on script VM instruction set)
    injection_offset = frame_update_offset + 20  # Adjust based on function structure
    
    # Insert hotkey-check call (pseudocode)
    hotkey_check_bytecode = b'\x01\x23\x45\x67'  # Placeholder
    data[injection_offset:injection_offset] = hotkey_check_bytecode
    
    # Update function length
    new_length = func_length + len(hotkey_check_bytecode)
    data[func_length_offset:func_length_offset+4] = struct.pack('<I', new_length)
    
    with open(output_file, 'wb') as f:
        f.write(data)
    
    print(f"Patched {output_file}")

patch_live3d("src\\live3d.mir.bin", "src\\live3d.mir.bin.patched")
```

**Testing Before Commit**:
```bash
# Verify file size and structure
sha256sum src\live3d.mir.bin.patched
ls -la src\live3d.mir.bin src\live3d.mir.bin.patched

# Backup original
cp src\live3d.mir.bin src\live3d.mir.bin.original

# Test with patched version
cp src\live3d.mir.bin.patched src\live3d.mir.bin
# Run game...
```

---

## Phase 6: Integration & Testing (Step 8)

### Step 8.1: Deploy and Test In-Game

1. **Apply all patches**:
   - `replay.exe` with input hook (Step 4)
   - `src\live3d.mir.bin` with hotkey script (Step 7)

2. **Verify file integrity**:
   ```bash
   sha256sum replay.exe
   sha256sum src\live3d.mir.bin
   ```

3. **Start game**:
   ```bash
   cd "C:\Users\Admin\Documents\Velvet Assassin"
   replay.exe
   ```

4. **Test sequence**:
   ```
   a) Load THE LOST Part 1 (Level 1)
      Expected: Pilot Violette (Outfit 1)
   
   b) Press Ctrl+Q during gameplay
      Expected: Outfit changes, animation plays, Violette appears as Campaign
      (Outfit 0 = different model)
   
   c) Press Ctrl+Q again
      Expected: Changes to Nightdress (Outfit 2)
   
   d) Press Ctrl+Q again
      Expected: Back to Pilot (Outfit 1)
   
   e) Hold Ctrl+Q for 1 second
      Expected: Only changes once (cooldown prevents spam)
   ```

5. **Check for crashes**:
   - Monitor game for crashes or anomalies
   - Check game logs if available
   - Verify mesh groups are rendering correctly

### Step 8.2: Validate Other Levels

1. **Test Level 5** (originally Campaign):
   ```
   Load level 5
   Expected: Campaign Violette appears
   Press Ctrl+Q → should cycle to Pilot, then Nightdress, then back to Campaign
   ```

2. **Test Level 11** (originally Nightdress):
   ```
   Load level 11
   Expected: Nightdress Violette appears
   Press Ctrl+Q → should cycle through all outfits
   ```

### Step 8.3: Performance Check

- Measure frame rate (should be unaffected)
- Check memory usage (should not grow over time)
- Verify no asset leaks when swapping repeatedly

---

## Phase 7: Documentation & Release (Step 9)

### Step 9.1: Document the Patch

Create `HOTKEY_MOD_DOCUMENTATION.md`:
```markdown
# Runtime Hotkey Outfit Swap Mod

## Installation
1. Backup your game directory
2. Copy patched files:
   - replay.exe (with input hook)
   - src\live3d.mir.bin (with hotkey script)

## Usage
- Press **Ctrl+Q** during gameplay to cycle outfits
- Outfit order: Pilot → Campaign → Nightdress → Pilot
- 10-frame cooldown prevents accidental double-presses

## Files Modified
- replay.exe (40 bytes added at 0x00410050)
- src\live3d.mir.bin (SetOutfit hook + frame-update integration)

## Rollback
cp BACKUPS\pre-hotkey-mod-full\* .
```

### Step 9.2: Commit to GitHub

```bash
git add HOTKEY_MOD_DOCUMENTATION.md IMPLEMENTATION_ROADMAP.md
git add BACKUPS/pre-hotkey-mod-full/
git commit -m "Add runtime hotkey outfit swap: Ctrl+Q cycles through outfits mid-game"
git push origin runtime-outfit-swap
```

### Step 9.3: Test & Release

Create a release package:
```
VelvetAssassin-HotkeyMod-v1.0/
├── README.md (installation instructions)
├── INSTALLATION_GUIDE.md
├── replay.exe (patched)
├── live3d.mir.bin (patched)
├── BACKUPS/
│   └── pre-hotkey-mod-full/
│       ├── replay.exe (original)
│       ├── live3d.mir.bin (original)
│       └── SHA256.txt
└── UNINSTALL.sh (restore originals)
```

---

## Critical Checkpoints

| Step | Success Criteria | Failure Action |
|---|---|---|
| 0 | Original game works, backups created | Restore from backup, abort |
| 1-2 | Input handler located, safe hook point identified | Continue with manual search, check Ghidra decompiler |
| 3-4 | Hotkey detected when Ctrl+Q pressed | Check Win32 API, verify GetAsyncKeyState address |
| 5-6 | SetOutfit signature understood | Decompile more carefully, may need symbol recovery |
| 7 | Script bytecode injects without corruption | Restore original live3d.mir.bin, try smaller patch |
| 8 | In-game hotkey works, outfit cycles correctly | Check if hotkey flag is being set, verify SetOutfit is called |
| 9 | Documentation complete, mod releasable | Test on another machine if possible |

---

## Tools Needed

- ✅ Ghidra 12.1.4 (open, has replay.exe loaded)
- ✅ Codex CLI (for bytecode search/extract)
- ✅ Python 3.x + `struct` module (for bytecode patching)
- ✅ Hex editor (010 Editor, HxD, etc. for manual verification)
- ✅ Git (for version control)

---

## Estimated Time per Phase

- Phase 1 (Backup): 5 min
- Phase 2 (Reverse-engineer input): 30-45 min
- Phase 3 (Add hotkey state): 15-20 min
- Phase 4 (Analyze SetOutfit): 20-30 min
- Phase 5 (Patch bytecode): 30-45 min
- Phase 6 (Integration & test): 20-30 min
- Phase 7 (Documentation): 10-15 min

**Total**: ~3-4 hours for first-time implementation.

---

## Contingency: Simpler Alternative

If bytecode patching proves too complex:

1. Use the **proven config-file approach** as the primary mod
2. Build a launcher that:
   - Detects hotkey press before game loads
   - Sets `aio\levelstructure.aio` outfit for the selected level
   - Launches game
3. This trades seamless in-game hotkey for a pre-game outfit selector (fast, proven)

---

## Next Action

**→ Start Phase 1, Step 0: Create backups and document current hashes.**

Once backups are secure, proceed to Phase 2: Locate the input handler in replay.exe using Ghidra.
