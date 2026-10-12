# Level editor evidence register

Research state as of 2026-10-12. Sources are the existing `level-editor-spec` branch notes and a small, targeted public-source check. This file records findings and limits, not a decoded `.scene` specification.

## Local repository evidence

| ID | Status | Artifact / locator | Finding | Verification and limit |
|---|---|---|---|---|
| LVE-001 | CONFIRMED (historical report; details superseded by LVE-012) | `FINDINGS.md`, `Levels\Lost1.scene` file offset `0x000084ac` | Bytes contain `.\models\characters\violette.scene`. | See LVE-012 for exact local sample identity and excerpt. Owner node/property unknown. |
| LVE-002 | CONFIRMED (static report) | replay.exe VA `0x007e3a0c`; xrefs `0x004ae0bc`, `0x0052e5eb`, `0x0052e606`, `0x0060ff50` | `.scene` extension participates in extension handling and loader dispatch. | Match executable hash/base; inspect assembly at xrefs. Not a file-format definition. |
| LVE-003 | CONFIRMED (static report) | `FUN_0060fe90` VA `0x0060fe90`, call site `0x0060ffa8` | `.scene` identifier is routed to `FUN_0060daa0`; caller paths include `FUN_00610040` and `FUN_006113d0`. | Existing `FINDINGS.md` and `FUNCTIONS.csv`; verify call arguments. Generic resource dispatch. |
| LVE-004 | CONFIRMED (static report) | `FUN_006140e0` VA `0x006140e0`; registrations `0x0061419a`, `0x006141a9`, `0x006141b8`, `0x006141c7`, `0x006141d6`, `0x0061414c` | Registers LoadScene, LoadSceneNoCheck, LoadSceneForceReload, LoadResource, CreateChild, CreateChilds. | Existing function table. No editor-specific registration shown. |
| LVE-005 | CONFIRMED (static report) | `FUN_0060daa0` `0x0060daa0`; `FUN_0060dad0` `0x0060dad0`; `FUN_0060db00` `0x0060db00`; shared `FUN_0060d970` `0x0060d970` | Script wrappers forward supplied identifier and flags; `FUN_0060d970` converts text then calls `FUN_0061c160`. | Assembly evidence IDs in `FINDINGS.md`: `ev_e92b48c91f0562b05b12e3ddcd6f3ab3718455b4fcca704614e3db41a460efa9`, `ev_4d7dd8754a5f5d8b2dccac0366a9d7249f47f44692df913eb04b4d1ef573b4d0`. These wrappers do not prove serialization. |
| LVE-006 | CONFIRMED (static report) | `FUN_0061c160` VA `0x0061c160`; call from `FUN_0060d970`; calls `FUN_00502020`, `FUN_0061a750`, `FUN_0061aef0` | Generic scene resource load and instantiation; rejects actors with existing children, has recursion check, and on ready resource invokes hierarchy construction/metadata initialization. | Confirm by assembly and call graph. No exact on-disk schema or editor functionality follows. |
| LVE-007 | CONFIRMED (static report; incomplete layouts) | `FUN_0061a750` VA `0x0061a750`; recursive self-call; `FUN_0061aef0` VA `0x0061aef0` | First attaches resource, copies node data/transforms, creates children and recurses; second processes entity metadata/child initialization recursively. | `FUNCTIONS.csv`; exact source structure offsets, transform encoding, and parent-reference serialization not recorded. |
| LVE-008 | CONFIRMED (string only; xrefs unverified) | replay.exe string VA `0x007ea40c` | `Scene data stored in .scene file:\n ` | Existing string search reports no analyzed xrefs. Could be diagnostic/log text; not evidence of editor or serialization routine. |
| LVE-009 | CONFIRMED (string only; xrefs unverified) | replay.exe string VA `0x008278f3` | `Aillegal .scene file.` | No analyzed xrefs. Error wording suggests validation exists, but exact checks are unknown. |
| LVE-010 | CONFIRMED (existing repo file inventory) | `LEVEL_EDITOR_SPECIFICATION.md` | Contains a proposed visual editor, example C#/Godot pseudocode, assumed headers and actor fields. | These are design hypotheses only. Header/version/actor count/transform fields are not evidenced; spec now carries a warning. |
| LVE-011 | UNRESOLVED | Repo `FUNCTIONS.csv`, `FINDINGS.md`, `LEVEL_EDITOR_SPECIFICATION.md`; present pass | No `.scene` save/serialization function or original editor binary/source/manual established by existing notes. | Search was intentionally bounded to scene/editor research artifacts; not proof of nonexistence. |
| LVE-012 | CONFIRMED (read-only local artifact) | `C:\Users\Admin\Documents\Velvet Assassin\Levels\Lost1.scene`; size 13,736,037; SHA-256 `6522A087512C658462CDFF4E4B20087D46979F63077C059AEA8306562C14FCB7` | At `0x84ac`: `.\models\characters\violette.scene`, then a structured-looking `ResetTM` property neighborhood. | Hash and bytes read from the owned install on 2026-10-12. No parser/schema decoded; no game file modified or copied into repo. |
| LVE-013 | CONFIRMED (read-only local config contents) | `aio\player_exdata.cfg`, lines 50–51; SHA-256 `37274AC3916F5C08FDE379089F8DD9D7A4FE1E328FC69F9EF64A81B289977641` | FOV config names/comments/value: `Cam1stPersonFOV=110`, `Cam3rdPersonFOV=105`. | Confirms text entries only. Native consumer, precedence, and runtime reload are unverified. |
| LVE-014 | CONFIRMED (read-only sampled text) | `aio\global.aio`, `aio\levelstructure.aio`, `aio\levelstructure_demo.aio`; file prefixes | These files begin with INI sections/comments and readable settings. | Three sampled `.aio` configuration files, not a claim about every file using the extension. Contradicts the brief’s blanket monolithic archive description. |
| LVE-015 | CONFIRMED existence; community-reported behavior | `FX\PostFX\directional_blur.pfx`, 93 bytes; [PCGamingWiki](https://www.pcgamingwiki.com/wiki/Velvet_Assassin) | File exists; community guide says removing/renaming disables motion blur. | No change or runtime test. Not scene-format evidence. |

## Public original-tool/debug leads

| ID | Status | Source | Evidence | Limit / follow-up |
|---|---|---|---|---|
| LVE-WEB-001 | CONFIRMED as a public claim; not independently runtime-verified | [Steam community mod guide, section 1.1](https://steamcommunity.com/sharedfiles/filedetails/?id=398044891) | Says Velvet Assassin Developer Mode can be enabled with a Cheat Engine table; Numpad 1 enables, Numpad 2 disables; says game-folder `readme.doc` contains the commands. | User-authored guide, currently marked removed/incompatible. Original Cheat Engine forum URL is dead; later comment points to [Fearless Revolution thread 4125](https://fearlessrevolution.com/viewtopic.php?t=4125), inaccessible in this pass. Commands and table not verified. A developer/debug command mode is not a level editor. |
| LVE-WEB-002 | CONFIRMED as credit listing | [Velvet Assassin manual PDF](https://cdn1.macgamestore.com/data/manuals/Velvet-Assassin-manual.pdf), credits page 31 (search excerpt) | Lists Replay Studios tools programmers and lead level designer/level designers. | Establishes staff roles only. Does not establish tool names or a shipped editor. |
| LVE-WEB-003 | CONFIRMED as a technical talk listing | [GDC Vault session](https://www.gdcvault.com/play/1012013/Building-a-Dynamic-Lighting-Engine) | “Building a Dynamic Lighting Engine for VELVET ASSASSIN,” Christian Schuler, Replay Studios, GDC Europe 2009. | Could inform engine rendering/light concepts if materials are accessible; not evidence of scene editor or serializer. |


## Supplied intelligence brief: claim triage

- **DISPROVED as stated:** “one monolithic `.aio` contains the scene graph.” The installed level config points to a separate `Levels\Lost1.scene`; `global.aio`, `levelstructure.aio`, and `levelstructure_demo.aio` sampled here begin as readable text configuration. This does not settle whether some referenced resources are inside other packages.
- **CONFIRMED file contents, unresolved runtime binding:** `aio\player_exdata.cfg` lines 50–51 contain `Cam1stPersonFOV = 110` (“FOV in aim mode”) and `Cam3rdPersonFOV = 105` (“FOV in 3rd person mode”). It is a worthwhile camera/config lead, not evidence of native object layout or live reload.
- **CONFIRMED local sample:** `Levels\Lost1.scene`, 13,736,037 bytes, SHA-256 `6522A087512C658462CDFF4E4B20087D46979F63077C059AEA8306562C14FCB7`. At offset `0x84ac`: `.\models\characters\violette.scene`, followed by bytes including `03 00 00 00 07 00 00 00 "ResetTM"...`. This confirms a structured-looking property/value neighborhood, not the general schema. No raw scene file was copied into the repository.
- **COMMUNITY-REPORTED, narrow:** PCGamingWiki documents the launcher swap as a direct-launch / Steam-overlay workaround; it does not establish custom argument forwarding or better DLL injection. No rename was performed.
- **COMMUNITY-REPORTED:** PCGamingWiki documents changing/removing `directional_blur.pfx`; the local file exists at 93 bytes. No deletion or runtime test was performed.
- **COMMUNITY MOD PRECEDENT:** the [No Hints v2.7 author thread](https://www.systemshock.org/index.php?topic=3501.0&wap2=) itemizes loose config/effect changes, including `aio/levelstructure.aio` and `aio/player_exdata.cfg`, for the clothed Morphine-mode behavior. The [PCGamingWiki summary](https://www.pcgamingwiki.com/wiki/Velvet_Assassin) also describes No Hints and Enhanced Edition tweaks. This supports data-driven file modding, not runtime-memory writes or `.scene` editing.
- **UNVERIFIED / orthogonal:** monolithic “baked `.aio`” serialization, user-profile/root partition enforcement, exact SpeedTree/Bink/FMOD stack, launcher argument interception and `-windowed`/`-nospeedtree`, LAA need, octree/gizmo requirements, and any original scene writer. Do not treat these as project facts.

The FOV file SHA-256 is `37274AC3916F5C08FDE379089F8DD9D7A4FE1E328FC69F9EF64A81B289977641`. Both hashes identify the files as read in the owned installation on 2026-10-12; they are not global guarantees for every release/platform.

## Unresolved questions

1. What are the exact `.scene` container/header/version/compression/endianness rules?
2. Which structures encode nodes, hierarchy, transforms, properties, resources, and stable IDs? Are values absolute/local or composed at load time?
3. Does the engine have a save/export path, and is it present in the retail binary or only internal tools?
4. What is the provenance and actual command set of the Developer Mode table and `readme.doc`?
5. Did Replay Studios distribute an editor to licensees/internal staff only, and are binaries/docs archived elsewhere?
6. Which scene asset formats/rendering dependencies are necessary to show geometry in an external viewport?

## Existing evidence IDs preserved

The source repository’s function/assembly evidence IDs for loader and adapter behavior remain in `FINDINGS.md` and `FUNCTIONS.csv`, including `ev_e92b48c91f0562b05b12e3ddcd6f3ab3718455b4fcca704614e3db41a460efa9`, `ev_4d7dd8754a5f5d8b2dccac0366a9d7249f47f44692df913eb04b4d1ef573b4d0`, `ev_124abf9b253d189385bd86cbc8a465f83aa3a5f85b146d81094d2a111da10c24`, `ev_5da49d0c0b2f23314bcbcd1d9d533e7ee285d5879a6fc986ad589d3bf64aa1d5`, `ev_f6c731c8aab82c72994da1e83c8bffdb4d64dd999781f7d3a6c15ce139e92925`. The research register includes LVE-001 through LVE-015 and LVE-WEB-001 through LVE-WEB-003; no raw executable or scene captures were added to the repository.
