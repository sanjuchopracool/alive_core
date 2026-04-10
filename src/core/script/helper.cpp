#include "helper.h"
#include <core/logging/logger.h>
#include <sol/sol.hpp>

namespace alive::script {

template<typename T>
void helper(const T &table, int indent)
{
    if constexpr (std::is_same_v<T, sol::table>) {
        if (!table.valid()) {
            return;
        }
    }

    for (auto &element : table) {
        const sol::object &key = element.first;
        const sol::object &value = element.second;
        std::string k = key.as<std::string>();
        sol::type v_t = value.get_type();
        sol::type k_t = value.get_type();

        std::string filler;
        filler.resize(indent, ' ');
        switch (v_t) {
        case sol::type::string:
            ALIVE_CORE_DEBUG("{}{} : {}", filler, k, value.as<std::string>());
            break;
        case sol::type::number:
            ALIVE_CORE_DEBUG("{}{} : {}", filler, k, value.as<double>());
            break;
        case sol::type::userdata: {
            {
                ALIVE_CORE_DEBUG("User Data: {}{}", filler, k);
                // const sol::optional<sol::table> &t2 = table[k];
                // if (t2) {
                //     helper(t2.value(), indent + 3);
                // }
            }
        } break;
        case sol::type::table: {
            ALIVE_CORE_DEBUG("Table Data: {}{}", filler, k);
            // const sol::optional<sol::table> &t2 = table[k];
            // if (t2) {
            //     helper(t2.value(), indent + 3);
            // }
        } break;
        default:
            ALIVE_CORE_DEBUG("{}{} : Unknown Type", filler, k);
            break;
        }
    }
}

void Helper::dump_state(const sol::state &state)
{
    // helper<sol::state>(state, 0);
}

} // namespace alive::script
