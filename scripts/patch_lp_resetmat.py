#!/usr/bin/env python3
"""lp-fork: processing_reset_material(graphics_id), so resetShader() can go back to the
normal fill material after shader()/material() (cppmode-resetmat).
Usage: python3 patch_lp_resetmat.py [~/lp-fork]"""
import pathlib, sys, shutil
MARK = "cppmode-resetmat"
LP = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else "~/lp-fork").expanduser()
F = {"command": LP / "crates/processing_render/src/render/command.rs",
     "mod": LP / "crates/processing_render/src/render/mod.rs",
     "ffi": LP / "crates/processing_ffi/src/lib.rs"}
T = {k: p.read_text() for k, p in F.items()}
if MARK in T["ffi"]: print("already patched"); sys.exit(0)
edits = [
 ("command", "    Material(Entity),\n",
  "    Material(Entity),\n    /// " + MARK + ": back to the default fill material (resetShader)\n    ResetMaterial,\n"),
 ("mod", "                DrawCommand::Material(entity) => {\n",
  "                DrawCommand::ResetMaterial => {\n"
  "                    state.style.material_key = style::Style::default().material_key;\n"
  "                }\n"
  "                DrawCommand::Material(entity) => {\n"),
 ("ffi", "#[unsafe(no_mangle)]\npub extern \"C\" fn processing_material(window_id: u64, mat_id: u64) {",
  "/// " + MARK + ": resetShader(): draw with the normal fill material again\n"
  "#[unsafe(no_mangle)]\npub extern \"C\" fn processing_reset_material(graphics_id: u64) {\n"
  "    error::clear_error();\n"
  "    let graphics_entity = Entity::from_bits(graphics_id);\n"
  "    error::check(|| graphics_record_command(graphics_entity, DrawCommand::ResetMaterial));\n}\n\n"
  "#[unsafe(no_mangle)]\npub extern \"C\" fn processing_material(window_id: u64, mat_id: u64) {"),
]
bad = [k + ": " + a.strip()[:50] for k, a, _ in edits if T[k].count(a) != 1]
if bad: print("NOT PATCHED, nothing changed:", *bad, sep="\n  "); sys.exit(1)
for k, a, b in edits: T[k] = T[k].replace(a, b, 1)
bak = LP / ".bak-cppmode-resetmat"; bak.mkdir(exist_ok=True)
for k, p in F.items(): shutil.copy2(p, bak / (k + "_" + p.name)); p.write_text(T[k]); print("patched", p.relative_to(LP))
