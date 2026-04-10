#include <core/model/keyframes/keyframe_base.h>
#include <core/model/keyframes/keyframe_utility.h>
#include <core/model/keyframes/lottie_interpretor.h>
#include <core/model/property/property.h>

namespace alive::model {

KeyFrame::KeyFrame(Property *property, const Vec3D &in_sp_tangent, const Vec3D &out_sp_tangent)
    : m_property(property)
    , m_time(0)
    , m_is_hold(true)
    , m_spatial_in_tangent(in_sp_tangent)
    , m_spatial_out_tangent(out_sp_tangent)
{
    update_name();
}

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
    update_name();
}

KeyFrame::~KeyFrame() {}

KeyFrame::Easing KeyFrame::easing_type() const
{
    return m_easing_type;
}

float KeyFrame::interpolated_progress(const KeyFrame *to, KeyFrameTime time) const
{
    if (time < m_time)
        return 0.0f;

    if (to->time() < time)
        return 1.0f;

    if (m_is_hold)
        return 0.0f;

    float progress = core::remap<FrameTimeType>(time, m_time, to->time(), 0, 1);

    // TODO this have performance implications to check 3 things always
    // can be avoided , if we can manage interpolation setter seperately
    if (!m_interpolator || m_last_out_tangent != m_out_tangent
        || to->in_tangent() != m_to_in_tangent) {
        m_interpolator = get_interpolator(to->in_tangent(), m_out_tangent);
        m_to_in_tangent = to->in_tangent();
        m_last_out_tangent = m_out_tangent;
    }
    // before m_interpolator = get_interpolator(m_in_tangent, m_out_tangent);
    return m_interpolator->progress(progress);
    // m_curve[1] = m_out_tangent;
    // m_curve[2] = to->in_tangent();
    // return static_cast<float>(m_curve.value(progress)[1]);
}

const std::string &KeyFrame::name() const
{
    return m_name;
}

void KeyFrame::update_name()
{
    m_name = m_property->path() + "_k_" + std::to_string(m_time);
}

void KeyFrame::update_easing_type()
{
    // m_curve[0] = {0.0f, 0.0f};
    // m_curve[3] = {1.0f, 1.0f};

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

KeyFrame::KeyFrame(Property *property, const KeyFrame &other)
    : Taggable(other)
    , m_property(property)
    , m_time(other.m_time)
    , m_is_hold(other.m_is_hold)
    , m_in_tangent(other.m_in_tangent)
    , m_out_tangent(other.m_out_tangent)
    , m_spatial_in_tangent(other.m_spatial_in_tangent)
    , m_spatial_out_tangent(other.m_spatial_out_tangent)
    , m_easing_type(other.m_easing_type)
    , m_expression(other.m_expression)
    , m_enable_expression(other.m_enable_expression)
{
    update_name();
}
} // namespace alive::model
