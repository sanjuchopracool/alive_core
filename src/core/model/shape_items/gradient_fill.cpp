#include "gradient_fill.h"


namespace alive::model {

const ObjectDescriptor &gradient_fill_descriptor()
{
    static const auto gradient_fill_descriptor = ObjectDescriptor("Gradient Fill",
                                                                  ObjectDescriptor::e_GradientFill);
    return gradient_fill_descriptor;
}

GradientFill::GradientFill(Object *object)
    : ShapeItem(ShapeType::e_GradientFill, object, gradient_fill_descriptor())
{}
} // namespace alive::model
