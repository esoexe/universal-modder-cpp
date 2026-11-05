# Notice

universal-modder-cpp is the C++ edition of **universal-modder** by Rehan and universal-modder contributors
(https://github.com/rehan-remade/universal-modder, commit 9811709d950979f1b5012dddf10b7ec122d7e95e), used under the MIT License (LICENSE).

- The `um` CLI is reimplemented in C++20 (`src/`). It is the same code the A.S.T.R.O AI engine
  (https://github.com/esoexe/astro-ai) compiles as its built-in `um` tool.
- The skills and agents are upstream's text. Where a skill ran a Python helper script, it now names the built-in
  command, and the edition notes say what changed. The guardrails are unchanged: mod only games the user owns, keep
  game files and decompiled code out of anything shared, no cheats against other players, back up saves first.
- The knowledge base, docs, examples (without their Python scripts), hooks and agent manifests are upstream files.
  Contributions to the knowledge base belong upstream: `um kb pr` opens them there.
- Space Grotesk and JetBrains Mono (SIL Open Font License 1.1) are embedded for `um video` and written, with
  their licence texts, to the um data folder the first time a video needs them.

This edition is not made or endorsed by the universal-modder authors, fal, Sprixen, Voidless or ComfyUI.
