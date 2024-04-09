#ifndef STATICIMAGELAYER_H
#define STATICIMAGELAYER_H

#include <core/model/layers/layer.h>

namespace inae::model {

class ImageLayer : public Layer
{
public:
    ImageLayer(Object *object);

    const std::string &path() const;
    void set_path(const std::string &path);

private:
    std::string m_image_path;
};

} // namespace inae::model

#endif // STATICIMAGELAYER_H
