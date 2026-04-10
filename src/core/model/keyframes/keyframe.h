#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <core/model/keyframes/keyframe_base.h>

namespace alive::model {
template<typename T>
class KeyFrameTyped : public KeyFrame
{
public:
    KeyFrameTyped(Property *property,
                  const T &value,
                  const Vec3D &in_sp_tangent = Vec3D(),
                  const Vec3D &out_sp_tangent = Vec3D())
        : KeyFrame(property, in_sp_tangent, out_sp_tangent)
        , m_value(value)
    {}

    KeyFrameTyped(Property *property,
                  const T &value,
                  KeyFrameTime time,
                  bool is_hold,
                  const Vec2D &in_tangent,
                  const Vec2D &out_tangent,
                  const Vec3D &in_sp_tangent = Vec3D(),
                  const Vec3D &out_sp_tangent = Vec3D())
        : KeyFrame(property, time, is_hold, in_tangent, out_tangent, in_sp_tangent, out_sp_tangent)
        , m_value(value)
    {
    }

    const T &value() const { return m_value; }
    void set_value(const T &value) { m_value = value; }

    KeyFrame *clone(Property *property) override { return new KeyFrameTyped(property, *this); }

private:
    T m_value;

private:
    KeyFrameTyped(Property *property, const KeyFrameTyped &other)
        : KeyFrame(property, other)
        , m_value(other.m_value)
    {}
};

} // namespace alive::model

#endif // KEYFRAME_H
