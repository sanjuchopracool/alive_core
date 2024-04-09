#ifndef KEYFRAMEBASE_H
#define KEYFRAMEBASE_H

#include <core/common_types.h>
#include <vector>

namespace inae::model {
class Property;
class KeyFrame
{
public:
    enum Easing {
        e_None,
        e_Both,
        e_In,
        e_Out,
    };
    KeyFrame(Property *property,
             const Vec3D &in_sp_tangent = Vec3D(),
             const Vec3D &out_sp_tangent = Vec3D());
    KeyFrame(Property *property,
             KeyFrameTime time,
             bool is_hold,
             const Vec2D &in_tangent,
             const Vec2D &out_tangent,
             const Vec3D &in_sp_tangent = Vec3D(),
             const Vec3D &out_sp_tangent = Vec3D());

    virtual ~KeyFrame();

    KeyFrameTime time() const { return m_time; }
    void set_time(KeyFrameTime t) { m_time = t; }

    bool is_hold() const { return m_is_hold; }

    const Vec2D &in_tangent() const { return m_in_tangent; }
    const Vec2D &out_tangent() const { return m_out_tangent; }

    void set_in_tangent(const Vec2D &val)
    {
        m_in_tangent = val;
        update_easing_type();
    }
    void set_out_tangent(const Vec2D &val)
    {
        m_out_tangent = val;
        update_easing_type();
    }

    const Vec3D &spatial_in_tangent() const { return m_spatial_in_tangent; }
    const Vec3D &spatial_out_tangent() const { return m_spatial_out_tangent; }
    Property *property() const { return m_property; }

    Easing easing_type() const;

    // expression
    const std::string &expression() const { return m_expression; }
    void set_expression(const std::string &expression) { m_expression = expression; }
    void enable_expression(bool enable) { m_enable_expression = enable; }
    bool expression_enabled() const { return m_enable_expression; }

protected:
    void update_easing_type();
    Property *m_property = nullptr;
    KeyFrameTime m_time;
    bool m_is_hold;
    Vec2D m_in_tangent = {};
    Vec2D m_out_tangent = {};
    Vec3D m_spatial_in_tangent = {};
    Vec3D m_spatial_out_tangent = {};
    Easing m_easing_type = e_None;
    std::string m_expression;
    bool m_enable_expression = true;
};

using KeyFrames = std::vector<KeyFrame *>;
} // namespace inae::model

#endif // KEYFRAMEBASE_H
