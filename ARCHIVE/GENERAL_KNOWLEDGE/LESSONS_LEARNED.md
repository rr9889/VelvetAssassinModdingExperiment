# Lessons learned

SESSION EVIDENCE describes observable records or explicit user reports. GENERALIZED INFERENCE describes recommendations derived from them.

## Configuration before patching

**Problem:** Model selection appeared to require binary changes. **What happened — SESSION EVIDENCE:** levelstructure.aio Outfit changed the fresh-load model without a replay.exe patch. **General principle — GENERALIZED INFERENCE:** search direct data controls first. **Apply to another game:** inspect level/entity configuration and comments before planning a detour.

## Decision logic versus generic loader logic

**Problem:** A scene loader appeared to control outfit choice. **What happened — SESSION EVIDENCE:** wrappers forwarded supplied identifiers; the stronger selector was upstream in Include_Scene script. **General principle — GENERALIZED INFERENCE:** consuming a choice is different from making it. **Apply elsewhere:** trace arguments backwards to the assignment or branch that produces them.

## Static state before live replacement

**Problem:** Initial selection success could be mistaken for safe hot swapping. **What happened — SESSION EVIDENCE:** fresh-load selection worked, but a native guard rejected loading into actors with children. Runtime switches were untested. **General principle — GENERALIZED INFERENCE:** initialization and mutation have different lifecycle constraints. **Apply elsewhere:** prove static selection, then inspect ownership, hierarchy, callbacks and rebinding.

## Visual state versus disguise gameplay

**Problem:** An SS mesh might be assumed to grant cover. **What happened — SESSION EVIDENCE:** static reports separated outfit/camouflage queries and per-NPC recognition state from resource availability. **General principle — GENERALIZED INFERENCE:** visual and gameplay states require separate evidence. **Apply elsewhere:** test appearance, faction, perception, hostility, animation and equipment independently.

## One variable at a time

**Problem:** Multiple edits obscure causality. **What happened — SESSION EVIDENCE:** the reported 1→0 test changed initial Outfit and used a fresh level. **General principle — GENERALIZED INFERENCE:** isolate the control producing the result. **Apply elsewhere:** keep unrelated configuration/assets/executable constant and record before/after effects.

## Fresh-level tests for cached state

**Problem:** File changes may not affect already constructed objects. **What happened — SESSION EVIDENCE:** successful tests started fresh; static reports found cached level data. **General principle — GENERALIZED INFERENCE:** distinguish edit time from read time. **Apply elsewhere:** test restart and checkpoint behavior separately; a restart result does not demonstrate hot reload.

## Hashes belong with addresses

**Problem:** Addresses may refer to another build or address space. **What happened — SESSION EVIDENCE:** executable/script hashes were recorded and file offsets separated from native addresses. **General principle — GENERALIZED INFERENCE:** bind addresses to artifact identity. **Apply elsewhere:** record hash, architecture, function entry, instruction xref and VA/RVA/file-offset/VM-slot classification.

## Runtime behavior versus decompiler assumptions

**Problem:** Pseudocode and reconstructed VM slots can look definitive. **What happened — SESSION EVIDENCE:** decompiler warnings existed; selector mappings remained LIKELY while the user test confirmed the config outcome. **General principle — GENERALIZED INFERENCE:** corroborate static models without upgrading every internal edge. **Apply elsewhere:** compare assembly, metadata and actual behavior; label unresolved dispatch.

## Confidence is claim-specific

**Problem:** A single label can blur fact and interpretation. **What happened — SESSION EVIDENCE:** records used CONFIRMED/LIKELY/SPECULATIVE and distinguished user reports from direct observation. **General principle — GENERALIZED INFERENCE:** label each claim and source. **Apply elsewhere:** string presence does not confirm execution, causality or safety.

## Preserve failures

**Problem:** Negative evidence disappears from polished summaries. **What happened — SESSION EVIDENCE:** provider_unavailable, helper errors, empty defined-string searches and loader guards constrained the work. **General principle — GENERALIZED INFERENCE:** preserve symptoms, search scope and recovery status. **Apply elsewhere:** separate tool lanes and do not treat an empty bounded search as global absence.

## Stop when the simpler goal is solved

**Problem:** Exploration can continue after a direct control works. **What happened — SESSION EVIDENCE:** tracing stopped at its candidate criterion; config tests solved level-load appearance, and runtime work was separately requested. **General principle — GENERALIZED INFERENCE:** scope follows the requested outcome. **Apply elsewhere:** deliver the minimal working control before optional hooks or unrelated systems.

## Validate asset compatibility

**Problem:** Humanoid assets appear interchangeable. **What happened — SESSION EVIDENCE:** enemy/Violette label sets differed, 52 versus 76; no swap was validated. **General principle — GENERALIZED INFERENCE:** shape is insufficient rig evidence. **Apply elsewhere:** inspect hierarchy, skin weights, bind transforms and animation requirements.

## Validate parsers and enums

**Problem:** Generic extraction can create false names or misses. **What happened — SESSION EVIDENCE:** printable length bytes appended to names; overlapping record candidates and regex escaping required correction; initial outfit and runtime mode enums differed. **General principle — GENERALIZED INFERENCE:** validate boundaries and namespaces. **Apply elsewhere:** check known records, escape literal binary patterns and tolerate configuration whitespace.

## Applied state versus intended state

**Problem:** An attempted write can be mistaken for completion. **What happened — SESSION EVIDENCE:** the last large runtime-documentation patch failed validation and the turn was interrupted. **General principle — GENERALIZED INFERENCE:** successful tool results and resulting files establish completion. **Apply elsewhere:** read back outputs and hash final archival bytes.
