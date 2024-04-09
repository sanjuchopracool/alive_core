#ifndef PROPERTY_H
#define PROPERTY_H

#include "property_descriptor.h"
#include <core/common_types.h>
#include <core/model/keyframes/keyframe_base.h>
#include <core/model/property/update_listener.h>
#include <core/model/tree_item.h>

namespace inae::script {
class ExpressionEngineInterface;
}

namespace inae::model {

enum class PropertyAnimationType {
    e_Constant,
    e_KeyFramed,
};

//enum class PropertyValueType {
//    e_None,
//    e_Int,
//    e_Int2D,
//    e_Int3D,
//    e_Vec1D,
//    e_Vec2D,
//    e_Vec3D,
//    e_Color,
//    e_String,
//    e_Object
//};

class Object;
class PropertyDescriptor;

struct KeyFrameStatus
{
    bool at_keyframe = false;
    bool on_left = false;
    bool on_right = false;
    KeyFrameTime previous = -1;
    KeyFrameTime next = -1;
    KeyFrameTime current = -1;
    bool scripted = false;
    bool left_scripted = false;
    bool right_scripted = false;
};

class Property : public CascadeUpdateListener, public TreeItem
{
public:
    Property(Object *object, const PropertyDescriptor &descriptor);
    virtual ~Property() = default;

    inline PropertyType type() const { return m_descriptor.type(); }
    inline int flags() const { return m_descriptor.flags(); }

    inline bool can_animate() const { return m_descriptor.can_animate(); }
    inline bool can_be_scripted() const { return m_descriptor.can_be_scripted(); }
    inline const std::string &property_name() const
    {
        return m_name.empty() ? m_descriptor.get_name() : m_name;
    }
    virtual bool is_animating() const { return false; }

    virtual const KeyFrames &get_keyframes() const
    {
        const static KeyFrames temp;
        return temp;
    }

    virtual KeyFrames &get_keyframes()
    {
        static KeyFrames temp;
        return temp;
    }

    virtual int keyframes_count() const { return 0; }
    virtual void set_keyframes(KeyFrames &keyframes) {}
    virtual void sort_keyframes_by_time() {}
    virtual KeyFrames remove_all_keyframes() { return KeyFrames(); }
    virtual KeyFrame *add_keyframe(KeyFrameTime t) { return nullptr; }
    virtual bool add_keyframe(KeyFrame *frame) { return false; }
    virtual void remove_keyframe(KeyFrame *frame) {}
    virtual KeyFrame *remove_keyframe(KeyFrameTime t) { return nullptr; }
    virtual KeyFrameStatus keyframe_status(KeyFrameTime) const { return {}; }
    virtual KeyFrameTime next_keyframe_time(KeyFrameTime time) const { return time; }
    virtual KeyFrameTime previous_keyframe_time(KeyFrameTime time) const { return time; }

    // scripting support
    virtual KeyFrame *set_expression(const std::string &, KeyFrameTime time) { return nullptr; }
    virtual bool is_valid_expression(script::ExpressionEngineInterface *engine,
                                     const std::string &expression)
    {
        return false;
    }
    virtual const std::string &get_expression(KeyFrameTime time) const
    {
        return inae::empty_string();
    }

    // value type
    virtual PropertyAnimationType value_type() const { return PropertyAnimationType::e_Constant; }
    virtual void set_value_type(PropertyAnimationType type) {}

    //    virtual constexpr PropertyValueType get_property_value_type()
    //    {
    //        return PropertyValueType::e_None;
    //    }

    // To support resuse with a different node name
    void update_property_name(const std::string &name) { m_name = name; }

protected:
    Object *m_object;
    const PropertyDescriptor &m_descriptor;
    std::string m_name;
};

//template<class T>
//static constexpr PropertyValueType property_value_type_from_type() noexcept;

//template<>
//PropertyValueType property_value_type_from_type<int>() noexcept
//{
//    return PropertyValueType::e_Int;
//};

//template<>
//PropertyValueType property_value_type_from_type<float>() noexcept
//{
//    return PropertyValueType::e_Vec1D;
//};

//template<>
//PropertyValueType property_value_type_from_type<SkV3>() noexcept
//{
//    return PropertyValueType::e_Vec3D;
//};

//template<>
//PropertyValueType property_value_type_from_type<SkV2>() noexcept
//{
//    return PropertyValueType::e_Vec2D;
//};

template<typename Type>
class PropertyTemplate : public Property
{
public:
    using value_type = Type;
    using creference = const Type &;
    using reference = Type &;

    PropertyTemplate(Object *object, const PropertyDescriptor &descriptor)
        : Property(object, descriptor)
    {}

    virtual creference get_value() const = 0;
    virtual KeyFrame *set_value(creference value, KeyFrameTime t = 0) = 0;

    //    constexpr PropertyValueType get_property_value_type()
    //    {
    //        return property_value_type_from_type<Type>();
    //    }
};

} // namespace inae::model

#endif // PROPERTY_H
