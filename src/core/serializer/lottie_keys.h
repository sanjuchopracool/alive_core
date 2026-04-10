#ifndef LOTTIE_KEYS_H
#define LOTTIE_KEYS_H

namespace alive::core {
namespace transform_keys {
const char k_anchor_key[] = "a";
const char k_position_key[] = "p";
const char k_scale_key[] = "s";
const char k_rotation_key[] = "r";
const char k_opacity_key[] = "o";
const char k_skew_key[] = "sk";
const char k_skew_axis_key[] = "sa";
} // namespace transform_keys

namespace rectangle_keys {
const char k_position_key[] = "p";
const char k_size_key[] = "s";
const char k_radius_key[] = "r";
} // namespace rectangle_keys

namespace polystar_keys {
const char k_position_key[] = "p";
const char k_rotation_key[] = "r";
const char k_outer_radius_key[] = "or";
const char k_outer_roundness_key[] = "os";
const char k_inner_radius_key[] = "ir";
const char k_inner_roundness_key[] = "is";
const char k_star_type_key[] = "sy";
const char k_num_points_key[] = "pt";
} // namespace polystar_keys
namespace shape_keys {
const char k_path_key[] = "ks";
}

namespace fill_keys {
const char k_opacity_key[] = "o";
const char k_color_key[] = "c";
const char k_fill_rule_key[] = "r";
const char k_fill_enabled_key[] = "fillEnabled";
} // namespace fill_keys

namespace stroke_keys {
const char k_opacity_key[] = "o";
const char k_color_key[] = "c";
const char k_width_key[] = "w";
//const QString line_cap_key = "lc";
//const QString line_join_key = "lj";
//const QString miter_limit_key = "ml";
//const QString dash_pattern_key = "d";
//const QString fill_enabled_key ="fillEnabled";
} // namespace stroke_keys

namespace trim_keys {
const char k_start_key[] = "s";
const char k_end_key[] = "e";
const char k_offset_key[] = "o";
const char k_trim_type_key[] = "m";
const char k_index_key[] = "ix";
} // namespace trim_keys
namespace layer_key {
const char k_name_key[] = "nm";
const char k_type_key[] = "ty";
const char k_index_key[] = "ind";
const char k_coordinate_space_key[] = "ddd";
const char k_in_frame_key[] = "ip";
const char k_out_frame_key[] = "op";
const char k_start_time_key[] = "st";
const char k_transform_key[] = "ks";
const char k_parent_key[] = "parent";
const char k_blend_mode_key[] = "bm";
const char k_masks_key[] = "masksProperties";
const char k_time_stretch_key[] = "sr";
const char k_matte_key[] = "tt";
const char k_hidden_key[] = "hd";
const char k_auto_orient_key[] = "ao";
const char k_is_used_as_matter[] = "td";
const char k_matte_parent[] = "tp";

const char k_alive_editor_color_index[] = "alive_eci";
const char k_alive_layer_locked[] = "alive_ll";
} // namespace layer_key

namespace solid_layer_key {
const char k_color_key[] = "sc";
const char k_width_key[] = "sw";
const char k_height_key[] = "sh";
} // namespace solid_layer_key

namespace shape_layer_key {
const char k_shapes_key[] = "shapes";
} // namespace shape_layer_key

namespace image_layer_key {
const char k_refid_key[] = "refId";
} // namespace image_layer_key

namespace precomp_layer_key {
const char k_refid_key[] = "refId";
} // namespace precomp_layer_key

namespace text_layer_key {
const char k_document_data_key[] = "t";
const char k_text_data_key[] = "d";
} // namespace text_layer_key

namespace group_key {
const char k_shapes_key[] = "it";
} // namespace group_key

namespace composition_keys {
const char k_version_key[] = "v";
const char k_alive_version_key[] = "alive_v";
const char k_type_key[] = "ddd";
const char k_in_point_key[] = "ip";
const char k_out_point_key[] = "op";
const char k_framerate_key[] = "fr";
const char k_width_key[] = "w";
const char k_height_key[] = "h";
const char k_layers_key[] = "layers";
const char k_name_key[] = "nm";
const char k_uuid_key[] = "uuid";
//const char glyphs_key[] ="chars";
//const char fonts_key[] ="fonts";
const char k_asset_library_key[] = "assets";
//const char markers_key[] ="markers";
} // namespace composition_keys

namespace shape_item_keys {
const char k_match_name_key[] = "mn";
const char k_name_key[] = "nm";
const char k_hidden_key[] = "hd";
const char k_index_key[] = "ind";
const char k_type_key[] = "ty";
const char k_direction_key[] = "d";
} // namespace shape_item_keys

namespace repeater_key {
const char k_copies_key[] = "c";
const char k_offset_key[] = "o";
const char k_composite_key[] = "m";
const char k_transform_key[] = "tr";
} // namespace repeater_key
} // namespace alive::core
#endif // LOTTIE_KEYS_H
