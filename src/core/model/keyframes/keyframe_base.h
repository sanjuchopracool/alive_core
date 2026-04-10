#ifndef KEYFRAMEBASE_H
#define KEYFRAMEBASE_H

#include <Magnum/Math/Bezier.h>
#include <core/alive_types/common_types.h>
#include <core/tagging/taggable.h>
#include <core/alive_types/class_helpers.h>
#include <vector>

struct LottieInterpolator;
namespace alive::model {
class Property;
class KeyFrame : public Taggable
{
    NON_COPYABLE(KeyFrame);
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
    void set_time(
        KeyFrameTime t)
    {
        m_time = t;
        update_name();
    }

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

    bool is_selected_in_editor() const { return m_selected; }
    void select_in_editor(bool select) const { m_selected = select; }

    float interpolated_progress(const KeyFrame *to, KeyFrameTime time) const;

    virtual KeyFrame *clone(Property *property) = 0;
    const std::string &name() const;

private:
    void update_name();

protected:
    void update_easing_type();
    Property *m_property = nullptr;
    KeyFrameTime m_time;
    bool m_is_hold;
    Vec2D m_in_tangent = {0.25, 0.25};
    Vec2D m_out_tangent = {0.75, 0.75};
    Vec3D m_spatial_in_tangent = {};
    Vec3D m_spatial_out_tangent = {};
    Easing m_easing_type = e_None;
    std::string m_expression;
    bool m_enable_expression = true;

    // editor specific property
    mutable bool m_selected = false;
    mutable const LottieInterpolator *m_interpolator = nullptr;
    mutable Vec2D m_to_in_tangent;
    mutable Vec2D m_last_out_tangent;

    std::string m_name;

protected:
    KeyFrame(Property *property, const KeyFrame &other);
};

using KeyFrames = std::vector<KeyFrame *>;
} // namespace alive::model

#endif // KEYFRAMEBASE_H
