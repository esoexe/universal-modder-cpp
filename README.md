# universal-modder-cpp

The C++ edition of [universal-modder](https://github.com/rehan-remade/universal-modder) by Rehan and
universal-modder contributors: skills, agents, a shared knowledge base and the `um` toolkit for modding games you
own with an AI agent. Here `um` is **one native program** (C++20). It needs no Python, uv, Pillow, numpy or Blender.

> Upstream is the original and the place for knowledge-base notes (`um kb pr` sends them there). This edition
> tracks upstream commit `9811709d9509`. PORTING.md lists every upstream Python file and what replaced it.

## Get `um`

- **Download** a binary from [Releases](https://github.com/esoexe/universal-modder-cpp/releases): Linux x64 (needs
  libcurl, which most distributions ship), macOS arm64, Windows x64 (a single .exe, no DLLs). Put it on PATH as `um`.
- **Or build it:** `make` (a C++20 compiler, libcurl and zlib), then `make test`. `make windows` cross-builds
  `build/um.exe` with MinGW-w64. In a clone, `bin/um` runs `build/um`, building it the first time.

## What `um` does

| Group | |
|---|---|
| `scan` | installed games, engine, anti-cheat, loaders, saves, routes |
| `fal`, `comfy`, `sprixen`, `voidless` | art, 3D, audio and video through fal (your key), a local ComfyUI, or Sprixen / Voidless (paid, your keys) |
| `sprite` | cut out, fit, pixelate, recolour, pack sprite sheets |
| `render3d` | GLB / glTF to sprite frames from a game's camera: aoe2, iso8, trueiso, topdown, side, turntable; motions; shadow passes |
| `video` | contact sheets and EDL showcase edits (needs ffmpeg) |
| `win` | launch, screenshot, input and record on Windows (also from WSL) |
| `backup`, `publish`, `kb` | save snapshots, a pre-release lint, the knowledge base |
| `fastfile`, `iwi`, `weapon` | Call of Duty 4 / World at War rawfiles, IWI textures, weapon files |
| `b3d` | Blitz3D viewmodels: dump, Luau keyframes, a skinned glTF for Roblox, preview frames |
| `upk` | decompress LZO-compressed Unreal Engine 3 packages for UELib |
| `sld` | Age of Empires II DE sprites: info, roundtrip, to PNG, PNG frames to .sld |
| `vpk`, `vtf` | Valve archives and textures: list, extract, decode |

Every group has `--help`. The last four replace the Python helper scripts that upstream's skills and examples ran.

## Use it with an agent

Open the clone in Claude Code, Codex, Cursor, Gemini CLI or OpenCode. The skills (`skills/`, with copies in
`.claude/skills` and `.agents/skills`), the agents (`agents/`), the fal MCP server (`.mcp.json`, needs `FAL_KEY`) and
the PATH hook work as they do upstream. Start with the **mod-any-game** skill. AGENTS.md has the loop and the rules.

## Rules (the same as upstream)

Only games you own. Back up saves before touching anything. Never ship game files, extracted assets or decompiled
code (`um publish check` looks for them, and for secrets). No cheats or advantages against other players; stay away
from games with online anti-cheat.

## Limits

- `render3d` is a software renderer, not Blender Cycles: same cameras and file names, simpler light.
- `um win` works on Windows and WSL only. `um video` needs ffmpeg (`um win setup` fetches a checksum-verified build).
- The examples' one-off scripts are not ported (PORTING.md says which and why).
- The macOS build is checked in CI on Apple Silicon only.

## Licence

MIT, like upstream (LICENSE). Made by the Astro AI team as part of A.S.T.R.O AI (https://github.com/esoexe/astro-ai),
whose engine compiles the same `um` code as a built-in tool.
