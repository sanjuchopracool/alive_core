#ifndef PROPERTY_H
#define PROPERTY_H

#include <core/model/property/property_descriptor.h>
#include <core/alive_types/common_types.h>
#include <core/model/keyframes/keyframe_base.h>
#include <core/model/property/update_listener.h>
#include <core/model/tree_item.h>
#include <core/alive_types/class_helpers.h>

namespace alive::script {
class ExpressionEngineInterface;
}

namespace alive::model {

enum class PropertyAnimationType {
    e_Constant,
    e_KeyFramed,
};

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
    // ONLY_CLONABLE(Property)
public:
    Property(Object *object, const PropertyDescriptor &descriptor, const std::string name = {});
    virtual ~Property();

    inline PropertyType type() const { return m_descriptor.type(); }
    inline int flags() const { return m_descriptor.flags(); }

    inline bool can_animate() const { return m_descriptor.can_animate(); }
    inline bool can_be_scripted() const { return m_descriptor.can_be_scripted(); }
    inline const std::string &property_name() const
    {
        return m_name.empty() ? m_descriptor.get_name() : m_name;
    }

    const std::string &name() const override { return property_name(); }

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

    virtual void mark_dirty() {}
    virtual int keyframes_count() const { return 0; }
    virtual void set_keyframes(KeyFrames &keyframes) {}
    virtual bool sort_keyframes_by_time() { return false; }
    virtual KeyFrames remove_all_keyframes() { return KeyFrames(); }
    virtual KeyFrame *add_keyframe(KeyFrameTime t) { return nullptr; }
    virtual bool add_keyframe(KeyFrame *frame) { return false; }
    virtual void remove_keyframe(KeyFrame *frame) {}
    virtual KeyFrame *remove_keyframe(KeyFrameTime t) { return nullptr; }
    virtual KeyFrame *keyframe_at(KeyFrameTime t) { return nullptr; }
    virtual bool update_keyframe_time(KeyFrame *keyframe, KeyFrameTime t) { return false; }
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
        return alive::empty_string();
    }

    virtual void enable_expression(bool enable) { }
    virtual bool expression_enabled() const { return false; }

    // value type
    virtual PropertyAnimationType value_type() const { return PropertyAnimationType::e_Constant; }
    virtual void set_value_type(PropertyAnimationType type) {}

    //    virtual constexpr PropertyValueType get_property_value_type()
    //    {
    //        return PropertyValueType::e_None;
    //    }

    void set_name(const std::string &name) override;

    virtual Property *clone(Object *obj = nullptr) const = 0;

    // some properties can be linked
    bool linkable() const;
    virtual bool is_linked() const { return false; }
    virtual bool set_linked(bool) { return false; }

    bool is_static() const;

    // we may have multiple properties with same name inside a property group
    // e.g. "Text" inside different Sections for T1 and T2
    // to identify them, we can set different id
    const std::string id() const { return m_id; }
    void set_id(const std::string &id);
    virtual bool assign_value_from(const Property *other) { return false; }

protected:
    Object *m_object;
    const PropertyDescriptor &m_descriptor;
    std::string m_name;
    std::string m_id;
};

} // namespace alive::model

#endif // PROPERTY_H
