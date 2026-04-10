#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "path_element.h"
#include <include/core/SkPath.h>
#include <core/model/keyframes/keyframe_utility.h>
#include <core/profiler.h>
#include <vector>

namespace alive::core {

class BezierPath
{
public:
    BezierPath() = default;
    ~BezierPath();
    BezierPath(const CurveVertex& vertex);

    void move_to_start_point(const CurveVertex& vertex);
    void add_vertex(const CurveVertex& vertex);
    void close();
    void add_element(const PathElement &element);

    //    void decode(const QJsonValue &in_value);
    const SkPath &path() const;
    bool operator!=(BezierPath &other) const;

public:
    std::vector<PathElement> m_elements;
    bool m_closed = false;
    double m_length = 0.0;

    // calculated
    mutable SkPath m_path;
};

template<>
inline BezierPath interpolate<BezierPath>(const BezierPath &from, const BezierPath &to, float amount)
{
    BezierPath result;
    {
        AutoProfiler p("P");
        int min_count = std::min(from.m_elements.size(), to.m_elements.size());
        for (int i = 0; i < min_count; ++i) {
            auto p1 = from.m_elements[i].m_vertex;
            auto p2 = to.m_elements[i].m_vertex;

            result.add_vertex(interpolate(p1, p2, amount));
        }
    }
    return result;
}
} // namespace alive::core
#endif // BEZIERPATH_H
