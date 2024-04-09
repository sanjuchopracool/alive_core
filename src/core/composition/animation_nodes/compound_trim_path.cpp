#include "compound_trim_path.h"
#include "path_algorithm.h"
#include "trim_node.h"
namespace inae::model {

CompoundTrimPath::CompoundTrimPath() {}

CompoundTrimPath::~CompoundTrimPath() {}

void CompoundTrimPath::add_trim_node(const TrimNode *trim)
{
    if (trim)
        m_trims.emplace_back(trim);
}

void CompoundTrimPath::apply_trim(SkPath &path)
{
    Vec1D start = 1;
    Vec1D end = 1;
    Vec1D offset = 1.0;

    bool apply_trim = false;
    // TODO assuming simultaneous trim
    for (const TrimNode *trim : m_trims) {
        if (trim->hidden()) {
            continue;
        }
        apply_trim = true;
        start *= trim->start() / 100.0f;
        end *= trim->end() / 100.0f;
        offset *= trim->offset() / 360.f;
    }

    if (apply_trim) {
        trim_path(path, start, end, offset);
    }
}

} // namespace inae::model
