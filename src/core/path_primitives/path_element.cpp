#include "path_element.h"

namespace inae::core {

SplitElementResult split_element_at(const PathElement &to, const PathElement &from, float at_length)
{
    SplitElementResult result;
    auto trim_results = trim_curve(from.m_vertex, to.m_vertex, at_length, to.m_length, 3);
    result.left_span.start = PathElement(from.m_length, CurveVertex(from.m_vertex.m_point,
                                                                    from.m_vertex.m_in_point,
                                                                    trim_results.start.m_out_point));

    result.left_span.end = result.left_span.start.path_element_to(trim_results.trim_point);

    result.right_span.start = PathElement(0.0, trim_results.trim_point);
    result.right_span.end = result.right_span.start.path_element_to(trim_results.end);
    return  result;
}

}
