#ifndef PATH_ALGORITHM_H
#define PATH_ALGORITHM_H

#include <core/alive_types/common_types.h>

class SkPath;
namespace alive::model {

void trim_path(SkPath &path, Vec1D start, Vec1D end, Vec1D offset);
}

#endif // PATH_ALGORITHM_H
