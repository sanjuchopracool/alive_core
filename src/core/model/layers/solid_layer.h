#ifndef SOLIDLAYER_H
#define SOLIDLAYER_H

#include <core/model/layers/layer.h>

namespace alive::model {

class SolidLayer : public Layer
{
public:
    SolidLayer(Object *object, int index = -1);
    ~SolidLayer() override;

    int width() const { return m_width; }
    int height() const { return m_height; }
    const Color4ub &color() const { return m_color; }

    void set_height(int height)
    {
        if (m_height != height) {
            m_height = height;
            notify_observers();
        }
    }
    void set_width(int width)
    {
        if (m_width != width) {
            m_width = width;
            notify_observers();
        }
    }
    void set_color(Color4ub color)
    {
        if (m_color != color) {
            m_color = color;
            notify_observers();
        }
    }

    SolidLayer *clone_layer(Object *object, int index) const override;

private:
    SolidLayer(Object *object, const SolidLayer &other, int index);

private:
    Color4ub m_color;
    int m_width;
    int m_height;
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model
#endif // SOLIDLAYER_H
