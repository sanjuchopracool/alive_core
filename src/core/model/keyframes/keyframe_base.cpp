#include "keyframe_base.h"

namespace inae::model {

KeyFrame::KeyFrame(Property *property, const Vec3D &in_sp_tangent, const Vec3D &out_sp_tangent)
    : m_property(property)
    , m_time(0)
    , m_is_hold(true)
    , m_spatial_in_tangent(in_sp_tangent)
    , m_spatial_out_tangent(out_sp_tangent)
{}

KeyFrame::KeyFrame(Property *property,
                   KeyFrameTime time,
                   bool is_hold,
                   const Vec2D &in_tangent,
                   const Vec2D &out_tangent,
                   const Vec3D &in_sp_tangent,
                   const Vec3D &out_sp_tangent)
    : m_property(property)
    , m_time(time)
    , m_is_hold(is_hold)
    , m_in_tangent(in_tangent)
    , m_out_tangent(out_tangent)
    , m_spatial_in_tangent(in_sp_tangent)
    , m_spatial_out_tangent(out_sp_tangent)
{
    update_easing_type();
}

KeyFrame::~KeyFrame() {}

KeyFrame::Easing KeyFrame::easing_type() const
{
    return m_easing_type;
}

void KeyFrame::update_easing_type()
{
    bool is_in_null = is_null(m_in_tangent);
    bool is_out_null = is_null(m_out_tangent);

    if (is_in_null == is_out_null) {
        if (is_in_null) {
            m_easing_type = e_None;
        } else {
            m_easing_type = e_Both;
        }

    } else if (is_in_null) {
        m_easing_type = e_Out;
    } else {
        m_easing_type = e_In;
    }
}
} // namespace inae::model
