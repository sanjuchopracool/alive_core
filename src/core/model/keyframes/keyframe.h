#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "keyframe_base.h"
#include "keyframe_utility.h"
#include <bezier/bezier.h>

#include <vector>
namespace inae::model {
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

    {}

    const T &value() const { return m_value; }
    void set_value(const T &value) { m_value = value; }
    float interpolated_progress(const KeyFrameTyped *to, FrameTimeType time) const
    {
        if (time < m_time)
            return 0.0f;

        if (to->time() < time)
            return 1.0f;

        if (m_is_hold)
            return 0.0f;

        float progress = core::remap<FrameTimeType>(time, m_time, to->time(), 0, 1);
        if ((not is_null(m_out_tangent)) or (not is_null(to->in_tangent()))) {
            Vec2D out_point = m_out_tangent;
            if (is_null(out_point)) {
                out_point.x = 0;
                out_point.y = 0;
            }

            Vec2D in_point = to->in_tangent();
            if (is_null(in_point)) {
                in_point.x = 1;
                in_point.y = 1;
            }
            bezier::Bezier<3> curve(
                {{0, 0}, {out_point.x, out_point.y}, {in_point.x, in_point.y}, {1, 1}});
            progress = curve.valueAt(progress).y;
        }
        return progress;
    }

private:
    T m_value;
};

} // namespace inae::model

#endif // KEYFRAME_H
