# Confirmed

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
