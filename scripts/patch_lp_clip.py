#!/usr/bin/env python3
"""lp-fork: clip()/noClip() for the libprocessing renderer (cppmode-clip).
Adds a Clip draw command, carries the rect through Style -> batch -> ProcessingMaterial
(uniform at binding 100), gives ProcessingMaterial a fragment shader that is Bevy's
pbr.wgsl plus a discard outside the rect, and exports processing_clip / processing_no_clip.
Usage: python3 patch_lp_clip.py [~/lp-fork]   (changes nothing unless every anchor matches)"""
import pathlib, re, sys, shutil
WGSL = r'''// cppmode-clip: Bevy's StandardMaterial fragment shader (bevy_pbr/src/render/pbr.wgsl
// at the processing/bevy commit this crate builds against), plus one thing: clip().
// processing_clip_rect is [x, y, w, h] in framebuffer pixels; w <= 0 means no clip.
#import bevy_pbr::{
    pbr_types,
    pbr_functions::alpha_discard,
    pbr_fragment::pbr_input_from_standard_material,
    decal::clustered::apply_decals,
}

#ifdef PREPASS_PIPELINE
#import bevy_pbr::{
    prepass_io::{VertexOutput, FragmentOutput},
    pbr_deferred_functions::deferred_output,
}
#else
#import bevy_pbr::{
    forward_io::{VertexOutput, FragmentOutput},
    pbr_functions::{apply_pbr_lighting, main_pass_post_lighting_processing},
    pbr_types::STANDARD_MATERIAL_FLAGS_UNLIT_BIT,
}
#endif

#ifdef VISIBILITY_RANGE_DITHER
#import bevy_pbr::pbr_functions::visibility_range_dither;
#endif

#ifdef MESHLET_MESH_MATERIAL_PASS
#import bevy_pbr::meshlet_visibility_buffer_resolve::resolve_vertex_output
#endif

#ifdef OIT_ENABLED
#import bevy_core_pipeline::oit::oit_draw
#endif // OIT_ENABLED

#ifdef FORWARD_DECAL
#import bevy_pbr::decal::forward::get_forward_decal_info
#endif

@group(#{MATERIAL_BIND_GROUP}) @binding(100)
var<uniform> processing_clip_rect: vec4<f32>;

@fragment
fn fragment(
#ifdef MESHLET_MESH_MATERIAL_PASS
    @builtin(position) frag_coord: vec4<f32>,
#else
    vertex_output: VertexOutput,
    @builtin(front_facing) is_front: bool,
#endif
) -> FragmentOutput {
#ifdef MESHLET_MESH_MATERIAL_PASS
    let vertex_output = resolve_vertex_output(frag_coord);
    let is_front = true;
#endif

    var in = vertex_output;

    // clip(): drop anything outside the rect (in.position is the framebuffer pixel)
    let clip = processing_clip_rect;
    if (clip.z > 0.0 && (in.position.x < clip.x || in.position.y < clip.y ||
                         in.position.x >= clip.x + clip.z || in.position.y >= clip.y + clip.w)) {
        discard;
    }

    // If we're in the crossfade section of a visibility range, conditionally
    // discard the fragment according to the visibility pattern.
#ifdef VISIBILITY_RANGE_DITHER
    visibility_range_dither(in.position, in.visibility_range_dither);
#endif

#ifdef FORWARD_DECAL
    let forward_decal_info = get_forward_decal_info(in);
    in.world_position = forward_decal_info.world_position;
    in.uv = forward_decal_info.uv;
#endif

    // generate a PbrInput struct from the StandardMaterial bindings
    var pbr_input = pbr_input_from_standard_material(in, is_front);

    // alpha discard
    pbr_input.material.base_color = alpha_discard(pbr_input.material, pbr_input.material.base_color);

    // clustered decals
    apply_decals(&pbr_input);

#ifdef PREPASS_PIPELINE
    // write the gbuffer, lighting pass id, and optionally normal and motion_vector textures
    let out = deferred_output(in, pbr_input);
#else
    // in forward mode, we calculate the lit color immediately, and then apply some post-lighting effects here.
    // in deferred mode the lit color and these effects will be calculated in the deferred lighting shader
    var out: FragmentOutput;
    if (pbr_input.material.flags & STANDARD_MATERIAL_FLAGS_UNLIT_BIT) == 0u {
        out.color = apply_pbr_lighting(pbr_input);
    } else {
        out.color = pbr_input.material.base_color;
    }

    // apply in-shader post processing (fog, alpha-premultiply, and also tonemapping, debanding if the camera is non-hdr)
    // note this does not include fullscreen postprocessing effects like bloom.
    out.color = main_pass_post_lighting_processing(pbr_input, out.color);
#endif

#ifdef OIT_ENABLED
    let alpha_mode = pbr_input.material.flags & pbr_types::STANDARD_MATERIAL_FLAGS_ALPHA_MODE_RESERVED_BITS;
    if alpha_mode != pbr_types::STANDARD_MATERIAL_FLAGS_ALPHA_MODE_OPAQUE {
        // The fragments will only be drawn during the oit resolve pass.
        oit_draw(in.position, out.color);
        discard;
    }
#endif // OIT_ENABLED

#ifdef FORWARD_DECAL
        out.color.a = min(forward_decal_info.alpha, out.color.a);
#endif

        return out;
}
'''
MARK = "cppmode-clip"
LP = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else "~/lp-fork").expanduser()
R = LP / "crates/processing_render/src"
F = {
  "command": R / "render/command.rs", "style": R / "render/style.rs", "mod": R / "render/mod.rs",
  "rmat": R / "render/material.rs", "mat": R / "material/mod.rs", "gltf": R / "gltf.rs",
  "ffi": LP / "crates/processing_ffi/src/lib.rs",
}
for p in F.values():
    if not p.exists(): print("missing", p); sys.exit(1)
T = {k: p.read_text() for k, p in F.items()}
if MARK in T["mat"]: print("already patched"); sys.exit(0)
problems = []
def rep(key, old, new, count=1):
    if T[key].count(old) != count:
        problems.append("%s: anchor found %d times (want %d): %s" % (F[key].name, T[key].count(old), count, old.strip().splitlines()[0][:70])); return
    T[key] = T[key].replace(old, new)

# 1. draw command
rep("command", "    BlendMode(Option<BlendState>),\n",
    "    BlendMode(Option<BlendState>),\n    /// " + MARK + ": [x, y, w, h] in framebuffer pixels, None = noClip()\n    Clip(Option<[f32; 4]>),\n")
# 2. style
rep("style", "    pub blend_state: Option<BlendState>,\n    pub tint_color: Option<Color>,\n",
    "    pub blend_state: Option<BlendState>,\n    pub clip_rect: Option<[f32; 4]>,\n    pub tint_color: Option<Color>,\n")
rep("style", "            blend_state: None,\n            tint_color: None,\n",
    "            blend_state: None,\n            clip_rect: None,\n            tint_color: None,\n")
# 3. render loop + batching
rep("mod", "    graphics_entity: Entity,\n}\n\nimpl BatchState {",
    "    graphics_entity: Entity,\n    clip_rect: Option<[f32; 4]>,\n}\n\nimpl BatchState {")
rep("mod", "            render_layers,\n            graphics_entity,\n        }\n",
    "            render_layers,\n            graphics_entity,\n            clip_rect: None,\n        }\n")
rep("mod", "                DrawCommand::BlendMode(blend_state) => {\n                    state.style.blend_state = blend_state;\n                }\n",
    "                DrawCommand::BlendMode(blend_state) => {\n                    state.style.blend_state = blend_state;\n                }\n"
    "                DrawCommand::Clip(rect) => {\n                    state.style.clip_rect = rect;\n                }\n")
rep("mod", "    material_changed || transform_changed || requires_separate_draws\n",
    "    let clip_changed = batch.clip_rect != state.style.clip_rect;\n    material_changed || transform_changed || requires_separate_draws || clip_changed\n")
rep("mod", "    batch.material_key = Some(material_key);\n",
    "    batch.material_key = Some(material_key);\n    batch.clip_rect = state.style.clip_rect;\n")
rep("mod", "_ => key.to_material_biased(&mut res.materials, batch.draw_index as f32 * BATCH_INDEX_STEP * 2.0),",
    "_ => key.to_material_clipped(&mut res.materials, batch.draw_index as f32 * BATCH_INDEX_STEP * 2.0, batch.clip_rect),")
rep("mod", "                extension: ProcessingMaterial {\n                    blend_state: state.style.blend_state,\n                    depth_write: None,\n                },",
    "                extension: ProcessingMaterial {\n                    blend_state: state.style.blend_state,\n                    depth_write: None,\n                    clip_rect: crate::render::material::clip_vec(state.style.clip_rect),\n                },")
# 4. MaterialKey -> material
rep("rmat", """    pub fn to_material_biased(
        &self,
        materials: &mut ResMut<Assets<ProcessingExtendedMaterial>>,
        depth_bias: f32,
    ) -> UntypedHandle {
""", """    pub fn to_material_biased(
        &self,
        materials: &mut ResMut<Assets<ProcessingExtendedMaterial>>,
        depth_bias: f32,
    ) -> UntypedHandle {
        self.to_material_clipped(materials, depth_bias, None)
    }

    /// """ + MARK + """: same as to_material_biased, plus the clip() rect the shader discards outside of
    pub fn to_material_clipped(
        &self,
        materials: &mut ResMut<Assets<ProcessingExtendedMaterial>>,
        depth_bias: f32,
        clip_rect: Option<[f32; 4]>,
    ) -> UntypedHandle {
""")
rep("rmat", """            extension: ProcessingMaterial {
                blend_state,
                depth_write: None,
            },""", """            extension: ProcessingMaterial {
                blend_state,
                depth_write: None,
                clip_rect: clip_vec(clip_rect),
            },""")
T["rmat"] = T["rmat"].rstrip("\n") + """

/// """ + MARK + """: clip rect for the shader; zero width means "no clip"
pub fn clip_vec(rect: Option<[f32; 4]>) -> bevy::math::Vec4 {
    match rect {
        Some([x, y, w, h]) if w > 0.0 && h > 0.0 => bevy::math::Vec4::new(x, y, w, h),
        Some(_) => bevy::math::Vec4::new(0.0, 0.0, 1e-6, 1e-6), // empty clip: hide everything
        None => bevy::math::Vec4::ZERO,
    }
}
"""
# 5. other ProcessingMaterial constructors get "no clip"
for key, ind in (("mat", "            "), ("gltf", "                ")):
    rep(key, ind + "blend_state: None,\n" + ind + "depth_write: None,\n",
        ind + "blend_state: None,\n" + ind + "depth_write: None,\n" + ind + "clip_rect: bevy::math::Vec4::ZERO,\n")
# 6. material extension: uniform + shader
rep("mat", "pub struct ProcessingMaterial {\n    pub blend_state: Option<BlendState>,\n    pub depth_write: Option<bool>,\n}",
    "pub struct ProcessingMaterial {\n    pub blend_state: Option<BlendState>,\n    pub depth_write: Option<bool>,\n    /// " + MARK + ": [x, y, w, h] in framebuffer pixels, w <= 0 means no clip\n    #[uniform(100)]\n    pub clip_rect: bevy::math::Vec4,\n}")
rep("mat", "    fn fragment_shader() -> ShaderRef {\n        <StandardMaterial as Material>::fragment_shader()\n    }",
    "    fn fragment_shader() -> ShaderRef {\n        // " + MARK + ": Bevy's pbr.wgsl plus the clip() discard\n        ShaderRef::Path(\"embedded://processing_render/material/processing_material.wgsl\".into())\n    }")
rep("mat", "    fn build(&self, app: &mut App) {\n        app.add_plugins(bevy::pbr::MaterialPlugin::<\n            ExtendedMaterial<StandardMaterial, ProcessingMaterial>,\n        >::default());",
    "    fn build(&self, app: &mut App) {\n        bevy::asset::embedded_asset!(app, \"processing_material.wgsl\");  // " + MARK + "\n        app.add_plugins(bevy::pbr::MaterialPlugin::<\n            ExtendedMaterial<StandardMaterial, ProcessingMaterial>,\n        >::default());")
# 7. C exports
rep("ffi", "#[unsafe(no_mangle)]\npub extern \"C\" fn processing_set_custom_blend_mode(",
    """/// """ + MARK + """: clip(): only draw inside [x, y, w, h] (framebuffer pixels) until no_clip.
#[unsafe(no_mangle)]
pub extern "C" fn processing_clip(graphics_id: u64, x: f32, y: f32, w: f32, h: f32) {
    error::clear_error();
    let graphics_entity = Entity::from_bits(graphics_id);
    error::check(|| graphics_record_command(graphics_entity, DrawCommand::Clip(Some([x, y, w, h]))));
}

/// """ + MARK + """: noClip()
#[unsafe(no_mangle)]
pub extern "C" fn processing_no_clip(graphics_id: u64) {
    error::clear_error();
    let graphics_entity = Entity::from_bits(graphics_id);
    error::check(|| graphics_record_command(graphics_entity, DrawCommand::Clip(None)));
}

#[unsafe(no_mangle)]
pub extern "C" fn processing_set_custom_blend_mode(""")

if problems:
    print("NOT PATCHED, nothing changed:"); [print("  -", x) for x in problems]; sys.exit(1)
bak = LP / ".bak-cppmode-clip"; bak.mkdir(exist_ok=True)
for k, p in F.items():
    if T[k] != p.read_text():
        shutil.copy2(p, bak / (k + "_" + p.name)); p.write_text(T[k]); print("patched", p.relative_to(LP))
(R / "material/processing_material.wgsl").write_text(WGSL)
print("wrote crates/processing_render/src/material/processing_material.wgsl")
