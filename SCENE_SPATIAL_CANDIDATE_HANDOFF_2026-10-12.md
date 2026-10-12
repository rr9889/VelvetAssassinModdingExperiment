# Lost1.scene spatial-field investigation handoff

Date: 2026-10-12  
Scope: read-only assessment of candidate spatial values in `Levels\Lost1.scene`.  
Purpose: preserve evidence and confidence boundaries for a later independent audit.  

## Executive status

The bytes 28 bytes before several framed `NODE` records contain plausible 3-float sequences. Repeated model instances have different values there, which is consistent with per-instance spatial data. This is **not yet proof** that the values are world-space XYZ positions. No independent in-game/editor coordinate was available for comparison, and the XY plot did not show a clearly recognizable level layout.

Do not call the extracted values verified coordinates. Treat them as candidate vectors pending independent validation.

## Evidence provenance and limits

- The scene file was previously read at `C:\Users\Admin\Documents\Velvet Assassin\Levels\Lost1.scene`; the file was not intentionally modified.
- Prior-session notes report a file size of 13,736,037 bytes and 968 tag-7 `NODE` records.
- The reported extraction checked a candidate `<3f` little-endian vector at `NODE - 28`, filtering for finite values in `[-500, 500]` and excluding nonzero denormalized values. It reported 438 plausible vectors: 224 nonzero and 214 all-zero.
- A prior plot was generated as `level_layout_plot.png` using `plot_level_layout.py`, with XY, XZ, and YZ projections. Its XY view clustered many actors at the origin and did not establish a recognizable map. A point cloud is not independent semantic validation.
- Files previously reported as generated: `spatial_actors.json`, `extract_spatial_actors.py`, `plot_level_layout.png`, `plot_level_layout.py`, and `diagnose_nodes.py`. Their present contents/existence were **not rechecked in this session**.
- On 2026-10-12, two read-only PowerShell inspection attempts failed before process startup with `helper_unknown_error: setup refresh had errors`. No new local reads or computations were completed in this session. The numeric results below are preserved from the immediately preceding investigation summary and require reproduction.

## Binary framing currently reported

Candidate `NODE` framing observed in prior analysis:

```text
ASCII "NODE" | 4 zero bytes | u32 little-endian tag | u32 little-endian name length | name bytes
```

The high-level actor subset was called `node_tag: 7` in the prior task. Validate this tag interpretation from independent records/parser code before relying on it.

The candidate vector is read as three little-endian IEEE-754 single-precision floats beginning at `NODE - 0x1c` (28 bytes before the marker). A distinct triple begins at `NODE - 0x10` in the cited examples. Neither field’s semantic type has been established. Nearby bytes may contain lengths, flags, bounds, or other values; byte proximity is not a schema proof.

## Repeated-model comparison (reported, needs raw-byte reproduction)

These three named nodes reportedly reference the same model path but have different candidate triples:

| Node | `NODE` offset | Candidate triple at `NODE - 0x1c` | Same referenced model |
|---|---:|---:|---|
| `NPC_Bridge_Guard` | `0xEE9CC` | `(0, 0, 195.96875)` | `\\models\\characters\\ss_sturmmann1.scene` (path was reported with leading `.` and `\` in the source) |
| `NPC_Drunken` | `0x22882D` | `(0, 0, 100)` | `\\models\\characters\\ss_sturmmann1.scene` |
| `NPC_Bunker_01` | `0xB202AE` | `(0, 0, 146.203125)` | `\\models\\characters\\ss_sturmmann1.scene` |

Interpretation: **LIKELY / supportive, not conclusive.** Different values attached to instances of a shared model argue against the triple being only an invariant model property. They do not distinguish position from per-instance bounds, radius, animation state, or another node parameter. Reproduce by reading exact record windows and proving the filename association boundaries.

Additional reported examples:

| Node | `NODE` offset | Candidate at `NODE - 0x1c` | Nearby triple at `NODE - 0x10` |
|---|---:|---:|---:|
| `PlayerStart` | `0x8407` | `(0, 0, 119.265625)` | `(511.984375, 511.984375, 511.984375)` |
| `Bunker_Top` | `0xE108D` | `(34.25, 129, 0)` | `(2047.9375, 2047.9375, 2047.9375)` |
| `Ground_End3` | `0x8616` | `(0, 0, 0.959931)` | `(1.09, 400, 2867.2)` |
| `NPC_Camp_2` | not retained | `(0, 0, 0)` | reported all-zero |
| `NPC_Bridge_Patrol` | not retained | `(0, 0, 0)` | reported all-zero |

These values need byte-level reproduction. In particular, `Ground_End3` shows the adjacent `NODE - 0x10` triple is not consistently a uniform scale/bounds field; that interpretation must remain **SPECULATIVE**.

Reported axis statistics for the 438 accepted vectors (also require reproduction):

| Axis | zero count | nonzero count | distinct values | min | max |
|---|---:|---:|---:|---:|---:|
| X | 339 | 99 | 82 | -199.8984 | 481.875 |
| Y | 312 | 126 | 101 | -178.15625 | 446.96875 |
| Z | 234 | 204 | 111 | -199.7734 | 478.09375 |

## Confidence classification

- **CONFIRMED by prior local scan, not reproduced in the current turn:** the reported file has repeated framed `NODE` records and candidate finite float triples at a consistent relative offset.
- **CONFIRMED as a report of observed data, but not independently verified here:** the three listed `ss_sturmmann1.scene` instances have different candidate triples.
- **LIKELY:** the candidate field is associated with an instance/node rather than solely with the shared model asset.
- **SPECULATIVE:** the field represents a translation/position vector; the three components map to X/Y/Z; the coordinates are world-space; the level's up axis; the `NODE - 0x10` values are scale or bounds.
- **DISPROVED as an inference standard:** a coherent-looking 2D scatter plot by itself would prove coordinate extraction. It would be suggestive only, especially without known landmarks and a verified axis mapping.
- **UNRESOLVED:** exact field start/width in the true serialized schema, coordinate frame, axis convention, unit scale, node-tag semantics, and whether values are transformed by parent nodes.

## What would constitute critical proof

Use one of these independent paths, ideally both:

1. **Runtime landmark comparison:** obtain a read-only position for a uniquely identified actor/landmark in a live level (debug UI, supported console, existing observation instrumentation, or a safe documented read). Match that actor to its scene node, account for axis/unit mapping, and compare within a stated tolerance. Do not infer identity from pointer stability alone.
2. **Loader/parser data-flow proof:** inspect the narrow scene deserialization/instantiation code path and establish exactly which serialized bytes are copied/decoded into the engine transform translation fields. Verify with at least one record and its runtime transform. A node-construction routine alone is insufficient unless it exposes the mapping from file offset to transform components.

Then validate a second landmark, preferably with nonzero values on multiple axes. This guards against accidental agreement on an axis-aligned `(0,0,z)` example.

## Reproduction and audit checklist for the next session

1. Verify target file path, size, and SHA-256; record them in the audit. Do not write to game data.
2. Re-run the scanner and print every tag-7 record count, framing validation rule, rejected-record reason, and the exact byte window around the three repeated-model examples.
3. Confirm how `Filename` belongs to a node (record/property boundaries), rather than associating it merely because it occurs within an arbitrary forward window.
4. Decode candidate floats from explicitly stated offsets (`NODE-0x20` through `NODE-0x04`) and print hex plus decoded values for the named examples. Check endianness, alignment, bounds, NaNs, infinities, signed zero, and duplicate/overlapping hits.
5. Establish whether `NODE-0x1c` is the beginning of a 12-byte field structurally, not merely a convenient float window. Look for the same field boundary across node kinds and compare zero/nonzero distributions.
6. Obtain an independent coordinate or narrow parser data-flow proof. Until then, keep JSON field names such as `candidate_xyz` or `candidate_vector`, never `world_position`.
7. Replot only after the coordinate semantics and axes are supported. Use named landmarks and report projections/axis conversion explicitly.

## Safety and scope

This work is read-only. Do not patch `replay.exe`, modify `.scene` assets, write runtime game state, or use an outfit-changing path. The current blocker is evidentiary, not permission to broaden into unrelated engine reverse engineering.

