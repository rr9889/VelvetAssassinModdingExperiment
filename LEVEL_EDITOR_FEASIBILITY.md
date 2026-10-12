# Velvet Assassin level editor feasibility

Status: exploratory research baseline, 2026-10-12. This is not an implementation plan based on a decoded file format. Do not treat the old `LEVEL_EDITOR_SPECIFICATION.md` pseudocode or sample headers as findings; it is now marked as an unvalidated vision document.

## Assessment

A read-only scene inventory/parser looks **feasible but not yet scoped to a known schema**. Existing notes identify native scene-resource and hierarchy-construction paths. A read-only check of the owned game installation has now fingerprinted `Levels\Lost1.scene` and verified the referenced string at `0x84ac`; the binary is not copied into the repository and no record grammar has been decoded. Static native evidence shows resource loading and recursive actor/node construction, but not the on-disk field layout.

A writable level editor is **premature and materially higher risk**. No serializer/save path for `.scene` has been identified. A visual editor would also need validated coordinate conventions, asset/mesh bindings, property metadata, and game-compatible reserialization. A safer long-term shape is a read-only parser and inspector first, then a lossless writer only after the source schema and round-trip requirements are established.


## Triage of the supplied Replay Engine brief

| Brief claim | Assessment | Why it matters to this project |
|---|---|---|
| Every level is compiled into one monolithic `.aio` containing scene graph, geometry, lighting, and triggers | **DISPROVED as stated / actual partition unresolved.** THE LOST Part 1 config names a separate `Levels\Lost1.scene`; that file exists in the owned install. `levelstructure.aio`, `levelstructure_demo.aio`, and `global.aio` begin with readable INI/comment text, not a single opaque archive header. Scene and config data may still reference packaged assets; the full packaging layout is unverified. | Do not start by designing an `.aio` binary archive parser. Start with the actual `.scene` sample and treat `.aio` as a family of extensions until file roles are established. |
| `.scene` placement/layout is a baked serialization model | **PARTLY SUPPORTED, schema unresolved.** Lost1 is 13,736,037 bytes and contains a length-prefixed-looking `Filename` property value at `0x84ac`; this is suggestive but one string cannot establish general record encoding. | Worth focused read-only format work. Preserve exact asset identity and compare multiple scenes before inferring fields. |
| `Cam1stPersonFOV` / `Cam3rdPersonFOV` are in `aio\player_exdata.cfg` | **CONFIRMED as file entries; runtime consumption not verified in this pass.** The installed text has comments “FOV in aim mode” / “FOV in 3rd person mode” and values 110 / 105 at lines 50–51. File SHA-256: `37274AC3916F5C08FDE379089F8DD9D7A4FE1E328FC69F9EF64A81B289977641`. | Useful camera/config lead, separate from `.scene` editor feasibility. It does not identify native camera fields or prove config reload behavior. |
| Launcher rename allows direct launch / fixes Steam overlay | **COMMUNITY-REPORTED, narrow scope.** PCGamingWiki says replacing Launcher.exe with Replay.exe makes the game start directly and can enable Steam overlay. It does not support the brief’s claims about bypassing argument interception, custom launchers, or DLL injection. No files were renamed. | Low value for scene parsing; avoid altering the install as part of this research. |
| LAA patch and single-core affinity are recommended stability fixes | **COMMUNITY/THIRD-PARTY REPORTS; not validated here.** A PCGamingWiki note says single-core affinity may reduce some crashes but crashes may remain. A third-party LAA patch claim is not validation for this exact executable. | Do not modify the executable or prescribe these as editor requirements. LAA would change the known executable identity and is outside this task. |
| `directional_blur.pfx` can be removed to disable motion blur | **COMMUNITY-REPORTED and locally plausible.** The file exists (93 bytes); PCGamingWiki describes renaming/deleting it. No change or runtime test performed. | Evidence of loose post-effect assets, not scene geometry or a serializer. |
| No Hints / Enhanced Edition prove outfit and gameplay tweaks | **COMMUNITY PRECEDENT CONFIRMED, mechanism must be stated precisely.** No Hints v2.7 describes edits to loose configs/effects, including `aio/levelstructure.aio` and `aio/player_exdata.cfg`, to keep Violette clothed in Morphine mode; it also changes weapon values and post effects. This is file-based modding evidence, not proof of memory modification or arbitrary scene editing. | Worth documenting as a safe/reversible modding precedent. It lowers the evidence bar for data-driven tweaks, not for writing `.scene` files. |
| C++, DirectX 9, SpeedTree, Bink, FMOD, PhysX and x86 describe the engine | **MIXED / mostly orthogonal.** x86 is supported by existing PE evidence. The legacy PhysX startup issue appears in community reports. The other exact middleware and renderer claims were not verified in this bounded pass. | These are not prerequisites for a read-only parser. Do not build a replacement engine or embed a renderer before decoding the data. |
| A WPF/Qt + DirectX/OpenGL bridge, octree, gizmos, raycasting and node editor are minimum editor requirements | **SPECULATIVE design choices.** None are schema facts or required for a CLI/tree inspector. | Defer UI and spatial acceleration until parsed object types, transforms, and mesh references are verified. |
| The first milestone is an `.aio` header parser | **DISPROVED as the best next step.** The observed `.aio` files are textual configuration; the actual level scene is `Lost1.scene`. | Revise roadmap to fingerprint scene samples and map loader structures first. |

The local file checks above were read-only. Lost1 identity: 13,736,037 bytes, SHA-256 `6522A087512C658462CDFF4E4B20087D46979F63077C059AEA8306562C14FCB7`. At file offset `0x84ac`, bytes decode as `\models\characters\violette.scene` preceded by `.` (`.\models\characters\violette.scene`); the string is followed by additional structured bytes. The exact property owner remains unknown. The FOV config identity and values are evidence of file contents only, not a traced native read.

## Evidence ledger

Confidence labels describe the repository evidence, not external certainty.

| Status | Finding | Exact locator / verification |
|---|---|---|
| CONFIRMED (static) | Lost1 names a model-scene placeholder `.\models\characters\violette.scene` in its bytes. Its owning property/node and semantic role are not decoded. | `FINDINGS.md`, file offset `0x000084ac`; newly corroborated by local sample LVE-012 (hash and bytes in `LEVEL_EDITOR_EVIDENCE.md`). Owning property/node remains unknown. |
| CONFIRMED (static) | Native loader dispatch recognizes `.scene` identifiers. | Extension string VA `0x007e3a0c`; reference `0x0060ff50` in `FUN_0060fe90`; `.scene` search/removal also at `FUN_004ade60` ref `0x004ae0bc` and `FUN_0052e5b0` refs `0x0052e5eb`, `0x0052e606`. Verify against matching replay.exe image base `0x00400000` and assembly xrefs. |
| CONFIRMED (static) | Script-visible `LoadScene`, `LoadSceneNoCheck`, `LoadSceneForceReload`, `LoadResource`, and `CreateChild` registrations exist. | Registration in `FUN_006140e0`; strings VAs `0x007f4798`, `0x007f4784`, `0x007f476c`, `0x007f475c`, `0x007f4750`; registrations `0x0061419a`, `0x006141a9`, `0x006141b8`, `0x006141c7`, `0x006141d6`. Implementations `FUN_0060daa0`, `FUN_0060dad0`, `FUN_0060db00`, `FUN_00610040`, `FUN_006113d0`. |
| CONFIRMED (static) | `FUN_0060d970` converts the supplied value's text and calls generic scene instantiation at `FUN_0061c160`; it does not select a level/editor file on its own. | Native VAs `0x0060d970`, `0x0061c160`; wrapper refs `0x0060dabc`, `0x0060daec`, `0x0060db1c`. See `FUNCTIONS.csv`; verify register/stack args in assembly. |
| CONFIRMED (static) | The generic scene loader requests a `SceneResource`, then calls hierarchy/metadata routines when the resource is ready. | `FUN_0061c160` -> `FUN_00502020(scene_id, 3)`; resource constructor `FUN_0053bc40`, type construction `FUN_00537bf0`, string `SceneResource` VA `0x007eabec`. Hierarchy construction `FUN_0061a750`; recursive entity initialization `FUN_0061aef0`. Verify native call graph/decompilation/assembly. |
| CONFIRMED (static) | `FUN_0061a750` copies node data including transforms, creates children, and recurses; `FUN_0061aef0` handles entity metadata and recursive initialization. | Native VAs `0x0061a750`, `0x0061aef0`, from existing `FINDINGS.md` / `FUNCTIONS.csv`. Exact structure offsets, transform representation, and file-to-runtime mapping are not recorded. |
| CONFIRMED (static strings; weak standalone evidence) | The executable contains `Scene data stored in .scene file:\n ` and `Aillegal .scene file.` strings. | String VAs `0x007ea40c`, `0x008278f3`; no analyzed xrefs in existing evidence. These do not by themselves prove an editor, a file format, or the error condition. |
| CONFIRMED (public report, secondary) | A 2015 Steam user guide describes reactivating “Developer Mode” via a Cheat Engine table, Numpad 1/2, and says a game-folder `readme.doc` lists commands. | [Steam community guide](https://steamcommunity.com/sharedfiles/filedetails/?id=398044891), section 1.1. It is user-authored, the guide is marked removed/incompatible, and its original Cheat Engine forum link is reported dead. Treat as a lead, not verified functionality. |
| CONFIRMED (public credits) | Velvet Assassin credits include Replay Studios tools programmers and level designers. | [Game manual PDF](https://cdn1.macgamestore.com/data/manuals/Velvet-Assassin-manual.pdf), credits p.31 (search-index excerpt). This establishes roles existed, not the name, existence, or recoverability of a level editor. |
| CONFIRMED scope-limited negative | No original Replay Studios level editor executable/source/project, original editor manual, or `.scene` save/serialization routine is documented in this repo or confirmed by the bounded public search. | Verify future artifact provenance and hashes; distinguish a runtime developer console from a content editor. This is not proof none existed. |

## Original tools and debug functionality

The best concrete lead is the reported Developer Mode cheat table and bundled `readme.doc` command list. It may expose useful runtime inspection, object listing, teleportation, camera control, or other commands; the guide does not quote the command list, so none of those capabilities are asserted. The historic table link redirects to a dead forum URL; a later commenter points to a Fearless Revolution mirror, which was inaccessible during this pass. Do not run an unverified table or use commands that mutate a save/game state as part of schema research.

Public searches for Replay Studios / Velvet Assassin editor tools did not establish an original level editor. The existence of tools programmers and level designers is unsurprising for a shipped game and is not proof of a distributable editor. The executable strings above are runtime diagnostics, not editor UI evidence. No `.scene` serializer is identified in `FUNCTIONS.csv` or the current scene findings.

## Minimum requirements

### Read-only parser / inspector

1. Obtain a legally owned, untouched copy of multiple representative `.scene` files; record exact path, size, SHA-256, source build/platform, and backups. Keep research inputs read-only.
2. Establish container framing before schema guesses: magic/version, byte order, lengths, alignment, compression, checksums, and resource tables. Require bounds checks and fail-closed behavior for unknown versions/records.
3. Correlate one deliberately simple scene and one complex level with `FUN_0061c160`, `FUN_0061a750`, and `FUN_0061aef0`. Identify file records separately from runtime objects; recover parent-child links, names/IDs, transforms, property typing, and references.
4. Preserve unknown bytes/records in the parse tree. Validate with independent examples and consistency checks; do not synthesize transforms from repeated float patterns alone.
5. Start with a CLI dump or tree/property inspector. A viewport is optional until coordinate, axis, unit, pivot, and transform composition conventions are verified.

### Writable editor

In addition to all read-only requirements: identify the actual engine writer or fully specify its inverse; preserve unknown data and stable IDs; prove deterministic parse→write→parse equality on untouched samples; test edits only on byte-for-byte copies; define reference/resource repair and version policy; and test staged outputs through fresh game loads with recovery copies. The editor must not write directly into the live game installation by default. Until these gates pass, keep any UI read-only.

## Feasibility decision

Proceed with the evidence roadmap's read-only acquisition and format-validation work. Defer framework selection, a 3D editor, scene mutation, and a writer. The native hierarchy loader makes a scene browser plausible once the file schema is recovered; it does not make serialization easy or safe.
