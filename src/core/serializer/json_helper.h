#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <core/common_types.h>
#include <core/model/text_primitives/text_document_property.h>
#include <nlohmann/json.hpp>
#include <core/path_primitives/bezier_path.h>

namespace inae::json {
using JsonObject = nlohmann::json;
inline JsonObject json_pop(JsonObject &data, const char *key)
{
    JsonObject result;
    if (data.contains(key)) {
        result = data[key];
        data.erase(key);
    }
    return result;
}


inline void decode(Vec1D &val, const JsonObject &in_value)
{
    val = in_value.is_array() ? *in_value.begin() : in_value;
}

inline JsonObject json_value(const Vec1D &value)
{
    return value;
}

inline void decode(unsigned int &val, const JsonObject &in_value)
{
    val = in_value.is_array() ? *in_value.begin() : in_value;
}

inline JsonObject json_value(const unsigned int &value)
{
    return value;
}

inline JsonObject json_value(const core::TextDocumentProperty &value)
{
    return value.to_json();
}

inline void decode(int &val, const JsonObject &in_value)
{
    val = in_value.is_array() ? *in_value.begin() : in_value;
}

inline void decode(Vec3D &val, const JsonObject &in_value)
{
    if (in_value.is_array() && in_value.size() >= 3) {
        val.x = in_value.at(0);
        val.y = in_value.at(1);
        val.z = in_value.at(2);
    } else {
        val = {};
    }
}

inline JsonObject json_value(const Vec3D &value)
{
    JsonObject array;
    array.push_back(value.x);
    array.push_back(value.y);
    array.push_back(value.z);
    return array;
}

inline JsonObject json_value(const Vec2D &value)
{
    JsonObject array;
    array.push_back(value.x);
    array.push_back(value.y);
    return array;
}

inline void decode(Vec2D &val, const JsonObject &in_value)
{
    assertm(in_value.is_array() && in_value.size() == 2, "Vec2D must be an array of size 2!");
    val.x = in_value.at(0);
    val.y = in_value.at(1);
}

inline void decode_keyframe_tangent(Vec2D &obj, const JsonObject &in_value)
{
    JsonObject x = in_value["x"];
    obj.x = x.is_array() ? x.at(0) : x;
    JsonObject y = in_value["y"];
    obj.y = y.is_array() ? y.at(0) : y;
}

inline JsonObject keyframe_tangent_to_json(const Vec2D &obj)
{
    JsonObject result;
    result["x"] = obj.x;
    result["y"] = obj.y;
    return result;
}

//inline void decode(QVector4D &color, const JsonObject &in_value)
//{
//    auto array_value = in_value.toArray();
//    color.setX(array_value.at(0).toDouble());
//    color.setY(array_value.at(1).toDouble());
//    color.setZ(array_value.at(2).toDouble());
//    color.setW(array_value.at(3).toDouble());
//}

inline void decode(core::BezierPath &path, JsonObject &in_value)
{
    path.decode(in_value.is_array() ? in_value.at(0) : in_value);
}

inline void decode(core::TextDocumentProperty &text, JsonObject &in_value)
{
    text.decode(in_value.is_array() ? in_value.at(0) : in_value);
}

} // namespace inae::json
#endif // JSON_HELPER_H
