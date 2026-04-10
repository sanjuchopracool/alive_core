#ifndef PATH_NODE_H
#define PATH_NODE_H

#include <include/core/SkPath.h>

namespace alive::model {

class PathNode
{
public:
    const SkPath &path() const { return m_path; }
    virtual ~PathNode() {}

protected:
    SkPath m_path;
};

} // namespace alive::model

#endif // PATH_NODE_H
