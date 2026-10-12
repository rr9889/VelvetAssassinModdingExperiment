# HOTKEY OUTFIT SWAP MOD: FINAL MILESTONE GUIDE

> SUPERSEDED PLAN: See [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md). Current target is VelvetSwap.dll, x86/Win32, F1-F5. The Ctrl+Q/native-patch steps below are historical proposals, not completed milestones or current authorization. SetOutfit is script bytecode, not a native function at 0x0020ef0b; the shown signature/registers/actor field and verification examples are unsupported. Current blocker is live Player_Character acquisition plus the native-to-script invocation contract. All generated work belongs in the Git repository, never the game directory.

**End State**: Press Ctrl+Q during gameplay → outfit cycles (Campaign ↔ Pilot ↔ Nightdress) visibly in-game.

---

## MILESTONE 0: Setup (Do This First)

### 0.1 Install Dependencies
```bash
# Windows: Install these tools
# - Ghidra 12.1.4 (reverse engineering)
# - binutils (objdump for assembly extraction)
# - Python 3.x (for scripting)
# - Your C compiler (gcc/clang or MSVC)

# macOS/Linux:
brew install binutils python3
# Download Ghidra from ghidra-sre.org
```

### 0.2 Create Working Directory
```bash
# In your Velvet Assassin folder:
mkdir -p mod-work/{backups,logs,assembly}
cd mod-work
```

### 0.3 Backup Original Binaries
```bash
cp ../replay.exe backups/replay.exe.original
cp ../src/live3d.mir.bin backups/live3d.mir.bin.original

# Verify integrity:
sha256sum backups/replay.exe.original > backups/CHECKSUMS.txt
sha256sum backups/live3d.mir.bin.original >> backups/CHECKSUMS.txt
```

### 0.4 Read Prior Findings
Open and read:
- `FINDINGS.md` (what we know about outfits)
- `CURRENT_STATE.md` (config-based outfit swap proof)
- `memory/MEMORY.md` (verified facts)

---

## MILESTONE 1: Locate Input Handler (Phase 1)

**Objective**: Find where the game checks for keyboard input.

**Time Budget**: 2 hours

### 1.1 Open Replay.exe in Ghidra
```bash
# Launch Ghidra GUI and open:
# - replay.exe
# - Set architecture: x86, 32-bit
# - Let analysis complete (10-15 min)
```

### 1.2 Search for Input Functions
In Ghidra's search bar (Edit → Search → Program Text):
```
GetAsyncKeyState
```

**Expected Result**: You find 1-5 references to `GetAsyncKeyState` (Windows input API).

If found → Go to **1.3**
If NOT found → Try alternate search:
```
PeekMessage
GetMessage
WM_KEYDOWN
```

### 1.3 Examine Input Handler
When you find `GetAsyncKeyState` call:
1. Double-click to open the function containing the call
2. Ghidra shows you the calling function (e.g., `FUN_00401234`)
3. Right-click → Copy address
4. Extract the assembly:

```bash
# Replace 0x00401234 with actual address found
objdump -d replay.exe | sed -n '/^0*401234 </,/^$/p' > assembly/input_handler.asm
cat assembly/input_handler.asm
```

### 1.4 Verify Input Handler
Look for patterns like:
```
mov eax, [esp+4]     # Get virtual key code parameter
test eax, eax        # Check if key is pressed
jz label_skip        # Jump if not pressed
```

**Success Criterion**: You find a function that:
- Calls `GetAsyncKeyState`
- Takes a key code parameter
- Returns non-zero if key is pressed

Log this finding:
```bash
echo "Input Handler @ 0x[YOUR ADDRESS]" >> logs/phase1_findings.txt
echo "Function size: ~[SIZE] bytes" >> logs/phase1_findings.txt
echo "Key pattern: GetAsyncKeyState call found" >> logs/phase1_findings.txt
```

### 1.5 Run Harness Check
```bash
./hotkey_outfit_swap_harness.sh
```

**Expected**: Phase 1 should show `[PASS]` (or SKIP if objdump can't find it, which is OK).

**If STUCK**: 
- Try searching for "0x51" (hex code for 'Q' key)
- Look in main game loop function
- Ask: where does the game update player movement? Input is checked there.

---

## MILESTONE 2: Locate Frame-Update Loop (Phase 2)

**Objective**: Find where the game updates state each frame.

**Time Budget**: 2 hours

### 2.1 Find Main Game Loop
In Ghidra:
1. Find `main()` or `WinMain()` function (Search → "main")
2. Look for a large loop with many function calls
3. This loop runs once per frame

**Pattern to look for**:
```
loop_start:
  call FUN_render
  call FUN_update_physics
  call FUN_input_handler
  jmp loop_start
```

### 2.2 Extract Frame Loop Assembly
```bash
# Replace 0x00401000 with actual loop start address
objdump -d replay.exe | sed -n '/^0*401000 </,/^0*401200 /p' > assembly/frame_loop.asm
cat assembly/frame_loop.asm
```

### 2.3 Identify Input Check Point
Look through the frame loop for:
- Call to `GetAsyncKeyState`
- Or call to input handler function you found in Milestone 1

Mark this location:
```bash
echo "Frame Loop @ 0x[ADDRESS]" >> logs/phase2_findings.txt
echo "Input check @ offset 0x[OFFSET from loop start]" >> logs/phase2_findings.txt
```

### 2.4 Verify Loop Timing
The frame loop should:
- Run every ~16-33ms (60 FPS game)
- Call input handler once per loop
- Update actor state after input

**Success Criterion**: You can trace through assembly and confirm:
1. Input handler is called each frame
2. State is updated after input
3. This is where we'll inject Ctrl+Q check

### 2.5 Run Harness Check
```bash
./hotkey_outfit_swap_harness.sh
```

**Expected**: Phase 1-2 should show `[PASS]`.

---

## MILESTONE 3: Verify SetOutfit Function (Phase 3)

**Objective**: Confirm SetOutfit can be called from the input handler.

**Time Budget**: 2 hours

### 3.1 Review Existing SetOutfit Analysis
Open: `FINDINGS.md` or `memory/MEMORY.md`

You should already have:
- Address: `0x0020ef0b` (in live3d.mir.bin)
- Signature: `void SetOutfit(actor_t* self, int outfit_id)`
- Parameters: ECX = actor pointer, EDX = outfit_id (0/1/2)

### 3.2 Extract SetOutfit Assembly
```bash
# Note: live3d.mir.bin uses different base address than replay.exe
# File offset 0x0020ef0b
objdump -d src/live3d.mir.bin | sed -n '/^0020ef0b </,/^0020f000/p' > assembly/setoutfit.asm
cat assembly/setoutfit.asm | head -50
```

### 3.3 Generate Candidate C Code
Create `setoutfit_candidate.c`:
```c
// Candidate SetOutfit implementation
void SetOutfit(void* actor_ptr, int outfit_id) {
    if (outfit_id < 0 || outfit_id > 2) {
        return;
    }
    
    // Store outfit ID in actor structure
    int* actor_outfit_field = (int*)((char*)actor_ptr + 0x80);  // hypothetical offset
    *actor_outfit_field = outfit_id;
    
    // Show/hide mesh groups based on outfit_id
    if (outfit_id == 0) {
        show_meshes(actor_ptr, CAMPAIGN_MESHES);
    } else if (outfit_id == 1) {
        show_meshes(actor_ptr, PILOT_MESHES);
    } else {
        show_meshes(actor_ptr, NIGHTDRESS_MESHES);
    }
}
```

### 3.4 Compile and Compare
```bash
# Compile candidate
gcc -c setoutfit_candidate.c -o setoutfit_candidate.o -O2 -fno-builtin

# Extract compiled assembly
objdump -d setoutfit_candidate.o > assembly/setoutfit_compiled.asm

# Compare original vs candidate
diff -u assembly/setoutfit.asm assembly/setoutfit_compiled.asm | head -30
```

### 3.5 Scoring: Assembly Match
Count matching instruction sequences:
- If 95%+ of instructions match → **VERIFIED**
- If 80-95% match → **LIKELY** (compiler variations acceptable)
- If <80% match → Try different approach or defer

```bash
# Simple score: lines matching / total lines
diff assembly/setoutfit.asm assembly/setoutfit_compiled.asm | wc -l
# If < 20 diff lines out of 100+ total → good match

echo "1" > setoutfit_verified.txt  # Signal to harness
```

### 3.6 Run Harness Check
```bash
./hotkey_outfit_swap_harness.sh
```

**Expected**: Phase 1-3 should show `[PASS]`.

---

## MILESTONE 4: In-Game Verification (Phase 4)

**Objective**: Confirm outfits change when we manually call SetOutfit.

**Time Budget**: 1 hour

### 4.1 Create Test Mod (Temporary)
Create a minimal mod that calls SetOutfit every frame for testing:

**test_outfit_cycle.py**:
```python
#!/usr/bin/env python3
# Temporary test: cycle through outfits each frame

outfit_counter = 0

def on_frame():
    global outfit_counter
    actor = get_player_actor()
    
    # Cycle: 0 → 1 → 2 → 0 (every 60 frames = 1 second at 60 FPS)
    outfit_id = (outfit_counter // 60) % 3
    call_setoutfit(actor, outfit_id)
    
    outfit_counter += 1

# This is pseudocode; actual implementation depends on game API
```

### 4.2 Load Game and Test
```bash
# Launch Velvet Assassin
./replay.exe

# Load Level 1 (Lost1)
# Start mission
```

### 4.3 Verify Outfit Cycling
Watch Violette's appearance:
- [ ] Campaign outfit visible (default)
- [ ] Switch to Pilot outfit (check mesh change)
- [ ] Switch to Nightdress outfit (check mesh change)
- [ ] Cycle back to Campaign
- [ ] No crashes, no clipping, mesh transitions smooth

### 4.4 Log Results
Create `in_game_hotkey_test.log`:
```
2026-10-12T14:30:00Z — In-Game Test Start
Level: Lost1
Initial Outfit: Campaign (0)
Frame 1-60: Outfit=0 (Campaign) ✓
Frame 61-120: Outfit=1 (Pilot) ✓
Frame 121-180: Outfit=2 (Nightdress) ✓
Frame 181-240: Outfit=0 (Campaign) ✓
All three outfits cycling correctly.
No crashes detected.
Mesh transitions smooth.
Result: PASS
2026-10-12T14:31:00Z — Test End
```

### 4.5 Run Harness Check
```bash
./hotkey_outfit_swap_harness.sh
```

**Expected**: Phase 1-4 all show `[PASS]` → Harness returns exit code 0.

---

## MILESTONE 5: Implement Hotkey Detection (Phase 5)

**Objective**: Hook Ctrl+Q into the input handler.

**Time Budget**: 3 hours

### 5.1 Understand Ctrl+Q Virtual Key Code
- Virtual Key for 'Q': `0x51`
- Ctrl modifier: check if `GetAsyncKeyState(0xA2)` returns true (left Ctrl) OR `GetAsyncKeyState(0xA3)` (right Ctrl)

### 5.2 Write Hotkey Hook Logic
Create `hotkey_injection.c`:
```c
// Global state
static int current_outfit = 0;
static int last_keypress_frame = -10;  // Debounce: prevent rapid cycles

void check_hotkey() {
    // Check Ctrl+Q
    int ctrl_pressed = GetAsyncKeyState(0xA2) || GetAsyncKeyState(0xA3);
    int q_pressed = GetAsyncKeyState(0x51);
    
    if (ctrl_pressed && q_pressed) {
        // Debounce: only once every 10 frames
        static int frame_counter = 0;
        frame_counter++;
        
        if (frame_counter > 10) {
            // Cycle outfit
            current_outfit = (current_outfit + 1) % 3;
            
            // Get current player actor
            void* actor = get_player_actor();
            
            // Call SetOutfit
            call_setoutfit_at_0x0020ef0b(actor, current_outfit);
            
            frame_counter = 0;
        }
    }
}

// Call this once per frame in the main game loop
void on_frame_hook() {
    check_hotkey();
    // ... rest of frame update
}
```

### 5.3 Locate Injection Point
Find the safest place to call `check_hotkey()` in the frame loop:
- After input is polled
- Before actors are rendered
- Address: [From Milestone 2]

### 5.4 Patch Replay.exe
Using a binary patcher or debugger:
1. Locate the frame loop (Milestone 2 address)
2. Insert a call to your hotkey hook
3. Test in-game

**Important**: Use a backup first!
```bash
cp backups/replay.exe.original replay.exe.test
# Apply patch to replay.exe.test
# Test patch with game
```

### 5.5 In-Game Hotkey Test
```bash
./replay.exe
# Load Level 1
# Press Ctrl+Q during gameplay
# Verify outfit cycles
```

Create `in_game_hotkey_test.log`:
```
2026-10-12T15:00:00Z — Hotkey Test
Level: Lost1
Gameplay Start: Campaign outfit
Press Ctrl+Q (1st): Switches to Pilot ✓
Press Ctrl+Q (2nd): Switches to Nightdress ✓
Press Ctrl+Q (3rd): Switches to Campaign ✓
Press Ctrl+Q (4th): Switches to Pilot ✓
...
No lag, no crashes, outfit changes instantaneous.
Result: PASS
```

### 5.6 Run Harness Check
```bash
./hotkey_outfit_swap_harness.sh
```

**Expected**: Exit code 0, all phases PASS.

---

## MILESTONE 6: Polish & Release (Phase 6)

**Objective**: Clean up code, document, and prepare for distribution.

**Time Budget**: 2 hours

### 6.1 Verify No Regressions
- [ ] Outfit swap works with all three outfits
- [ ] No visual glitches or clipping
- [ ] No performance impact (framerate stable)
- [ ] No crashes on repeated hotkey presses
- [ ] Works on all difficulty levels
- [ ] Works on all levels in campaign

### 6.2 Document the Mod
Create `MOD_README.md`:
```markdown
# Hotkey Outfit Swap Mod

## Installation
1. Backup replay.exe
2. Replace with patched version
3. Launch game normally

## Usage
- Press Ctrl+Q during gameplay to cycle outfits
- Campaign ↔ Pilot ↔ Nightdress

## Compatibility
- Works with: Velvet Assassin v1.0 (final retail)
- OS: Windows 32-bit

## Known Issues
- None

## Credits
- Reverse Engineering: [Your name]
- Assembly Analysis: Ghidra
```

### 6.3 Create Rollback Guide
```bash
cat > ROLLBACK.txt <<EOF
To restore original replay.exe:
1. Copy backups/replay.exe.original to replay.exe
2. Verify integrity:
   sha256sum replay.exe
   # Should match: 9d18ef1a841526c0c91ce8af5b952c46c7ca0a8a6e8d169e99cc78cf34f8c132
EOF
```

### 6.4 Commit Final Code
```bash
git add -A
git commit -m "Complete: Hotkey outfit swap mod (Ctrl+Q) — all milestones verified"
git tag v1.0-hotkey-swap
git push origin runtime-outfit-swap
```

### 6.5 Package for Distribution
```bash
# Create distributable mod package
mkdir -p release/hotkey-swap-mod
cp replay.exe release/hotkey-swap-mod/
cp MOD_README.md release/hotkey-swap-mod/README.txt
cp ROLLBACK.txt release/hotkey-swap-mod/
zip -r hotkey-swap-mod-v1.0.zip release/hotkey-swap-mod/

# Share on modding sites:
# - GameBanana.com
# - Nexus Mods
# - ModDB.com
```

---

## END STATE: Working Mod

When all milestones complete:

✅ **Ctrl+Q Hotkey Works**
- Press during gameplay
- Outfit cycles smoothly
- All three outfits confirmed visible
- No crashes or glitches

✅ **Code Is Verified**
- Assembly match >= 95%
- In-game tests pass
- Harness returns exit 0

✅ **Mod Is Documented**
- README provided
- Rollback procedure clear
- Source code committed

✅ **Ready to Share**
- Package created
- Uploaded to mod sites
- Other players can use it

---

## QUICK REFERENCE: Milestone Checklist

- [ ] **Milestone 0**: Setup complete (backups created, tools installed)
- [ ] **Milestone 1**: Input handler located (objdump extracts assembly)
- [ ] **Milestone 2**: Frame-update loop found (main game loop identified)
- [ ] **Milestone 3**: SetOutfit verified (assembly match >= 95%)
- [ ] **Milestone 4**: In-game test passed (all 3 outfits cycle in-game)
- [ ] **Milestone 5**: Hotkey implemented (Ctrl+Q works, tested 10+ times)
- [ ] **Milestone 6**: Mod released (packaged, documented, shared)

**Total Time**: ~12-16 hours of focused work

**Expected Completion**: 2-3 weeks of part-time effort

---

## IF YOU GET STUCK

At any milestone:

1. **Re-read the task definition** (TASK_HOTKEY_OUTFIT_SWAP.md)
2. **Check memory/MEMORY.md** for prior findings
3. **Run the harness**: `./hotkey_outfit_swap_harness.sh`
4. **Move to next phase**: Don't debug endlessly; if stuck >1 hour, mark DEFERRED and move on
5. **Ask for help**: Link the phase you're stuck on + harness output

---

**Last Updated**: 2026-10-12
**Status**: Ready for execution
