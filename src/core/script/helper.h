#ifndef HELPER_H
#define HELPER_H

namespace sol {
class state;
}

namespace alive::script {
class Helper
{
public:
    static void dump_state(const sol::state &state);
};

} // namespace alive::script

#endif // HELPER_H
