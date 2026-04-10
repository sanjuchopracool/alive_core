#include "image_layer.h"

namespace alive::model {

inline const auto k_image_layer_descriptor = LayerDescriptor("Image", LayerType::e_Image);

ImageLayer::ImageLayer(Object *object, int index)
    : Layer(object, k_image_layer_descriptor)
{
    set_layer_index(index);
}

const std::string &ImageLayer::path() const
{
    return m_image_path;
}

void ImageLayer::set_path(const std::string &path)
{
    if (m_image_path != path) {
        m_image_path = path;
        notify_observers();
    }
}

ImageLayer *ImageLayer::clone_layer(Object *object, int index) const
{
    return new ImageLayer(object, *this, index);
}

ImageLayer::ImageLayer(Object *object, const ImageLayer &other, int index)
    : Layer(object, other)
    , m_image_path(other.m_image_path)
{
    set_layer_index(index);
}
} // namespace alive::model
