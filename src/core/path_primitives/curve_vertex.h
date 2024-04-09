#ifndef CURVEVERTEX_H
#define CURVEVERTEX_H

#include <core/common_types.h>

namespace inae::core {

class CurveVertex
{
public:
    CurveVertex(const SkPoint &point = SkPoint(),
                const SkPoint &in_point = SkPoint(),
                const SkPoint &out_point = SkPoint())
        : m_point(point)
        , m_in_point(in_point)
        , m_out_point(out_point)
    {

    }

    static CurveVertex from_relative(const SkPoint &point,
                                     const SkPoint &in_point,
                                     const SkPoint &out_point)
    {
        return CurveVertex(point, point + in_point, point + out_point);
    }

    Point in_relative_tangent() const { return (m_point - m_in_point); }
    Point out_relative_tangent() const { return (m_point - m_out_point); }

    CurveVertex reveresed() const
    {
        return  CurveVertex(m_point, m_out_point, m_in_point);
    }

    CurveVertex reveresed(const SkPoint &translation) const
    {
        return  CurveVertex(m_point + translation, m_in_point + translation, m_out_point + translation);
    }

    float distance_to(const CurveVertex &to, int sample = 25) const;

public:
    Point m_point;
    Point m_in_point;
    Point m_out_point;
};

struct SplitResult
{
    CurveVertex start;
    CurveVertex trim_point;
    CurveVertex end;
};

SplitResult split_curve(const CurveVertex &from, const CurveVertex &to, float amount);
SplitResult trim_curve(const CurveVertex &from,
                       const CurveVertex &to,
                       float at_length,
                       float curve_length,
                       int max_samples,
                       float accouracy = 1.0);
CurveVertex interpolate(const CurveVertex &from, const CurveVertex &to, float amount);
}

#endif // CURVEVERTEX_H
