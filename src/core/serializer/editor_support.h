#ifndef EDITOR_SUPPORT_H
#define EDITOR_SUPPORT_H

#include <core/alive_types/common_types.h>

namespace alive {
struct AliveProjectInformation
{
    alive::KeyFrameTime m_range_start;
    alive::KeyFrameTime m_range_end;
    alive::KeyFrameTime m_play_range_start;
    alive::KeyFrameTime m_play_range_end;
    alive::KeyFrameTime m_current_frame = {0};

    // play ground script
    std::string m_playground_script;
};
} // namespace alive
#endif // EDITOR_SUPPORT_H
