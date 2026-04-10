#ifndef OBJECT_DESCRIPTOR_H
#define OBJECT_DESCRIPTOR_H

#include <string>

namespace alive::model {

class ObjectDescriptor
{
public:
    enum ObjectType {
        e_Layer,
        e_Transform,
        e_Composition,
        e_Precomposition,
        e_ContentGroup,
        e_DummyGroup,
        e_PropertyGroup,
        e_Group,
        e_ShapeItemStart,
        e_RectanglePath,
        e_EllipsePath,
        e_PolyStarPath,
        e_ShapePath,
        e_Fill,
        e_GradientFill,
        e_Stroke,
        e_Trim,
        e_ShapeTransformation,
        e_Repeater,
        e_ShapeItemEnd
    };

    ObjectDescriptor(const std::string &name, ObjectType type);
    const std::string &get_name() const { return m_name; }

    ~ObjectDescriptor() {}
    ObjectType object_type() const { return m_object_type; }
    ObjectDescriptor(const ObjectDescriptor &) = delete;
    ObjectDescriptor operator=(const ObjectDescriptor &) = delete;
    ObjectDescriptor(ObjectDescriptor &&) = delete;
    ObjectDescriptor operator=(ObjectDescriptor &&) = delete;

    void set_support_visibility(bool flag) { m_support_visibility = flag; }
    bool support_visibility() const { return m_support_visibility; }

    static bool can_delete(ObjectType type);

private:
    const std::string m_name;
    ObjectType m_object_type;
    bool m_support_visibility = true;
};

} // namespace alive::model

#endif // OBJECT_DESCRIPTOR_H
