#ifndef COMPOUNDTRIMPATH_H
#define COMPOUNDTRIMPATH_H

#include <vector>

class SkPath;

namespace inae::model {

class TrimNode;
class CompoundTrimPath
{
public:
    CompoundTrimPath();
    ~CompoundTrimPath();
    void add_trim_node(const TrimNode *trim);
    void clear() { m_trims.clear(); }

    void apply_trim(SkPath &path);

private:
    std::vector<const TrimNode *> m_trims;
};

} // namespace inae::model

#endif // COMPOUNDTRIMPATH_H
