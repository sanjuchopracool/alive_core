#ifndef SOLIDLAYER_H
#define SOLIDLAYER_H

#include <include/core/SkColor.h>
#include <core/model/layers/layer.h>

namespace inae::model {

class SolidLayer : public Layer
{
public:
    SolidLayer(Object *object, int index = -1);
    ~SolidLayer() override;

    int width() const { return m_width; }
    int height() const { return m_height; }
    const SkColor &color() const { return m_color; }

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
    void set_color(SkColor color)
    {
        if (m_color != color) {
            m_color = color;
            notify_observers();
        }
    }

private:
    SkColor m_color;
    int m_width;
    int m_height;
    friend class inae::core::Serializer;
};
} // namespace inae::model
#endif // SOLIDLAYER_H
