#ifndef CURVEVERTEX_H
#define CURVEVERTEX_H

#include <core/alive_types/common_types.h>

namespace alive::core {

class CurveVertex
{
public:
    CurveVertex(const Point2D &point = Point2D(),
                const Point2D &in_point = Point2D(),
                const Point2D &out_point = Point2D())
        : m_point(point)
        , m_in_point(in_point)
        , m_out_point(out_point)
    {

    }

    static CurveVertex from_relative(const Point2D &point,
                                     const Point2D &in_point,
                                     const Point2D &out_point)
    {
        return CurveVertex(point, point + in_point, point + out_point);
    }

    Point2D in_relative_tangent() const { return (m_point - m_in_point); }
    Point2D out_relative_tangent() const { return (m_point - m_out_point); }

    CurveVertex reveresed() const
    {
        return  CurveVertex(m_point, m_out_point, m_in_point);
    }

    CurveVertex reveresed(const Point2D &translation) const
    {
        return  CurveVertex(m_point + translation, m_in_point + translation, m_out_point + translation);
    }

    float distance_to(const CurveVertex &to, int sample = 25) const;

public:
    Point2D m_point;
    Point2D m_in_point;
    Point2D m_out_point;
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
