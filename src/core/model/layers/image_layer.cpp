#include "image_layer.h"

namespace inae::model {

inline const auto k_image_layer_descriptor = LayerDescriptor("Image", LayerType::e_Image);

ImageLayer::ImageLayer(Object *object)
    : Layer(object, k_image_layer_descriptor)
{}

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
} // namespace inae::model
