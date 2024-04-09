#include "property_descriptor.h"
#include "property_type.h"

namespace inae::model {

bool PropertyDescriptor::can_animate() const
{
    return m_type >= PropertyType::e_AnimatiedTypeStart;
}

bool PropertyDescriptor::can_be_scripted() const
{
    return can_animate();
}

} // namespace inae::model
