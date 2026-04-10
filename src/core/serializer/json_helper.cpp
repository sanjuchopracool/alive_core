#include "json_helper.h"
#include <core/asset_management/font_manager.h>
#include <core/model/text_primitives/text.h>
#include <core/path_primitives/bezier_path.h>
#include <core/path_primitives/curve_vertex.h>

namespace alive::json {

namespace text_doc_keys {
const char k_font_size_key[] = "s";
const char k_fill_color_key[] = "fc";
const char k_font_family_key[] = "f";
const char k_alive_font_style_key[] = "alive_fs";
const char k_text_key[] = "t";
const char k_stroke_color_key[] = "sc";
const char k_stroke_width_key[] = "sw";
} // namespace text_doc_keys

namespace bezier_path_keys {
const char k_closed_key[] = "c";
const char k_in_points_key[] = "i";
const char k_out_points_key[] = "o";
const char k_vertices_key[] = "v";
} // namespace bezier_path_keys

void decode_text(core::Text &text, JsonObject &in_value)
{
    using namespace text_doc_keys;
    using namespace alive::json;
    auto temp_json = json_pop(in_value, k_font_size_key);
    if (!temp_json.is_null()) {
        text.m_font.size = temp_json;
    }
    temp_json = json_pop(in_value, k_fill_color_key);
    if (!temp_json.is_null()) {
        json::decode(text.m_fill_color, temp_json);
    } else {
        text.m_fill = false;
    }

    const auto &font_mgr = core::FontManager::instance();
    temp_json = json_pop(in_value, k_font_family_key);
    if (!temp_json.is_null()) {
        const std::string family_name = temp_json;
        int index = font_mgr.family_index(family_name);
        if (index == -1) {
            text.m_font.family = font_mgr.default_index().first;
        } else {
            text.m_font.family = index;
        }
        temp_json = json_pop(in_value, k_alive_font_style_key);
        if (!temp_json.is_null()) {
            const std::string style_name = temp_json;
            text.m_font.style = font_mgr.style_index(index, style_name);
        }
    }
    temp_json = json_pop(in_value, k_text_key);
    if (!temp_json.is_null()) {
        text.m_text = temp_json;
    }

    temp_json = json_pop(in_value, k_stroke_color_key);
    if (!temp_json.is_null()) {
        json::decode(text.m_stroke_color, temp_json);
        text.m_stroke = true;
    }

    temp_json = json_pop(in_value, k_stroke_width_key);
    if (!temp_json.is_null()) {
        text.m_stroke_width = temp_json;
    }
}

void decode(core::Text &text, JsonObject &in_value)
{
    decode_text(text, in_value.is_array() ? in_value.at(0) : in_value);
}

JsonObject json_value(const core::Text &text)
{
    using namespace text_doc_keys;
    nlohmann::ordered_json result;
    result[k_font_size_key] = text.m_font.size;
    result[k_font_family_key] = core::FontManager::instance().family_name(text.m_font.family);
    result[k_alive_font_style_key] = core::FontManager::instance().style_name(text.m_font.family,
                                                                             text.m_font.style);
    if (text.m_fill) {
        result[k_fill_color_key] = json::json_value(text.m_fill_color);
    }
    if (text.m_stroke) {
        result[k_stroke_color_key] = json::json_value(text.m_stroke_color);
        result[k_stroke_width_key] = text.m_stroke_width;
    }
    result[k_text_key] = text.m_text;
    return result;
}

void decode_bezier(core::BezierPath &path, JsonObject &in_value)
{
    using namespace alive::json;
    using namespace bezier_path_keys;

    path.m_closed = json_pop(in_value, k_closed_key);
    auto vertices_array = json_pop(in_value, k_vertices_key);
    auto in_points_array = json_pop(in_value, k_in_points_key);
    auto out_points_array = json_pop(in_value, k_out_points_key);

    assert(vertices_array.size() == in_points_array.size());
    assert(out_points_array.size() == in_points_array.size());
    int size = vertices_array.size();
    if (!size)
        return;

    auto json_to_point = [](JsonObject &data) {
        assertm(data.is_array(), "vertices index should");
        float x = data.at(0);
        float y = data.at(1);
        return Vec2D(x, y);
    };
    auto first_vertex = core::CurveVertex::from_relative(json_to_point(vertices_array.at(0)),
                                                         json_to_point(in_points_array.at(0)),
                                                         json_to_point(out_points_array.at(0)));

    auto previous_element = core::PathElement(0, first_vertex);

    std::vector<core::PathElement> decoded_elements;
    decoded_elements.reserve(size);
    decoded_elements.push_back(previous_element);

    double total_length = 0;
    for (int i = 1; i < size; ++i) {
        auto vertex = core::CurveVertex::from_relative(json_to_point(vertices_array.at(i)),
                                                       json_to_point(in_points_array.at(i)),
                                                       json_to_point(out_points_array.at(i)));

        const auto path_element = previous_element.path_element_to(vertex);
        decoded_elements.emplace_back(path_element);
        total_length += path_element.m_length;
        previous_element = path_element;
    }

    if (path.m_closed) {
        auto close_element = previous_element.path_element_to(first_vertex);
        decoded_elements.emplace_back(close_element);
        total_length += close_element.m_length;
    }

    path.m_length = total_length;
    path.m_elements = decoded_elements;
};
void decode(core::BezierPath &path, JsonObject &in_value)
{
    decode_bezier(path, in_value.is_array() ? in_value.at(0) : in_value);
}

JsonObject json_value(const core::BezierPath &path)
{
    using namespace alive::json;
    using namespace bezier_path_keys;
    JsonObject result;
    result[k_closed_key] = path.m_closed;
    std::vector<alive::Point2D> vertices_array;
    std::vector<alive::Point2D> in_points_array;
    std::vector<alive::Point2D> out_points_array;

    int size = path.m_elements.size();
    vertices_array.reserve(size);
    in_points_array.reserve(size);
    out_points_array.reserve(size);

    auto to_json_array = [](const std::vector<alive::Point2D> &points) {
        JsonObject array;
        for (const auto &item : points) {
            JsonObject point;
            point.emplace_back(item.x());
            point.emplace_back(item.y());
            array.emplace_back(point);
        }
        return array;
    };
    if (size) {
        auto &first_vertex = path.m_elements[0].m_vertex;
        vertices_array.emplace_back(first_vertex.m_point);
        in_points_array.emplace_back(first_vertex.m_in_point - first_vertex.m_point);
        out_points_array.emplace_back(first_vertex.m_out_point - first_vertex.m_point);

        if (path.m_closed)
            size -= 1;
        for (int i = 1; i < size; ++i) {
            auto &curve_vertex = path.m_elements[i].m_vertex;
            vertices_array.emplace_back(curve_vertex.m_point);
            in_points_array.emplace_back(curve_vertex.m_in_point - curve_vertex.m_point);
            out_points_array.emplace_back(curve_vertex.m_out_point - curve_vertex.m_point);
        }
    }
    result[k_vertices_key] = to_json_array(vertices_array);
    result[k_in_points_key] = to_json_array(in_points_array);
    result[k_out_points_key] = to_json_array(out_points_array);
    return result;
}

JsonObject json_value(const core::Font &value)
{
    auto &font_mgr = alive::core::FontManager::instance();
    JsonObject array;
    array.push_back(font_mgr.family_name(value.family));
    array.push_back(font_mgr.style_name(value.family, value.style));
    array.push_back(value.size);
    return array;
}

void decode(core::Font &font, const JsonObject &in_json)
{
    if (in_json.is_array() && in_json.size() == 3) {
        const std::string family_name = in_json[0];
        auto &font_mgr = alive::core::FontManager::instance();
        int index = font_mgr.family_index(family_name);
        if (index == -1) {
            font.family = font_mgr.default_index().first;
        } else {
            font.family = index;
        }
        const std::string style_name = in_json[1];
        font.style = font_mgr.style_index(index, style_name);
        font.size = in_json[2];
    }
}

} // namespace alive::json
