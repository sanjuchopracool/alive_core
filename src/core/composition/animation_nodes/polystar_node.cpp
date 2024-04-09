#include "polystar_node.h"

#include "model/shape_items/polystar.h"
#include <cmath>

namespace inae::core {
PolyStarNode::PolyStarNode(const PolyStar &polystar)
    : m_center(polystar.m_center->create_animator(this))
    , m_rotation(polystar.m_rotation->create_animator(this))
    , m_outer_radius(polystar.m_outer_radius->create_animator(this))
    , m_outer_roundness(polystar.m_outer_roundness->create_animator(this))
    , m_inner_radius(polystar.m_inner_radius->create_animator(this))
    , m_inner_roundness(polystar.m_inner_roundness->create_animator(this))
    , m_num_points(polystar.m_num_points->create_animator(this))
{
}

bool PolyStarNode::update(FrameType t, bool force_update)
{
    bool result = false;
    //    if (force_update or need_update(t))
    //    {
    //        result = true;
    //        m_center->update(t);
    //        m_rotation->update(t);
    //        m_outer_radius->update(t);
    //        m_outer_roundness->update(t);
    //        m_inner_radius->update(t);
    //        m_inner_roundness->update(t);
    //        m_num_points->update(t);

    //        m_star_path = QPainterPath();
    //        auto center = m_center->value();
    //        auto inner_radius = m_inner_radius->value();
    //        auto outer_radius = m_outer_radius->value();
    //        auto num_points = m_num_points->value();

    //        auto half_angle = M_PI/num_points;

    ////        qDebug() << inner_radius << outer_radius;
    //        auto angle = half_angle/2;
    //        auto x1 = inner_radius * std::cosf(angle);
    //        auto y1 = inner_radius * std::sinf(angle);
    //        m_star_path.moveTo(x1, y1);
    //        int i = 0;
    //        for(; i < num_points - 1; ++i) {
    //            angle += half_angle;
    //            auto x2 = outer_radius * std::cosf(angle);
    //            auto y2 = outer_radius * std::sinf(angle);
    //            m_star_path.lineTo(x2, y2);

    //            angle += half_angle;
    //            auto x1 = inner_radius * std::cosf(angle);
    //            auto y1 = inner_radius * std::sinf(angle);
    //            m_star_path.lineTo(x1, y1);
    //        }
    //        angle += half_angle;
    //        auto x2 = outer_radius * std::cosf(angle);
    //        auto y2 = outer_radius * std::sinf(angle);
    //        m_star_path.lineTo(x2, y2);
    //        m_star_path.closeSubpath();
    //        m_star_path.translate(center.x(), center.y());

    //    }
    //    m_path = m_star_path;
    return result;
}
}
