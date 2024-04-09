#include "repeater.h"

namespace inae::model {

inline const auto k_repeater_descriptor = ObjectDescriptor("Repeater", ObjectDescriptor::e_Repeater);

Repeater::Repeater(Object *object)
    : ShapeItem(ShapeType::Repeater, object, k_repeater_descriptor)
    , m_transformation(this)
{
    m_transformation.set_position(Vec2D{100, 0});
}
} // namespace inae::model
