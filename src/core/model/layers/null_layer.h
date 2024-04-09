#ifndef NULLLAYER_H
#define NULLLAYER_H

#include <core/model/layers/layer.h>

namespace inae::model {

class NullLayer : public Layer
{
public:
    NullLayer(Object *object);
};
} // namespace inae::model

#endif // NULLLAYER_H
