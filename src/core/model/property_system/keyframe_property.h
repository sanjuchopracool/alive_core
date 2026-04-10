#ifndef KEYFRAME_PROPERTY_H
#define KEYFRAME_PROPERTY_H

#include "property.h"
#include <model/keyframes/keyframe.h>
#include <utility/value_provider/keyframe_value_provider.h>

namespace inae::core {

template<typename T>
class KeyFrameProperty : public Property<T>
{
public:
    using CRefFrames = const KeyFrames<T> &;
    using RefFrames = KeyFrames<T> &;

    KeyFrameProperty(RefFrames frames)
        : m_frames(std::move(frames))
    {}

    ~KeyFrameProperty() override {}

    std::unique_ptr<PropertyAnimator<T>> create_animator(PropertyUpdateListener *) const override;

private:
    KeyFrames<T> m_frames;
};

template<typename T>
class KeyFramePropertyAnimator : public PropertyAnimator<T>
{
public:
    using CRefProperty = const KeyFrameProperty<T> &;
    using CRefFrames = const KeyFrames<T> &;
    using CRefValueType = const T &;

    KeyFramePropertyAnimator(CRefFrames frames, PropertyUpdateListener *listener)
        : value_provider(frames)
        , m_listener(listener)
    {}

    ~KeyFramePropertyAnimator() override {}
    void update(inae::core::FrameType frame) override
    {
        if (value_provider.has_update(frame)) {
            value_provider.update(frame);
            m_listener->on_update();
        }
    }
    CRefValueType value() const override { return value_provider.value(); }

private:
    KeyFrameValueProvider<T> value_provider;
    PropertyUpdateListener *m_listener;
};

template<typename T>
std::unique_ptr<PropertyAnimator<T>> KeyFrameProperty<T>::create_animator(
    PropertyUpdateListener *listener) const
{
    return std::make_unique<KeyFramePropertyAnimator<T>>(m_frames, listener);
}

} // namespace inae::core

#endif // KEYFRAME_PROPERTY_H
