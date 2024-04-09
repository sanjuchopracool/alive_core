#ifndef PATHELEMENT_H
#define PATHELEMENT_H

#include "curve_vertex.h"

namespace inae::core {

class PathElement
{
public:
    PathElement(double length = 0.0, const CurveVertex& vertex = CurveVertex())
        : m_length(length),
          m_vertex(vertex)
    {

    }

    PathElement path_element_to(const CurveVertex &to) const
    {
        return  PathElement(m_vertex.distance_to(to), to);
    }

    PathElement update_vertex(const CurveVertex& to)
    {
        return  PathElement(m_length, to);
    }

public:
    double m_length;
    CurveVertex m_vertex;
};

struct PathElementSpan
{
    PathElement start;
    PathElement end;
};

struct SplitElementResult
{
    PathElementSpan left_span;
    PathElementSpan right_span;
};

SplitElementResult split_element_at(const PathElement &to, const PathElement &from, float at_length);
}
#endif // PATHELEMENT_H
