#ifndef LOTTIE_PARSER_H
#define LOTTIE_PARSER_H

#include "message.h"
#include <Corrade/Containers/Pointer.h>
#include <core/alive_types/common_types.h>
#include <string>
#include <variant>

namespace alive::model {
class Layer;
class Composition;
class Precomposition;
} // namespace alive::model

namespace alive {
using ParserVariant = std::variant<int, float, std::string>;
struct AliveProjectInformation;

} // namespace alive

namespace alive {

class LottieParser
{
public:
    static Corrade::Containers::Pointer<alive::model::Composition> parse_alive(
        const char *data, AliveProjectInformation &out_info, Messages &out_messages);
    static std::string save_alive(const alive::model::Composition *comp,
                                 const AliveProjectInformation &info);

    static std::string save_alive_pre_comp(const model::Composition *parent_comp,
                                          const AliveProjectInformation &info,
                                          const std::vector<model::Layer *> &layers,
                                          const std::string &uuid);

    static Corrade::Containers::Pointer<alive::model::Precomposition> parse_alive_precomp(
        const char *data, AliveProjectInformation &info, Messages &out_messages);

    static Corrade::Containers::Pointer<alive::model::Precomposition> parse_alive_precomp(
        const std::string &file_path, Messages &out_messages);
};

} // namespace alive
#endif // LOTTIE_PARSER_H
