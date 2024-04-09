#ifndef OBJECT_DESCRIPTOR_H
#define OBJECT_DESCRIPTOR_H

#include "property_descriptor.h"
namespace inae::model {

class ObjectDescriptor : public PropertyDescriptor
{
public:
    enum ObjectType {
        e_Layer,
        e_Transform,
        e_Composition,
        e_ContentGroup,
        e_DummyGroup,
        e_Group,
        e_ShapeItemStart,
        e_RectanglePath,
        e_EllipsePath,
        e_PolyStarPath,
        e_ShapePath,
        e_Fill,
        e_Stroke,
        e_Trim,
        e_ShapeTransformation,
        e_Repeater,
        e_ShapeItemEnd
    };

    ObjectDescriptor(const std::string &name, ObjectType type);

    ~ObjectDescriptor() {}
    ObjectType object_type() const { return m_object_type; }
    ObjectDescriptor(const ObjectDescriptor &) = delete;
    ObjectDescriptor operator=(const ObjectDescriptor &) = delete;
    ObjectDescriptor(ObjectDescriptor &&) = delete;
    ObjectDescriptor operator=(ObjectDescriptor &&) = delete;

    void set_support_visibility(bool flag) { m_support_visibility = flag; }
    bool support_visibility() const { return m_support_visibility; }

private:
    ObjectType m_object_type;
    bool m_support_visibility = true;
};

} // namespace inae::model

#endif // OBJECT_DESCRIPTOR_H
