# Confirmed

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

- CONFIRMED global declaration `player`, record 0x2e46ec, flags 01 00 14 00, type 0x31, camera class ID 0x516, default ff ff ff ff. This narrows the previous Actor-valued ActivePlayer/PlayerRoot leads.
- CONFIRMED corrected HandlePlayerSwitches body at 0x33a9e7, 429 bytes: reads global operand 0x000a; gets member 0x87; compares Character method 0x803d result to global 0x0266; conditionally assigns that object to global 0x055f. LIKELY mapping to the typed global player/current controlled camera; exact symbolic global/type mappings and lifecycle clearing remain incomplete.
- Native host helper 0x004aafb0 returns 0x008ec408 (assembly Evidence `ev_320646b7eed27831e6ae670c00481cfec7645415abf836ea068901739c677633`). Host context is read through +4; native lookup helper 0x004c4030 uses context name-key slot +0xa8 then value slot +0x98 and returns the resulting VM value (`ev_bafd882e8920b81a13d116f63d32faa49c61007ed84719e20521ee184d423e1a`). It is an object lookup helper, not an outfit caller.
- Native reference-unwrapping evidence: checked REF<NAMESPACE> table 0x007de8cc slot +0x40 points to 0x004cdc00 (`ev_8e9e185a95c1b7e50c2f03251d553ab0fd493c95dbc0bef4ec7ee137edf729e0`). Assembly `MOV EAX,[ECX+0x18]; RET` confirms `Namespace* __thiscall Unwrap(RefNamespace*)` for that type (`ev_1121e79a4b3afc02238ea150293c75b6128f1008b55bb8f8ceec3744b9badb72`). Type/lifetime checks are essential before applying it to a fetched live value.
- LIKELY candidate: native host -> checked context -> existing lookup for narrow MirString name player -> checked reference unwrap -> camera script namespace -> ah namespace member -> camera outfit wrapper. No fully verified GetCurrentPlayer/GetCurrentCamera native routine, concrete type-validation API, or fixed ah byte offset can be supplied yet. NULL during menus/loading and pointer changes on level load are unknown; serialized defaults are not runtime observations.

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

Exact resume point: validate the typed global player/HandlePlayerSwitches mapping and the existing host-name-lookup/reference-unwrapper candidate, then resolve the existing camera wrapper/ChangeInitialOutfit completion call and safe thread. Do not restart FUN_005059e0, generic scene loading, input discovery, or broad VM investigation. Stop after this checkpoint; no native DLL contract is marked runtime-safe.

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
