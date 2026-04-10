#include "bezier_path.h"

#include <core/utility.h>
#include <core/model/keyframes/keyframe_utility.h>
#include <core/serializer/json_helper.h>

namespace alive::core {

BezierPath::~BezierPath()
{
}

BezierPath::BezierPath(const CurveVertex &vertex)
{
    m_elements.emplace_back(0.0, vertex);
}

void BezierPath::move_to_start_point(const CurveVertex &vertex)
{
    m_elements.clear();
    m_elements.emplace_back(0.0, vertex);
    m_length = 0;
}

void BezierPath::add_vertex(const CurveVertex &vertex)
{
    if (m_elements.size())
    {
        const auto &last = m_elements.back();
        add_element(last.path_element_to(vertex));
    }
    else
    {
        add_element(PathElement(0, vertex));
    }
}

void BezierPath::close()
{
    m_closed = true;
}

void BezierPath::add_element(const PathElement &element)
{
    m_elements.emplace_back(element);
    m_length += element.m_length;
}

const SkPath &BezierPath::path() const
{
    if (m_path.isEmpty())
    {
        auto el_size = m_elements.size();
        if (el_size)
        {
            const auto *prev_el = &m_elements.front();
            m_path.moveTo(prev_el->m_vertex.m_point.x(), prev_el->m_vertex.m_point.y());
            for (int i = 1; i < el_size; ++i)
            {
                const auto *el = &m_elements[i];

                if (prev_el->m_vertex.out_relative_tangent().isZero()
                    and el->m_vertex.in_relative_tangent().isZero()) {
                    m_path.lineTo(el->m_vertex.m_point.x(), el->m_vertex.m_point.y());
                } else {
                    m_path.cubicTo(prev_el->m_vertex.m_out_point.x(),
                                   prev_el->m_vertex.m_out_point.y(),
                                   el->m_vertex.m_in_point.x(),
                                   el->m_vertex.m_in_point.y(),
                                   el->m_vertex.m_point.x(),
                                   el->m_vertex.m_point.y());
                }

                prev_el = el;
            }

            if (m_closed)
                m_path.close();
        }
    }

    return m_path;
}

bool BezierPath::operator!=(BezierPath &other) const
{
    return true;
}

} // namespace alive::core
