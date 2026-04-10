#include "property_descriptor.h"
#include "property_type.h"

namespace alive::model {

bool PropertyDescriptor::linkable() const
{
    return m_flags & Flags::e_Linked;
}

bool PropertyDescriptor::can_animate() const
{
    return m_type >= PropertyType::e_AnimatiedTypeStart
           && m_type < PropertyType::e_AnimatiedTypeEnd;
}

bool PropertyDescriptor::can_be_scripted() const
{
    return can_animate();
}

} // namespace alive::model
