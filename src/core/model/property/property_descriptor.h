#ifndef PROPERTY_DESCRIPTOR_H
#define PROPERTY_DESCRIPTOR_H

#include <string>

namespace inae::model {

enum class PropertyType;

class PropertyDescriptor
{
    // TODO type can also be used for dervied types
public:
    enum Flags { e_NoFlags = 0x0, e_ReadOnly = 0x01, e_Animated = 0x02, e_Scripted = 0x04 };

    template<typename T>
    constexpr PropertyDescriptor(T &&name, const PropertyType type, const int flags = e_NoFlags)
        : m_name(std::forward<T>(name))
        , m_type(type)
        , m_flags(flags)
    {}

    virtual ~PropertyDescriptor() {}
    const std::string &get_name() const { return m_name; }
    inline PropertyType type() const { return m_type; }
    inline int flags() const { return m_flags; }

    bool can_animate() const;
    bool can_be_scripted() const;

    PropertyDescriptor(const PropertyDescriptor &) = delete;
    PropertyDescriptor operator=(const PropertyDescriptor &) = delete;
    PropertyDescriptor(PropertyDescriptor &&) = delete;
    PropertyDescriptor operator=(PropertyDescriptor &&) = delete;

protected:
    const std::string m_name;
    const PropertyType m_type;
    const int m_flags = e_NoFlags;
};

} // namespace inae::model

#endif // PROPERTYDESCRIPTOR_H
