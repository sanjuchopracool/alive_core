#ifndef PROPERTY_MACROS_H
#define PROPERTY_MACROS_H

#include "dynamic_property.h"
#include "property.h"

namespace inae::model {

//---------------------------------------------------------------------------//
#define ADD_PROPERTY_IMPL(type, name) \
public: \
    type get_##name() const \
    { \
        return m_##name.get_value(); \
    } \
    bool set_##name(const type &v) \
    { \
        return m_##name.set_value(v); \
    } \
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
#define ADD_DYNAMIC_PROPERTY(type, name, descriptor) \
private: \
    DynamicProperty<type> m_##name{this, descriptor}; \
    ADD_PROPERTY_IMPL(type, name) \
public: \
    const DynamicProperty<type> &name() const \
    { \
        return m_##name; \
    } \
    DynamicProperty<type> &name() \
    { \
        return m_##name; \
    }

//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
#define ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(type, name, descriptor, default_value) \
private: \
    DynamicProperty<type> m_##name{this, descriptor, default_value}; \
    ADD_PROPERTY_IMPL(type, name) \
public: \
    const DynamicProperty<type> &name() const \
    { \
        return m_##name; \
    }

//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
#define ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(type, name, descriptor, default_value, validator) \
private: \
    DynamicProperty<type> m_##name{this, descriptor, default_value, validator}; \
    ADD_PROPERTY_IMPL(type, name) \
public: \
    const DynamicProperty<type> &name() const \
    { \
        return m_##name; \
    }

//---------------------------------------------------------------------------//

} // namespace inae::model

#endif // PROPERTY_MACROS_H
