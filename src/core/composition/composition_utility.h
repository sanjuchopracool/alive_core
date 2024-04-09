#ifndef COMPOSITION_UTILITY_H
#define COMPOSITION_UTILITY_H

#include <optional>

enum class SkBlendMode;
namespace inae::model {
enum class BlendMode;
std::optional<SkBlendMode> to_sk_blend_mode(BlendMode in_mode);
} // namespace inae::model
#endif // COMPOSITION_UTILITY_H
