#include "curve_vertex.h"
#include <Magnum/Math/Distance.h>
#include <algorithm>
#include <core/model/keyframes/keyframe_utility.h>
#include <include/core/SkPath.h>
#include <include/core/SkPathMeasure.h>

namespace alive::core {

SplitResult split_curve(const CurveVertex& from, const CurveVertex& to, float amount)
{
    SplitResult result;
    if (amount <= 0)
    {
        result.start = CurveVertex::from_relative(from.m_point, from.in_relative_tangent(), Point2D());
        result.trim_point = CurveVertex::from_relative(from.m_point,
                                                       Point2D(),
                                                       from.out_relative_tangent());
        result.end = to;
        return  result;
    }

    if (amount>=1)
    {
        result.start = from;
        result.trim_point = CurveVertex::from_relative(to.m_point,
                                                       to.in_relative_tangent(),
                                                       Point2D());
        result.end = CurveVertex::from_relative(to.m_point, Point2D(), to.out_relative_tangent());
        return  result;
    }

    if (from.out_relative_tangent().isZero() and to.in_relative_tangent().isZero()) {
        result.start = from;
        result.trim_point = CurveVertex(interpolate<Point2D>(from.m_point, to.m_point, amount));
        result.end = to;
    }

    Point2D a = interpolate<Point2D>(from.m_point, from.m_out_point, amount);
    Point2D b = interpolate(from.m_out_point, to.m_in_point, amount);
    auto c = interpolate(to.m_in_point, to.m_point, amount);
    auto d = interpolate(a, b, amount);
    auto e = interpolate(b, c, amount);
    auto f = interpolate(d, e, amount);
    result.start = CurveVertex(from.m_point, from.m_in_point, a);
    result.trim_point = CurveVertex(f, d, e);
    result.end = CurveVertex(to.m_point, c, to.m_out_point);
    return result;
}

SplitResult trim_curve(const CurveVertex& from, const CurveVertex& to,
                        float at_length, float curve_length,
                        int max_samples, float accouracy)
{
    auto current_position = at_length/curve_length;
    auto results = split_curve(from, to, current_position);

    if (max_samples == 0)
        return  results;

    for(int i = 0; i < max_samples; ++i)
    {
        auto length = results.start.distance_to(results.trim_point);
        const auto length_diff = at_length - length;

        if (length_diff < accouracy)
            return results;

        const auto diff_position = std::max<float>(std::min<float>((current_position / length)
                                                                       * length_diff,
                                                                   current_position * 0.5),
                                                   current_position * -0.5);
        current_position = diff_position + current_position;
        results = split_curve(from, to, current_position);
    }

    return  results;
}

float CurveVertex::distance_to(const CurveVertex &to, int sample) const
{
    if (out_relative_tangent().isZero() && to.in_relative_tangent().isZero())
        return Magnum::Math::Distance::pointPoint(m_point, to.m_point);

    float distance = 0;
    Point2D previous_point = m_point;
    for (int i = 0; i < sample; ++i) {
        auto next_point = split_curve(*this, to, static_cast<float>(i) / static_cast<float>(sample))
                              .trim_point.m_point;
        distance += Magnum::Math::Distance::pointPoint(previous_point, next_point);
        previous_point = next_point;
    }

    distance += Magnum::Math::Distance::pointPoint(previous_point, to.m_point);
    return distance;

    SkPath path;
    path.moveTo(m_point.x(), m_point.y());
    path.cubicTo(m_out_point.x(),
                 m_out_point.y(),
                 to.m_in_point.x(),
                 to.m_in_point.y(),
                 to.m_point.x(),
                 to.m_point.y());
    SkPathMeasure path_measure;
    path_measure.setPath(&path, false);
    return path_measure.getLength();
}

CurveVertex interpolate(const CurveVertex &from, const CurveVertex &to, float amount)
{
    return CurveVertex(interpolate<Point2D>(from.m_point, to.m_point, amount),
                       interpolate<Point2D>(from.m_in_point, to.m_in_point, amount),
                       interpolate<Point2D>(from.m_out_point, to.m_out_point, amount));
}

} // namespace alive::core
