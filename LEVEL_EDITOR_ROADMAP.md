# Velvet Assassin level-editor research roadmap

Order is dependency-driven; priority reflects impact on deciding whether an editor is possible. This is a bounded research roadmap, not authorization to alter game files or build a complete editor.

| Rank | Work item | Impact / feasibility | Depends on | Deliverable and stop gate |
|---:|---|---|---|---|
| 1 | Acquire and fingerprint representative `.scene` inputs from the owned game installation (Lost1 plus a small character scene and one other level). | Very high / high | Read-only access to installed assets | Record path, size, SHA-256, build/source, and byte-preserving copies. Stop if inputs are missing; do not infer format from strings alone. |
| 2 | Verify loading/hierarchy evidence against the exact target image. | High / high | Existing `FINDINGS.md`, `FUNCTIONS.csv`, target hash | Recheck `FUN_0061c160` (VA `0x0061c160`), `FUN_0061a750` (`0x0061a750`), `FUN_0061aef0` (`0x0061aef0`), and `.scene` xrefs listed in feasibility doc. Export assembly/decompilation evidence with target hash and call-site addresses. Identify precise node/transform structures only where directly supported. |
| 3 | Determine container framing and compression/versioning. | Very high / medium | Rank 1 samples | Hex/entropy/offset analysis plus loader path confirmation. Produce a grammar for header and resource/record boundaries with all unknown bytes marked. No parser code yet. |
| 4 | Correlate serialized records to runtime construction. | Very high / medium-low | Ranks 2–3 | Trace one minimal object through `FUN_0061a750` and metadata through `FUN_0061aef0`; document exact transforms, parent edges, IDs, and property types with evidence. Stop if file-to-runtime relationship remains ambiguous. |
| 5 | Find original developer command docs/table and look for shipped tool artifacts. | Medium-high / medium | Inventory of installation/distribution files; public lead | Preserve source, version, provenance, and hashes of any `readme.doc`, `.CT`, editor executable/project, or asset pipeline files. Classify commands by read-only vs state-changing. A debug console alone does not satisfy editor evidence. |
| 6 | Define read-only parser acceptance criteria and only then implement a small parser. | High / medium | Ranks 1–4 | Parse multiple sample files, report hierarchy/references/transforms, preserve unknown records, reject malformed files safely. Deliver a text/JSON tree first; no writes. |
| 7 | Evaluate writable round-trip path. | High / low until proven | Successful parser and original writer evidence | Find serialization/write routine or prove complete inverse schema. Byte-identical round-trip untouched samples, stable IDs, opaque data preservation, and isolated copy-only tests are mandatory. If no writer path exists, stop writable-editor effort or keep edits explicitly experimental. |
| 8 | Choose UI/viewport framework. | Low before schema; medium after parser | Rank 6; verified coordinates/assets | Compare lightweight tree/property inspector with a 3D viewport. Choose only after knowing model references and coordinate conventions; avoid a framework commitment before data is available. |
| 9 | Validate controlled edits in game and package workflow. | High / low and risky | Rank 7, explicit authorization, backups | Test staged copies, one reversible edit, fresh level load, crash/recovery handling, version compatibility. Never point the editor at the live install as its default output. |

## Evidence workflow

- For each claim capture the exact artifact hash, tool/version, operation, file offset or native VA, and confidence label.
- Keep script file offsets distinct from native VAs. Existing MIR offsets in `FINDINGS.md` do not locate `.scene` fields.
- Record negative results with search scope; “not found in this repo/current bounded web search” does not mean “never existed.”
- Store raw captures under `EVIDENCE/`; write concise interpretations in `LEVEL_EDITOR_EVIDENCE.md`.
- Revalidate all parser assumptions against at least two structurally different files before calling them format rules.

## Immediate next session

Start at ranks 1 and 2. The repository currently records Lost1 offset `0x000084ac` but lacks the scene file and its identity metadata. Obtain read-only samples and attach exact hashes. Then verify the three loader/hierarchy functions against the matching `replay.exe` and capture exact transform-copy instructions. Do not choose Godot/Unity/Qt yet; do not code a writer.
