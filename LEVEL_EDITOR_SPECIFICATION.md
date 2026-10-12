# Level Editor for Velvet Assassin

## Vision

A **visual level editor** that allows users to:
- Load and edit existing levels (e.g., `Levels\Lost1.scene`)
- Place/move/delete actors, props, cameras, spawners
- Modify level properties (lighting, atmosphere, mission objectives)
- Export/test changes in-game
- Package custom levels

**Target Audience**: Modders, content creators, speedrunners

---

## Phase 1: Reconnaissance — Understand Level Format

### Goal
Map the structure of a `.scene` file and the supporting config ecosystem.

### Current Knowledge
From `FINDINGS.md`:
- **Level file**: `Levels\Lost1.scene` (contains actor/prop hierarchy)
- **Config**: `aio\levelstructure.aio` (outfit, mission setup)
- **Scene placeholder**: `violette.scene` reference at file offset `0x000084ac` in Lost1.scene
- **Supporting files**: 
  - `aio\player_exdata.cfg` (object definitions)
  - `Models\Characters\` (model assets)
  - `Levels\` (all scene files)

### Investigation Steps

#### Step 1.1: Analyze .scene File Format

**Tool**: Hex editor (010 Editor, HxD) + Codex CLI

```bash
# Extract first 1KB of Lost1.scene to understand structure
hexdump -C "Levels\Lost1.scene" | head -50

# Or use Codex to search for magic numbers/headers
codex search "SCENE" "Levels\Lost1.scene"
codex search -x "RIFF\|FORM\|BM" "Levels\Lost1.scene"  # Common file format headers
```

**Key Questions**:
1. Is it a binary format or text-based (JSON/XML)?
2. Does it have a header/magic number?
3. How are actors/props/hierarchy stored?
4. How are properties (position, rotation, scale, material) encoded?

#### Step 1.2: Reverse the .scene Binary Format

**Expected Format** (game engines typically use one of):
- **Unreal/Unity style**: Binary with tagged sections
  ```
  [Magic: 4 bytes] [Version: 4 bytes] [TOC offset: 4 bytes]
  [Actor count: 4 bytes]
  [Actor 0: name, transform, properties]
  [Actor 1: ...]
  ```

- **Custom format** (Steamworks/XNA-based):
  ```
  [Header] [Resource refs] [Actor list] [Hierarchy tree] [Metadata]
  ```

**Reverse Approach**:
1. Load `Lost1.scene` in Ghidra as binary
2. Scan for ASCII strings (actor names, property names)
3. Look for repeated patterns (transforms, offsets)
4. Cross-reference with `replay.exe` scene-loading functions

```bash
# Extract ASCII strings from scene file
strings "Levels\Lost1.scene" | head -30

# Look for position/rotation/scale patterns (floats = 0x3f800000 for 1.0)
hexdump -C "Levels\Lost1.scene" | grep "3f 80"
```

#### Step 1.3: Map Actor/Property References

Using Ghidra and previous findings:
- `FUN_0061aef0` processes scene entity metadata
- `FUN_0061a750` constructs actor hierarchy

**Document**:
```
Scene File Structure:
├─ Header (size, version)
├─ Resource list
│  ├─ model refs
│  ├─ material refs
│  └─ texture refs
├─ Actor hierarchy
│  ├─ Actor name (string)
│  ├─ Transform (position, rotation, scale)
│  ├─ Properties (color, shader, collision)
│  └─ Children (recursive)
└─ Metadata (lighting, fog, physics)
```

---

## Phase 2: Build the Editor Foundation

### Goal
Create a basic editor that can:
1. Load a `.scene` file
2. Display it visually (3D viewport)
3. Parse and display actor properties
4. Save changes back to disk

### Step 2.1: Choose Editor Framework

**Options**:

| Framework | Pros | Cons | Effort |
|---|---|---|---|
| **Unreal Engine 5** | Full 3D editor, plugins, asset pipeline | Overkill for modding, licensing | 3-4 weeks |
| **Unity + Editor Extensions** | Scene import, C# scripting, inspector | Requires runtime for testing | 2-3 weeks |
| **Qt (C++)** | Native performance, cross-platform | Must write 3D viewport | 3-4 weeks |
| **Godot 4.x** | Open-source, GDScript, built-in editor | Smaller community | 2 weeks |
| **Three.js / Babylon.js (Web)** | Web-based, no install, fast iteration | Harder to save/export | 1-2 weeks |

**Recommendation**: **Godot 4.x or Three.js**
- Godot: Better for shipped tool, full control
- Three.js: Faster prototype, web-based sharing

### Step 2.2: Implement Scene Loader

**Core Module**: `VelvetSceneLoader`

```csharp
// Pseudocode structure
public class VelvetScene
{
    public string Name { get; set; }
    public List<Actor> Actors { get; set; }
    public Vector3 LightPosition { get; set; }
    public Color AmbientLight { get; set; }
    
    public static VelvetScene Load(string path)
    {
        byte[] data = File.ReadAllBytes(path);
        // Parse binary format
        // Return scene object
    }
    
    public void Save(string path)
    {
        // Serialize back to binary
        byte[] data = Serialize();
        File.WriteAllBytes(path, data);
    }
}

public class Actor
{
    public string Name { get; set; }
    public Vector3 Position { get; set; }
    public Quaternion Rotation { get; set; }
    public Vector3 Scale { get; set; }
    public string ModelPath { get; set; }
    public Dictionary<string, object> Properties { get; set; }
    public List<Actor> Children { get; set; }
}
```

### Step 2.3: Create 3D Viewport

Use the editor framework's 3D rendering:
- **Godot**: Use `CSGBox3D`, `MeshInstance3D` nodes
- **Three.js**: Use `THREE.Mesh`, `THREE.Group` for hierarchy
- **Unity**: Use `GameObject` prefabs

**Features**:
- Load models from `Models\Characters\`, `Models\Props\`, etc.
- Render actor hierarchy as a tree
- Gizmos for transform (move, rotate, scale)
- Camera controls (orbit, pan, zoom)

```python
# Godot pseudocode
extends Node3D

var scene: VelvetScene
var actor_nodes: Dictionary = {}

func load_scene(path: String) -> void:
    scene = VelvetSceneLoader.load(path)
    
    for actor in scene.actors:
        var node = create_actor_node(actor)
        add_child(node)
        actor_nodes[actor.name] = node

func create_actor_node(actor: Actor) -> Node3D:
    var node = Node3D.new()
    node.name = actor.name
    node.position = actor.position
    node.rotation = actor.rotation
    
    # Load mesh
    if actor.model_path:
        var mesh = load_model(actor.model_path)
        var mesh_instance = MeshInstance3D.new()
        mesh_instance.mesh = mesh
        node.add_child(mesh_instance)
    
    # Recursively add children
    for child in actor.children:
        node.add_child(create_actor_node(child))
    
    return node

func save_scene(path: String) -> void:
    scene.save(path)
```

---

## Phase 3: Editor Features

### Step 3.1: Property Inspector

Display and edit actor properties:
- Position (X, Y, Z sliders)
- Rotation (Euler angles or quaternion)
- Scale
- Model path (dropdown from available assets)
- Custom properties (shader, color, physics)

```
┌─────────────────────────────┐
│ Actor: "Violette"           │
├─────────────────────────────┤
│ Transform:                  │
│  Position: [0.5] [1.2] [0]  │
│  Rotation: [0] [90] [0]     │
│  Scale: [1] [1] [1]         │
├─────────────────────────────┤
│ Model:                      │
│  [▼ Violette_Campaign.scene]│
├─────────────────────────────┤
│ Properties:                 │
│  Hidden: [ ] (checkbox)     │
│  AI Type: [▼ Guard]         │
│  Speed: [1.5]               │
├─────────────────────────────┤
│ [Delete] [Duplicate]        │
└─────────────────────────────┘
```

### Step 3.2: Asset Browser

Browse available models and props:
```
Models/
├─ Characters/
│  ├─ Violette_Campaign.scene ✓
│  ├─ Violette_Pilot.scene ✓
│  ├─ Violette_Nightdress.scene ✓
│  └─ Guard_01.scene
├─ Props/
│  ├─ Chair.scene
│  ├─ Door.scene
│  └─ Window.scene
└─ Vehicles/
   └─ Car.scene
```

Allow drag-and-drop to place actors in the level.

### Step 3.3: Hierarchy Tree

Display actor parent-child relationships:
```
┌─ Lost1 (Root)
├─ Lighting
│  ├─ Sunlight
│  └─ Ambient
├─ Actors
│  ├─ Violette
│  ├─ Guard_01
│  ├─ Guard_02
│  └─ Civilian_01
└─ Geometry
   ├─ BuildingA
   ├─ BuildingB
   └─ StreetGeometry
```

Right-click to add/delete/duplicate actors.

### Step 3.4: Gizmo Tools

Transform actors visually:
- **Move (W)**: Drag to translate
- **Rotate (E)**: Circular gizmo for rotation
- **Scale (R)**: Handle to scale uniformly
- **Snap to grid (V)**: Optional grid snapping

```
    Y ↑
      │   / Z (into screen)
      │  /
      │ /
  ────+───→ X
```

---

## Phase 4: Import/Export & Testing

### Step 4.1: Test in Game

1. **Save edited level** to `Levels\Lost1.scene`
2. **Launch game**: `replay.exe`
3. **Load level**: Game loads modified scene
4. **Verify**: Actors appear at new positions, properties apply

**Hot-reload workflow** (advanced):
- Editor watches game process
- Detects level unload/reload
- Auto-saves and reloads level
- Minimal iteration loop

### Step 4.2: Package Custom Levels

Allow export/sharing of custom levels:

```
MyLevel.mod
├─ level.scene (edited level file)
├─ metadata.json (name, author, version)
├─ preview.png (screenshot)
└─ custom_assets/
   ├─ models/
   └─ textures/
```

Install mod:
```bash
./velvet-assassin-mod-installer.exe MyLevel.mod
# Copies to Levels\, registers in levelstructure.aio
```

### Step 4.3: Undo/Redo System

```csharp
public class UndoRedo
{
    private Stack<Action> undoStack = new();
    private Stack<Action> redoStack = new();
    
    public void Record(Action change, Action undo)
    {
        change.Invoke();
        undoStack.Push(undo);
        redoStack.Clear();
    }
    
    public void Undo()
    {
        if (undoStack.Count > 0)
        {
            var undo = undoStack.Pop();
            undo.Invoke();
            redoStack.Push(/* inverse of undo */);
        }
    }
}
```

---

## Phase 5: Advanced Features (Optional)

### Step 5.1: Behavior Scripting

Hook into the script bytecode layer:
- Define actor behavior (patrol, idle, chase)
- Trigger events (on actor death, player proximity)
- Custom dialogue/cutscenes

**Editor UI**:
```
┌─────────────────────┐
│ Behavior: Guard_01  │
├─────────────────────┤
│ [ ] Patrol          │
│   Waypoints: [+]    │
│   │ Waypoint 1: (5,0,2)
│   │ Waypoint 2: (5,0,10)
│   └─ Speed: [1.5]   │
│ [ ] Alert on player │
│   Distance: [15]    │
│   Behavior: Attack  │
└─────────────────────┘
```

### Step 5.2: Pathfinding Visualizer

Show AI patrol routes and sight lines:
```
Guard_01 patrol path:
  ↓ (red line)
  └─ Waypoint 1
     ↓
  └─ Waypoint 2
     ↓
  └─ Return to start

Sight radius: (yellow circle, 20m)
```

### Step 5.3: Physics Editor

Configure collision, gravity, physics properties for custom objects.

### Step 5.4: Lighting Editor

Tweak light positions, colors, intensities:
```
Sunlight:
  Position: [100, 50, -100]
  Color: [RGB sliders]
  Intensity: [1.0]
  Direction: (visual arrow)
```

---

## Technical Stack Recommendation

### Core Editor
- **Language**: C# (Unity) or GDScript (Godot)
- **Framework**: Godot 4.x
  - Built-in 3D editing
  - GDScript for scripting
  - Open-source, no licensing
  - Active community

### Scene Loader
- **Parser**: Custom C# or Rust DLL
- **Input**: Binary `.scene` files from game
- **Output**: JSON intermediate format for editor

### Asset Pipeline
- **Model import**: FBX/GLTF from game assets
- **Texture loading**: PNG/DDS from `Models\`
- **Audio**: WAV from `Audio\`

### Distribution
- **Package**: Standalone executable
- **Platforms**: Windows (primary), Linux/Mac (stretch)
- **Installation**: Portable (no install) or MSI installer

---

## Development Timeline

| Phase | Task | Duration | Milestone |
|---|---|---|---|
| 1 | Reverse-engineer .scene format | 1-2 weeks | Scene structure doc |
| 2 | Build basic editor + loader | 2-3 weeks | Load/display level |
| 3 | Add editing features (gizmos, properties) | 2 weeks | Edit + save cycle |
| 4 | Test in-game, export/package | 1 week | Playable custom level |
| 5 | Polish + advanced features | 2-4 weeks | Feature-complete editor |
| 6 | Documentation + release | 1 week | Shipped to community |

**Total: 9-14 weeks (2-3 months) for MVP; 4-5 months for full feature set.**

---

## Immediate Next Steps

### Step 1: Document .scene Binary Format (This Week)

1. **Load `Levels\Lost1.scene` in hex editor**
   ```bash
   hexdump -C "Levels\Lost1.scene" | head -100
   ```

2. **Search for strings and patterns**
   ```bash
   strings "Levels\Lost1.scene"
   ```

3. **Cross-reference with Ghidra**
   - Find `FUN_0061aef0` (scene entity parser)
   - Decompile to understand structure
   - Document field offsets

4. **Write a binary parser** (Python prototype)
   ```python
   import struct
   
   def parse_scene(filename):
       with open(filename, 'rb') as f:
           data = f.read()
       
       # Read header
       magic = data[0:4]
       version = struct.unpack('<I', data[4:8])[0]
       actor_count = struct.unpack('<I', data[8:12])[0]
       
       print(f"Magic: {magic}")
       print(f"Version: {version}")
       print(f"Actor count: {actor_count}")
       
       # Parse actors...
   ```

5. **Commit documentation**
   ```bash
   git add SCENE_FORMAT_ANALYSIS.md
   git commit "Document Velvet Assassin .scene binary format"
   ```

### Step 2: Choose Editor Framework (Week 2)

- [ ] Install Godot 4.x
- [ ] Create test project
- [ ] Implement basic scene loader
- [ ] Render a cube in 3D viewport
- [ ] Test save/load cycle

### Step 3: Prototype Scene Loading (Week 3)

- [ ] Parse `Lost1.scene` into Godot
- [ ] Display actor hierarchy
- [ ] Render models
- [ ] Test in-game reload

---

## Success Criteria

✅ **Level Editor MVP**:
1. Load existing level from disk
2. Display actors in 3D viewport
3. Edit actor positions/rotations
4. Save changes
5. Reload in game and verify

✅ **Full Feature Set**:
1. All of above +
2. Add/delete/duplicate actors
3. Asset browser + drag-and-drop
4. Property inspector
5. Undo/redo
6. Package & share custom levels

---

## Questions to Answer First

Before diving into Phase 2:

1. **What is the `.scene` file format?** (binary, text, compressed?)
2. **Can you extract/decompile one successfully?**
3. **Do you have all game assets available locally?** (models, textures)
4. **What scripting/behavior system should custom levels support?**
5. **Do you want single-player campaigns only, or multiplayer support?**

---

## Resources

- **Godot 4.x**: https://docs.godotengine.org/
- **Binary file analysis**: https://en.wikipedia.org/wiki/Hex_editor
- **3D graphics primer**: LearnOpenGL.com
- **Game modding communities**: r/gamedev, r/gamemods, GameBanana.com

---

## Next Action

**→ This week: Reverse-engineer the .scene format using hex editor + Ghidra.**

Once you understand the structure, we can build the editor with full confidence.

Ready to start Phase 1?
