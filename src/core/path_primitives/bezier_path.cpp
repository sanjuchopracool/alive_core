#include "bezier_path.h"

#include <core/utility.h>
#include <core/model/keyframes/keyframe_utility.h>
#include <core/serializer/json_helper.h>

namespace inae::core {

namespace {
const char k_closed_key[] = "c";
const char k_in_points_key[] = "i";
const char k_out_points_key[] = "o";
const char k_vertices_key[] = "v";
}

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

void BezierPath::decode(json::JsonObject &in_value)
{
    using namespace inae::json;
    m_closed = json_pop(in_value, k_closed_key);
    auto vertices_array = json_pop(in_value, k_vertices_key);
    auto in_points_array = json_pop(in_value, k_in_points_key);
    auto out_points_array = json_pop(in_value, k_out_points_key);

    assert(vertices_array.size() == in_points_array.size());
    assert(out_points_array.size() == in_points_array.size());
    int size = vertices_array.size();
    if (!size)
        return;

    auto json_to_point = [](JsonObject &data) {
        assertm(data.is_array(), "vertices index should");
        float x = data.at(0);
        float y = data.at(1);
        return SkPoint::Make(x, y);
    };
    auto first_vertex = CurveVertex::from_relative(json_to_point(vertices_array.at(0)),
                                                   json_to_point(in_points_array.at(0)),
                                                   json_to_point(out_points_array.at(0)));

    auto previous_element = PathElement(0, first_vertex);

    std::vector<PathElement> decoded_elements;
    decoded_elements.reserve(size);
    decoded_elements.push_back(previous_element);

    double total_length = 0;
    for (int i = 1; i < size; ++i) {
        auto vertex = CurveVertex::from_relative(json_to_point(vertices_array.at(i)),
                                                 json_to_point(in_points_array.at(i)),
                                                 json_to_point(out_points_array.at(i)));

        const auto path_element = previous_element.path_element_to(vertex);
        decoded_elements.emplace_back(path_element);
        total_length += path_element.m_length;
        previous_element = path_element;
    }

    if (m_closed) {
        auto close_element = previous_element.path_element_to(first_vertex);
        decoded_elements.emplace_back(close_element);
        total_length += close_element.m_length;
    }

    m_length = total_length;
    m_elements = decoded_elements;
}

const SkPath &BezierPath::path() const
{
    if (m_path.isEmpty())
    {
        auto el_size = m_elements.size();
        if (el_size)
        {
            const auto *prev_el = &m_elements.front();
            m_path.moveTo(prev_el->m_vertex.m_point);
            for (int i = 1; i < el_size; ++i)
            {
                const auto *el = &m_elements[i];

                if (prev_el->m_vertex.out_relative_tangent().isZero()
                    and el->m_vertex.in_relative_tangent().isZero()) {
                    m_path.lineTo(el->m_vertex.m_point);
                } else {
                    m_path.cubicTo(prev_el->m_vertex.m_out_point,
                                   el->m_vertex.m_in_point,
                                   el->m_vertex.m_point);
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

} // namespace inae::core
