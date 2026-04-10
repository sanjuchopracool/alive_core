#ifndef STATICIMAGELAYER_H
#define STATICIMAGELAYER_H

#include <core/model/layers/layer.h>

namespace alive::model {

class ImageLayer : public Layer
{
public:
    ImageLayer(Object *object, int index = -1);

    const std::string &path() const;
    void set_path(const std::string &path);

    ImageLayer *clone_layer(Object *object, int index) const override;

private:
    ImageLayer(Object *object, const ImageLayer &other, int index);

private:
    std::string m_image_path;
};

} // namespace alive::model

#endif // STATICIMAGELAYER_H
