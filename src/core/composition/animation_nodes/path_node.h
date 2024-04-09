#ifndef PATH_NODE_H
#define PATH_NODE_H

#include <include/core/SkPath.h>

namespace inae::model {

class PathNode
{
public:
    const SkPath &path() const { return m_path; }
    virtual ~PathNode() {}

protected:
    SkPath m_path;
};

} // namespace inae::model

#endif // PATH_NODE_H
