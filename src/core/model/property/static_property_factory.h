#ifndef STATIC_PROPERTY_FACTORY_H
#define STATIC_PROPERTY_FACTORY_H

namespace alive::model {
class Property;
enum class PropertyType;

class StaticPropertyFactory
{
public:
    static Property *create_static_property(PropertyType type, bool with_reset = false);
    static void register_factories();
};
} // namespace alive::model

#endif // STATIC_PROPERTY_FACTORY_H
