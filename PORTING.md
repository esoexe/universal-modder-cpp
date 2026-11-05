# Porting status

universal-modder-cpp is the C++ edition of [universal-modder](https://github.com/rehan-remade/universal-modder)
by Rehan and universal-modder contributors (MIT), at upstream commit `9811709d950979f1b5012dddf10b7ec122d7e95e`. Every Python file
upstream has is listed here with what took its place.

## The `um` CLI (`um/*.py`, `bin/um`)

All of it, as one C++20 program: `scan`, `fal`, `comfy`, `sprixen`, `voidless`, `sprite`, `render3d`, `video`, `win`,
`backup`, `publish`, `kb`, `fastfile`, `iwi`, `weapon`. Same commands, flags, output and file names.

- `render3d` scripted Blender (`um/blender/render_sprites.py`). Here it is a built-in software renderer (glTF 2.0
  GLB/.gltf, skinned and animated meshes, the six presets, the seven motions, shadow passes). It doesn't need Blender, and the light is simpler
  than Cycles: a sun plus ambient.
- Pillow, numpy and PyYAML are replaced by the C++ in `src/packtools/` (PNG codec, resampling, quantizing, YAML).
- HTTP goes through libcurl on Linux and macOS and WinHTTP on Windows.

## Skill helper scripts

| Upstream | Here |
|---|---|
| `skills/blitz3d-decomp/scripts/roblox/b3d_skin.py` | `um b3d dump` |
| `skills/blitz3d-decomp/scripts/roblox/vm_anim_data.py` | `um b3d anim` |
| `skills/blitz3d-decomp/scripts/roblox/vm_build.py` (Blender, FBX) | `um b3d build` (glTF: `NAME.glb` + `NAME.preview.glb`, no Blender) |
| `skills/blitz3d-decomp/scripts/roblox/vm_render.py` (Blender) | `um b3d render` |
| `skills/reverse-engineering/references/ue3-packages/unpack_upk.py` (minilzo) | `um upk unpack` / `um upk clear-flags` (own LZO1X decompressor) |

## Examples (38 Python files: 3 ported, 35 not)

The reusable file-format tools are built-in commands. The rest belong to one mod's own build: build scripts, the
Minecraft passthrough hosts and video directors, the Portal 2 model converter. They need that exact setup and
can't be tested without it, so they are not part of this edition. Each example's write-up, data and media are here.
The scripts are in the upstream repository.

| Upstream file | Status |
|---|---|
| `examples/aoe2-de-civ/aoe2paths.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/aoe2-de-civ/aoe2paths.py)) |
| `examples/aoe2-de-civ/build_mod.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/aoe2-de-civ/build_mod.py)) |
| `examples/aoe2-de-civ/civ.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/aoe2-de-civ/civ.py)) |
| `examples/aoe2-de-civ/make_scenario.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/aoe2-de-civ/make_scenario.py)) |
| `examples/aoe2-de-civ/make_sprites.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/aoe2-de-civ/make_sprites.py)) |
| `examples/aoe2-de-civ/make_wonder.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/aoe2-de-civ/make_wonder.py)) |
| `examples/aoe2-de-civ/sld.py` | ported: `um sld` (info, roundtrip, to-png, and `write` for what `sld.write()` did) |
| `examples/cities-skylines-city-dashboard/tools/mock_game.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/cities-skylines-city-dashboard/tools/mock_game.py)) |
| `examples/minecraft-gta5-passthrough/host/cmd.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/host/cmd.py)) |
| `examples/minecraft-gta5-passthrough/host/fakehost.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/host/fakehost.py)) |
| `examples/minecraft-gta5-passthrough/host/mcframe.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/host/mcframe.py)) |
| `examples/minecraft-gta5-passthrough/host/place_test.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/host/place_test.py)) |
| `examples/minecraft-gta5-passthrough/host/tnt_test.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/host/tnt_test.py)) |
| `examples/minecraft-gta5-passthrough/video/backup_rec.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/backup_rec.py)) |
| `examples/minecraft-gta5-passthrough/video/cut.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/cut.py)) |
| `examples/minecraft-gta5-passthrough/video/cut_segments.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/cut_segments.py)) |
| `examples/minecraft-gta5-passthrough/video/director.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/director.py)) |
| `examples/minecraft-gta5-passthrough/video/go.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/go.py)) |
| `examples/minecraft-gta5-passthrough/video/mobwar_live.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/mobwar_live.py)) |
| `examples/minecraft-gta5-passthrough/video/record.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/record.py)) |
| `examples/minecraft-gta5-passthrough/video/take.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/take.py)) |
| `examples/minecraft-gta5-passthrough/video/titles.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-gta5-passthrough/video/titles.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/fix-owml-config.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/fix-owml-config.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/install-guest.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/install-guest.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/install-mod.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/install-mod.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/launch-mc.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/launch-mc.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/look.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/look.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/oracle.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/oracle.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/pose-sweep.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/pose-sweep.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/probe-frame.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/probe-frame.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/probe-guest.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/probe-guest.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/probe_guest_lib.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/probe_guest_lib.py)) |
| `examples/minecraft-outerwilds-passthrough/tools/stop-guest.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/minecraft-outerwilds-passthrough/tools/stop-guest.py)) |
| `examples/outer-wilds-portal-gun/tools/convert.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/outer-wilds-portal-gun/tools/convert.py)) |
| `examples/outer-wilds-portal-gun/tools/source_mdl.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/outer-wilds-portal-gun/tools/source_mdl.py)) |
| `examples/outer-wilds-portal-gun/tools/vpk.py` | ported: `um vpk` (list, extract) |
| `examples/outer-wilds-portal-gun/tools/vtf.py` | ported: `um vtf` (info, to-png) |
| `examples/shadow-of-war-offline-mode/tools/log_to_object.py` | not ported: a case study tied to one setup ([upstream](https://github.com/rehan-remade/universal-modder/blob/9811709d950979f1b5012dddf10b7ec122d7e95e/examples/shadow-of-war-offline-mode/tools/log_to_object.py)) |

## Tests and CI

`tests/*.py` (pytest) are replaced by bash suites (`tests/modder_*.sh`) with fixtures made by a Node script.
They check the same behaviour plus the new groups. No Python is used, not even as a test oracle.
Upstream's star-chart workflow (`.github/star-chart/*.py`) draws upstream's own README chart, so it is not carried over.
