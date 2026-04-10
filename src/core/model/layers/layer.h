#ifndef LAYER_H
#define LAYER_H

#include <Corrade/Containers/Pointer.h>
#include <core/alive_types/common_types.h>
#include <core/model/property/layer_descriptor.h>
#include <core/model/property/object.h>
#include <core/model/property/update_listener.h>
#include <core/model/transform.h>
#include <set>

namespace alive::model {

class TextLayer;
class ShapeLayer;
class NullLayer;
class PropertyGroup;
class PrecompositionLayer;

enum class MatteType { e_None, e_Alpha, e_AlphaInvert, e_Luma, e_LumaInvert, e_Last };
enum class BlendMode {
    e_Normal,
    e_Multiply,
    e_Screen,
    e_Overlay,
    e_Darken,
    e_Lighten,
    e_ColorDodge,
    e_ColorBurn,
    e_HardLight,
    e_SoftLight,
    e_Difference,
    e_Exclusion,
    e_Hue,
    e_Saturation,
    e_Color,
    e_Luminosity,
    e_Add,
    e_HardMix,
    e_Last
};

const std::string &layer_type_name(LayerType type);
class Layer : public Object
{
public:
    Layer(Object *object, const LayerDescriptor &descriptor);
    virtual ~Layer();

    bool in_range(FrameTimeType t) const;
    LayerType layer_type() const;
    KeyFrameTime in_point() const { return m_in_point; }
    KeyFrameTime out_point() const { return m_out_point; }
    KeyFrameTime start_time() const { return m_start_time; }

    const Transform &transform() const { return m_transform; }
    Transform &transform() { return m_transform; }

    bool is_hidden() const { return m_hidden; }
    int parent_layer_index() const { return m_parent_index; }
    bool set_parent_layer_index(int index)
    {
        // restrict matte to itself
        if (m_parent_index != index && index != m_index) {
            m_parent_index = index;
            notify_observers();
            return true;
        }
        return false;
    }

    bool is_visible() const { return !m_hidden; }
    void set_visible(bool visible)
    {
        if (m_hidden == visible) {
            m_hidden = !visible;
            notify_observers();
        }
    }

    bool is_object_visible() const override { return is_visible(); }
    void set_object_visible(bool flag) override { set_visible(flag); }

    void set_in_point(KeyFrameTime in_time)
    {
        m_in_point = in_time;
        notify_observers();
    }
    void set_out_point(KeyFrameTime out_time)
    {
        m_out_point = out_time;
        notify_observers();
    }

    void move_in_out_point(KeyFrameTime delta)
    {
        m_in_point += delta;
        m_out_point += delta;
        notify_observers();
    }

    void set_blend_mode(BlendMode mode)
    {
        if (m_blend_mode != mode) {
            m_blend_mode = mode;
            notify_observers();
        }
    }

    BlendMode blend_mode() const { return m_blend_mode; }

    void set_matte_type(MatteType mode)
    {
        if (m_matte != mode) {
            m_matte = mode;
            notify_observers();
        }
    }
    MatteType matte_type() const { return m_matte; }
    int layer_index() const { return m_index; }

    int matte_layer_index() const { return m_matte_layer_index; }
    bool set_matte_layer_index(int index)
    {
        // restrict matte to itself
        if (m_matte_layer_index != index && index != m_index) {
            m_matte_layer_index = index;
            notify_observers();
            return true;
        }
        return false;
    }

    void add_as_matte_parent_for_layer(int index)
    {
        if (index != m_index) {
            m_matte_subscribers.emplace(index);
        }
    }

    void remove_as_matte_parent_for_layer(int index)
    {
        if (index != m_index) {
            m_matte_subscribers.erase(index);
        }
    }

    int editor_color_index() const { return m_editor_color_index; }
    void set_editor_color_index(int color_index)
    {
        m_editor_color_index = color_index;
        notify_observers();
    }

    bool is_locked() const { return m_locked; }
    void set_lock(bool lock)
    {
        if (m_locked != lock) {
            m_locked = lock;
            notify_observers();
        }
    }

    bool is_blinking() const { return m_blink; }
    void set_blink(bool blink) { m_blink = blink; }

    virtual Layer *clone_layer(Object *obj, int index) const = 0;
    Layer *clone(Object *object) const override;

    const TextLayer *to_text_layer() const;
    TextLayer *to_text_layer();

    const ShapeLayer *to_shape_layer() const;
    ShapeLayer *to_shape_layer();

    const NullLayer *to_null_layer() const;
    NullLayer *to_null_layer();

    const PrecompositionLayer *to_precomp_layer() const;
    PrecompositionLayer *to_precomp_layer();

    virtual PropertyGroup *property_group() { return nullptr; }

protected:
    Layer(Object *object, const Layer &other);

protected:
    bool set_layer_index(int index)
    {
        if (m_index == -1 && m_index != index) {
            m_index = index;
            notify_observers();
            return true;
        }

        return false;
    }

protected:
    Transform m_transform;
    int m_index = -1;
    int m_matte_layer_index = -1;
    KeyFrameTime m_in_point;
    KeyFrameTime m_out_point;
    KeyFrameTime m_start_time;
    int m_parent_index = -1;
    BlendMode m_blend_mode = BlendMode::e_Normal;
    // array masks
    double m_time_stretch;
    MatteType m_matte = MatteType::e_None;
    bool m_hidden = false;
    std::set<int> m_matte_subscribers;

    // editor specific properties
    int m_editor_color_index = 0;
    bool m_locked = false;
    bool m_blink = false;
private:
    ADD_FRIEND_SERIALIZERS
};

using LayerPtr = Corrade::Containers::Pointer<Layer>;
using Layers = std::vector<LayerPtr>;
} // namespace alive::model

#endif // LAYER_H
