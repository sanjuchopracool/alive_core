#ifndef PROPERTY_TYPE_INFORMATION_H
#define PROPERTY_TYPE_INFORMATION_H

#include <string>

namespace alive {
namespace model {
struct PropertyTypeInformation
{
    std::string type_name;
    std::string defaut_name;
    std::string hint;
};
} // namespace model
} // namespace alive
#endif // PROPERTY_TYPE_INFORMATION_H
