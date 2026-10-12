# Velvet Assassin Research

This folder documents reverse engineering of Velvet Assassin `replay.exe` for a local single-player mod.

Target: `C:\Users\Admin\Documents\Velvet Assassin\replay.exe`

Tools: Codex CLI, REA, Ghidra 12.1.4.

Current target: VelvetSwap.dll, x86/Win32, with F1=Campaign, F2=Pilot, F3=Nightdress, F4=SS uniform, F5=restore previous/original. Start with [RUNTIME_OUTFIT_SWAP_CHECKPOINT.md](RUNTIME_OUTFIT_SWAP_CHECKPOINT.md). The current pass is documentation and read-only identity verification. The user previously reported a configuration edit; its restoration is unreported. No assistant game-file modification is claimed.

CURRENT_STATE.md, FINDINGS.md, FUNCTIONS.csv, SESSION_LOG.md, and NEXT_STEPS.md preserve the evidence and current blocker. Historical milestone/patch plans are marked as superseded. Keep all source, builds, logs, findings, and generated documentation in this repository, never the game directory. PROMPTS contains the continuation prompt; EVIDENCE and BACKUPS describe evidence and backup handling.
