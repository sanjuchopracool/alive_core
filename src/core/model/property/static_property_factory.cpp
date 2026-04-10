#include "static_property_factory.h"
#include "property_type.h"
#include "static_property.h"
#include <core/font.h>
#include <core/model/property/property_type_information.h>
#include <functional>
#include <map>

namespace alive::model {
namespace {
std::map<alive::model::PropertyType, std::function<alive::model::Property *(bool)>> m_factories;
}

namespace validators {
inline const PropertyValueValidator<float> *size_factor_validator()
{
    static const auto size_factor_validator = MinMaxPropertyValueValidator<float>(0.0f, 1.0f);
    return &size_factor_validator;
}

inline const PropertyValueValidator<float> *positive_float_validator()
{
    static const auto positive_float_validator = MinMaxPropertyValueValidator<float>(0.0f);
    return &positive_float_validator;
}

inline const PropertyValueValidator<int> *positive_int_validator()
{
    static const auto positive_int_validator = MinMaxPropertyValueValidator<int>(0);
    return &positive_int_validator;
}

inline const PropertyValueValidator<core::Font> *font_validator()
{
    static const auto font_validator
        = FunctorValidator<core::Font>([](const core::Font &val) { return val.size >= 0.0; },
                                       [](core::Font &val) {
                                           if (val.size < 0.0) {
                                               val.size = 0.0;
                                               return true;
                                           }
                                           return false;
                                       });
    return &font_validator;
}
} // namespace validators

Property *StaticPropertyFactory::create_static_property(PropertyType type, bool with_reset)
{
    register_factories();
    if (type >= PropertyType::e_StaticPropertyStart && type < PropertyType::e_StaticPropertyEnd) {
        auto found = m_factories.find(type);
        if (found != m_factories.end()) {
            return found->second(with_reset);
        }
    }

    return nullptr;
}

template<typename T>
StaticProperty<T> *create_static_property_function(bool revert_support,
                                                   const PropertyDescriptor &des,
                                                   const PropertyValueValidator<T> *validator
                                                   = nullptr,
                                                   const T &default_value = T{})
{
    if (revert_support) {
        return new StaticPropertyWithReset<T>(nullptr, des, validator, default_value);
    } else {
        return new StaticProperty<T>(nullptr, des, validator, default_value);
    }
}
void StaticPropertyFactory::register_factories()
{
    if (m_factories.empty()) {
        auto des_from_type = [](PropertyType type) -> PropertyDescriptor {
            return PropertyDescriptor(info_from_property_type(type).defaut_name, type);
        };
        static const auto k_text_descriptor = des_from_type(PropertyType::e_StaticSingleLineText);
        static const auto k_color3v_descriptor = des_from_type(PropertyType::e_StaticColor3V);
        static const auto k_color4ub_descriptor = des_from_type(PropertyType::e_StaticColor4ub);
        static const auto k_int_descriptor = des_from_type(PropertyType::e_StaticInt);
        static const auto k_pos_int_descriptor = des_from_type(PropertyType::e_StaticPositveInt);
        static const auto k_float_descriptor = des_from_type(PropertyType::e_StaticFloat);
        static const auto k_pos_float_descriptor = des_from_type(
            PropertyType::e_StaticPositiveFloat);
        static const auto k_size_factor_descriptor = des_from_type(PropertyType::e_StaticSizeFactor);
        static const auto k_font_descriptor = des_from_type(PropertyType::e_Font);
        static const auto k_bool_descriptor = des_from_type(PropertyType::e_Bool);
        static const auto k_direction_descriptor = des_from_type(PropertyType::e_Direction);

        m_factories[PropertyType::e_StaticSingleLineText] = [](bool with_reset) {
            return create_static_property_function<std::string>(with_reset, k_text_descriptor);
        };

        m_factories[PropertyType::e_StaticColor3V] = [](bool with_reset) {
            return create_static_property_function<Color>(with_reset, k_color3v_descriptor);
        };

        m_factories[PropertyType::e_StaticColor4ub] = [](bool with_reset) {
            return create_static_property_function<Color4ub>(with_reset,
                                                             k_color4ub_descriptor,
                                                             nullptr,
                                                             Color4ub(0, 0, 0, 255));
        };

        m_factories[PropertyType::e_StaticInt] = [](bool with_reset) {
            return create_static_property_function<int>(with_reset, k_int_descriptor);
        };

        m_factories[PropertyType::e_StaticPositveInt] = [](bool with_reset) {
            return create_static_property_function<int>(with_reset,
                                                        k_pos_int_descriptor,
                                                        validators::positive_int_validator());
        };

        m_factories[PropertyType::e_StaticFloat] = [](bool with_reset) {
            return create_static_property_function<float>(with_reset, k_float_descriptor);
        };

        m_factories[PropertyType::e_StaticPositiveFloat] = [](bool with_reset) {
            return create_static_property_function<float>(with_reset,
                                                          k_pos_float_descriptor,
                                                          validators::positive_float_validator());
        };

        m_factories[PropertyType::e_StaticSizeFactor] = [](bool with_reset) {
            return create_static_property_function<float>(with_reset,
                                                          k_size_factor_descriptor,
                                                          validators::size_factor_validator());
        };

        m_factories[PropertyType::e_Font] = [](bool with_reset) {
            return create_static_property_function<core::Font>(with_reset,
                                                               k_font_descriptor,
                                                               validators::font_validator());
        };

        m_factories[PropertyType::e_Bool] = [](bool with_reset) {
            return create_static_property_function<bool>(with_reset, k_bool_descriptor);
        };

        m_factories[PropertyType::e_Direction] = [](bool with_reset) {
            return create_static_property_function<int>(with_reset, k_direction_descriptor);
        };
    }
}

} // namespace alive::model
