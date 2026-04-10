#ifndef PRECOMPLAYER_H
#define PRECOMPLAYER_H

#include <Corrade/Containers/Pointer.h>
#include <core/model/layers/layer.h>
namespace alive::model {

class Precomposition;
class PrecompositionLayer : public Layer
{
public:
    PrecompositionLayer(Object *object, int index = -1, const std::string &path = std::string());

    PrecompositionLayer *clone_layer(Object *object, int index) const override;

    const Layers &layers() const;
    Layers &layers();

    const std::string &precomp_path() const { return m_precomp_path; }
    bool set_precomp_path(const std::string &path);
    void sync_in_out_with_precomp();

    PropertyGroup *property_group() override;
    const PropertyGroup *property_group() const;

    void apply_scripts_on_layers(script::ExpressionEngineInterface *engine);

    void on_update(ISubject *s) override;

    Size2Di precomp_size() const;

    bool is_applying_script() const { return m_applying_script; }
private:
    PrecompositionLayer(Object *object, const PrecompositionLayer &other, int index);
    void apply_script();

private:
    std::string m_precomp_path;
    script::ExpressionEngineInterface *m_engine = nullptr;
    bool m_applying_script = false;
    Corrade::Containers::Pointer<Precomposition> m_comp;
};
} // namespace alive::model

#endif // PRECOMPLAYER_H
