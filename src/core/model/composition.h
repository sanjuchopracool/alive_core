#ifndef EAO_COMPOSITION_H
#define EAO_COMPOSITION_H

#include <core/common_types.h>
#include <memory>
#include <core/model/layers/layer.h>
#include <vector>

namespace inae::model {
class CompositionDocument;
class Composition : public Object
{
public:
    using LayerPtr = std::unique_ptr<Layer>;
    using Layers = std::vector<LayerPtr>;

    Composition(std::string name,
                std::string version,
                int width,
                int height,
                KeyFrameTime in_point,
                KeyFrameTime out_point,
                FrameTimeType framerate);
    ~Composition();

    int width() const { return m_width; }
    void set_width(int width);

    int height() const { return m_height; }
    void set_height(int height);

    KeyFrameTime in_point() const { return m_in_point; }
    void set_in_point(KeyFrameTime in_point);
    KeyFrameTime out_point() const { return m_out_point; }
    void set_out_point(KeyFrameTime out_point);
    FrameTimeType framerate() const { return m_framerate; }
    void set_framerate(FrameTimeType framerate);

    auto layers_count() const { return m_layers.size(); }
    Layer *layer_at(int index) { return m_layers[index].get(); }
    const Layer *layer_at(int index) const { return m_layers[index].get(); }
    const Layers &layers() const { return m_layers; }
    int index_of_layer(const Layer *layer) const;
    const std::string &version() const { return m_version; }

    void add_layer_at_index(Layer *layer, int index);
    Layer *remove_layer_at_index(int index);
    void remove_matte_for_layers(int matte_index);
    bool move_layer(int from, int to);

    const std::string &name() const { return m_name; }
    void set_name(const std::string &name);

    // editor specific
    const std::string &get_uuid() const { return m_uuid; }
    void set_uuid(const std::string &uuid);

    int layers_max_index() const;

    void set_show_matte_details(bool show) { m_show_matte_details = show; }
    bool show_matte_details() const { return m_show_matte_details; }

private:
    void set_layers(Layers &layers);

protected:
    std::string m_name;
    std::string m_version;

    int m_width;
    int m_height;

    KeyFrameTime m_in_point;
    KeyFrameTime m_out_point;

    FrameTimeType m_framerate;
    Layers m_layers;

    // editor specific
    std::string m_uuid;
    bool m_show_matte_details = false;

private:
    friend class inae::core::Serializer;
};

std::string get_unique_layer_name(const Composition *comp, const std::string &prefix);
bool is_unique_layer_name(const Composition *comp, const std::string &name);

} // namespace inae::model

#endif // EAO_COMPOSITION_H
