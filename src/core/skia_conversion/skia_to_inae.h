#ifndef SKIA_TO_INAE_H
#define SKIA_TO_INAE_H

#include <string>

class SkFontStyle;

namespace inae::skia {
std::string to_string(const SkFontStyle &style);
}

#endif // SKIA_TO_INAE_H
