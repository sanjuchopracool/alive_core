#ifndef EAO_COMPOSITION_H
#define EAO_COMPOSITION_H

#include <core/alive_types/common_types.h>
#include <core/model/layers/layer.h>
#include <core/tagging/tag_manager.h>

namespace alive {

namespace model {
class CompositionDocument;
class PropertyGroup;
class TextLayerNode;
class NullLayer;
class KeyFrame;

static const ObjectDescriptor &precomposition_descriptor()
{
    static const auto precomposition_descriptor
        = ObjectDescriptor("PreComposition", ObjectDescriptor::e_Precomposition);
    return precomposition_descriptor;
}
class Precomposition : public Object
{
public:
    Precomposition(Object *object,
                   std::string name,
                   std::string version,
                   int width,
                   int height,
                   KeyFrameTime in_point,
                   KeyFrameTime out_point,
                   FrameTimeType framerate,
                   const ObjectDescriptor &descriptor = precomposition_descriptor());

    virtual ~Precomposition();

    // editor specific
    const std::string &get_uuid() const { return m_uuid; }
    void set_uuid(const std::string &uuid);

    auto layers_count() const { return m_layers.size(); }
    Layer *layer_at(int index) { return m_layers[index].get(); }
    const Layer *layer_at(int index) const { return m_layers[index].get(); }
    const Layers &layers() const { return m_layers; }
    Layers &layers() { return m_layers; }
    int index_of_layer(const Layer *layer) const;

    void add_layer_at_index(Layer *layer, int index);
    Layer *remove_layer_at_index(int index);
    void remove_matte_for_layers(int matte_index);
    bool move_layer(int from, int to);

    int layers_max_index() const;
    const std::string &version() const { return m_version; }

    Precomposition *clone(Object *object) const override;

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

    void set_show_matte_details(bool show) { m_show_matte_details = show; }
    bool show_matte_details() const { return m_show_matte_details; }

    Color4ub canvas_background_color() const { return m_canvas_bg_color; }
    void set_canvas_background_color(Color4ub colro);

    bool show_background_color() const { return m_show_canvas_bg_color; }
    void set_show_canvas_bg_color(bool show);

    // scripting related
    PropertyGroup *property_group();
    const PropertyGroup *property_group() const;
    Layer *search_layer(const std::string &layer_name);
    TextLayer *search_text_layer(const std::string &layer_name);
    ShapeLayer *search_shape_layer(const std::string &layer_name);
    NullLayer *search_null_layer(const std::string &layer_name);

    bool apply_script_on_change() const { return m_apply_property_on_change; }
    void set_apply_script_on_change(bool enable);

    void set_apply_script(const std::string &str);
    const std::string apply_script() const;

    void on_update(ISubject *subject) override;

    void apply_scripts_on_layers(script::ExpressionEngineInterface *engine);

    TagManager *tag_manager() { return &m_tag_manager; }
    const TagManager *tag_manager() const { return &m_tag_manager; }

    KeyFrame *get_keyframe(const std::string &ev);

protected:
    Precomposition(Object *object, const Precomposition &other);

private:
    void set_layers(Layers &layers);

protected:
    std::string m_version;
    std::string m_uuid;

    int m_width;
    int m_height;

    KeyFrameTime m_in_point;
    KeyFrameTime m_out_point;

    FrameTimeType m_framerate;
    Layers m_layers;

    // editor specific
    Color4ub m_canvas_bg_color = Color4ub{255, 255, 255, 255};
    bool m_show_canvas_bg_color = false;
    bool m_show_matte_details = false;

    Corrade::Containers::Pointer<PropertyGroup> m_prop_group;
    bool m_apply_property_on_change = true;
    std::string m_apply_script;

    script::ExpressionEngineInterface *m_engine = nullptr;
    TagManager m_tag_manager;

private:
    ADD_FRIEND_SERIALIZERS
};

class Composition : public Precomposition
{
public:
    Composition(std::string name,
                std::string version,
                int width,
                int height,
                KeyFrameTime in_point,
                KeyFrameTime out_point,
                FrameTimeType framerate);
    ~Composition();

    Composition *clone(Object *object) const override;

private:
    Composition(const Composition &other);

private:
    ADD_FRIEND_SERIALIZERS
};

std::string get_unique_layer_name(const Composition *comp, const std::string &prefix);
bool is_unique_layer_name(const Composition *comp, const std::string &name);

} // namespace model
} // namespace alive

#endif // EAO_COMPOSITION_H
