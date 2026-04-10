#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "core/font.h"
#include <core/alive_types/common_types.h>
#include <nlohmann/json.hpp>

namespace alive::core {
class Text;
class BezierPath;
} // namespace alive::core

namespace alive::json {
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

JsonObject json_value(const alive::core::Font &value);
void decode(alive::core::Font &val, const JsonObject &in_json);

inline void decode(unsigned int &val, const JsonObject &in_value)
{
    val = in_value.is_array() ? *in_value.begin() : in_value;
}

inline JsonObject json_value(const unsigned int &value)
{
    return value;
}

JsonObject json_value(const core::Text &text);

inline void decode(int &val, const JsonObject &in_value)
{
    val = in_value.is_array() ? *in_value.begin() : in_value;
}

inline void decode(Vec3D &val, const JsonObject &in_value)
{
    if (in_value.is_array() && in_value.size() >= 2) {
        val.x() = in_value.at(0);
        val.y() = in_value.at(1);

        if (in_value.size() >= 3)
            val.z() = in_value.at(2);
        return;
    }

    val = {};
}

inline void decode(Color4ub &val, const JsonObject &in_value)
{
    if (in_value.is_array() && in_value.size() == 4) {
        val.r() = in_value.at(0);
        val.g() = in_value.at(1);
        val.b() = in_value.at(2);
        val.a() = in_value.at(3);
        return;
    }

    val = {};
}

inline JsonObject json_value(const Vec3D &value)
{
    JsonObject array;
    array.push_back(value.x());
    array.push_back(value.y());
    array.push_back(value.z());
    return array;
}
inline JsonObject json_value(const Color4ub &value)
{
    JsonObject array;
    array.push_back(value.r());
    array.push_back(value.g());
    array.push_back(value.b());
    array.push_back(value.a());
    return array;
}

inline JsonObject json_value(const Vec2D &value)
{
    JsonObject array;
    array.push_back(value.x());
    array.push_back(value.y());
    return array;
}

inline void decode(Vec2D &val, const JsonObject &in_value)
{
    assertm(in_value.is_array() && in_value.size() == 2, "Vec2D must be an array of size 2!");
    val.x() = in_value.at(0);
    val.y() = in_value.at(1);
}

inline void decode_keyframe_tangent(Vec2D &obj, const JsonObject &in_value)
{
    JsonObject x = in_value["x"];
    obj.x() = x.is_array() ? x.at(0) : x;
    JsonObject y = in_value["y"];
    obj.y() = y.is_array() ? y.at(0) : y;
}

inline JsonObject keyframe_tangent_to_json(const Vec2D &obj)
{
    JsonObject result;
    result["x"] = obj.x();
    result["y"] = obj.y();
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

void decode(core::BezierPath &path, JsonObject &in_value);
JsonObject json_value(const core::BezierPath &value);
void decode(core::Text &text, JsonObject &in_value);

} // namespace alive::json
#endif // JSON_HELPER_H
