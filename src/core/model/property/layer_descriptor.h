#ifndef LAYER_DESCRIPTOR_H
#define LAYER_DESCRIPTOR_H

#include "object_descriptor.h"
#include <core/alive_types/common_types.h>
namespace alive::model {
class LayerDescriptor : public ObjectDescriptor
{
public:

    template<typename Name>
    LayerDescriptor(Name &&name, LayerType type)
        : ObjectDescriptor(name, ObjectDescriptor::e_Layer)
        , m_layer_type(type)
    {}

    ~LayerDescriptor() {}
    LayerType layer_type() const { return m_layer_type; }
    LayerDescriptor(const LayerDescriptor &) = delete;
    LayerDescriptor operator=(const LayerDescriptor &) = delete;
    LayerDescriptor(LayerDescriptor &&) = delete;
    LayerDescriptor operator=(LayerDescriptor &&) = delete;

private:
    LayerType m_layer_type;
};

} // namespace alive::model

#endif // LAYER_DESCRIPTOR_H
