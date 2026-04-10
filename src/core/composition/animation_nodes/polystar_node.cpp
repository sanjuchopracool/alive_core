#include "polystar_node.h"

#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Constants.h>
#include <core/math_utility.h>
#include <core/model/property/dynamic_property_animator.h>
#include <core/model/shape_items/polystar.h>

namespace alive::model {
PolyStarNode::PolyStarNode(const PolyStar *polystar)
    : ShapePathNode(polystar)
    , m_polystar(polystar)
    , m_num_points(create_animator(polystar->points(), this))
    , m_position(create_animator(polystar->position(), this))
    , m_rotation(create_animator(polystar->rotation(), this))
    , m_inner_radius(create_animator(polystar->inner_radius(), this))
    , m_outer_radius(create_animator(polystar->outer_radius(), this))
    , m_inner_roundness(create_animator(polystar->inner_roundness(), this))
    , m_outer_roundness(create_animator(polystar->outer_roundness(), this))
{
}

bool PolyStarNode::update(UpdateContext &context)

{
    bool result = false;
    m_num_points->update(context);
    m_position->update(context);
    m_rotation->update(context);
    m_inner_radius->update(context);
    m_outer_radius->update(context);
    m_inner_roundness->update(context);
    m_outer_roundness->update(context);

    // TODO listen to trim paths also
    if (context.force_update || m_dirty || m_polystar->is_dirty()) {
        create_path();
        m_dirty = false;
        result = true;
    }
    return result;
}

void PolyStarNode::create_path()
{
    m_path.reset();

    if (m_polystar->hidden()) {
        return;
    }

    auto points = m_num_points->value();
    Vec2D position = m_position->value();

    using namespace Magnum::Math;
    {
        int num_points = points;
        auto partial_points = points - num_points;
        bool is_partial = !math::zero(partial_points);
        // if (!is_partial) {
        //     partial_points = 0.0f;
        //     points = num_points;
        // }
        auto half_angle = Constants<float>::pi() / points;
        int direction = 1;
        if (m_polystar->path_direction() != PathDirection::e_CounterClockwise) {
            direction = -1;
        }

        half_angle *= direction;

        auto alpha = Constants<float>::pi() * m_rotation->value() / 180;
        auto outer_radius = m_outer_radius->value();
        auto inner_radius = m_inner_radius->value();
        Vec1D outer_roundness = m_outer_roundness->value() * 0.01f;
        Vec1D inner_roundness = m_inner_roundness->value() * 0.01f;
        bool is_star = m_polystar->polystar_type() == PolyStar::Type::e_Star;

        auto to_degree = [](Vec1D rad) { return rad * 190 / Constants<Vec1D>::pi(); };
        Vec1D tan_out = outer_roundness * half_angle / 2;
        Vec1D tan_in = inner_roundness * half_angle / 2;

        auto partial_radius = inner_radius + (outer_radius - inner_radius) * partial_points;

        Vec1D angle = -Constants<float>::piHalf() + alpha;
        if (is_partial) {
            angle += -half_angle + (partial_points) *half_angle;
        }
        auto initial_angle = angle;

        // star
        if (is_star) {
            auto radius = outer_radius;
            bool rounded = !alive::math::zero(outer_roundness) | !alive::math::zero(inner_roundness);

            auto cos_out = cosf(angle);
            auto sin_out = sinf(angle);

            if (is_partial) {
                radius = partial_radius;
            }

            auto pos_x = radius * cos_out;
            auto pos_y = radius * sin_out;

            auto init_x = pos_x;
            auto init_y = pos_y;

            SkPoint prev_point = {pos_x, pos_y};
            m_path.moveTo(pos_x, pos_y);

            if (is_partial) {
                angle -= (1 - partial_points) * half_angle;
            }

            for (std::size_t i = 0; i < num_points; ++i) {
                auto beta_inner = angle + half_angle;
                float cos_inner = cos(beta_inner);
                float sin_inner = sin(beta_inner);

                //inner point
                auto in = SkPoint::Make(inner_radius * cos_inner, inner_radius * sin_inner);
                bool is_first = (i == 0);
                if (rounded) {
                    SkPoint ct1 = prev_point;
                    Vec1D x = 0;
                    Vec1D y = 0;

                    if (is_first && is_partial) {
                        x = partial_radius;
                        y = tan_out * partial_points * partial_radius;
                    } else {
                        x = outer_radius;
                        y = tan_out * outer_radius;
                    }
                    ct1 = SkPoint{
                        (x * cos_out - y * sin_out),
                        (y * cos_out + x * sin_out),
                    };

                    SkPoint ct2 = in;
                    {
                        Vec1D x = 0;
                        Vec1D y = 0;

                        if (is_first && is_partial) {
                            x = inner_radius;
                            y = -tan_in * partial_points * inner_radius;
                        } else {
                            x = inner_radius;
                            y = -tan_in * inner_radius;
                        }

                        ct2 = SkPoint{
                            (x * cos_inner - y * sin_inner),
                            (y * cos_inner + x * sin_inner),
                        };
                    }

                    m_path.cubicTo(ct1, ct2, in);
                } else {
                    m_path.lineTo(in);
                }
                prev_point = in;

                // outer point
                angle += (half_angle * 2);

                cos_out = cosf(angle);
                sin_out = sinf(angle);

                SkPoint out(outer_radius * cos_out, outer_radius * sin_out);
                if (rounded) {
                    Vec1D x = outer_radius;
                    Vec1D y = -tan_out * outer_radius;

                    SkPoint ct1 = prev_point;
                    {
                        Vec1D ang = angle - half_angle;
                        Vec1D x = inner_radius;
                        Vec1D y = tan_in * inner_radius;
                        float cos_angle = cos(ang);
                        float sin_angle = sin(ang);
                        ct1 = SkPoint{
                            (x * cos_angle - y * sin_angle),
                            (y * cos_angle + x * sin_angle),
                        };
                    }

                    SkPoint ct2{
                        (x * cos_out - y * sin_out),
                        (y * cos_out + x * sin_out),
                    };

                    m_path.cubicTo(ct1, ct2, out);
                } else {
                    m_path.lineTo(out);
                }

                prev_point = out;
            }
            // if partial, add last inner and curve
            if (is_partial) {
                auto beta_inner = angle + half_angle;
                float cos_inner = cos(beta_inner);
                float sin_inner = sin(beta_inner);
                auto in = SkPoint::Make(inner_radius * cos_inner, inner_radius * sin_inner);
                if (rounded) {
                    Vec1D x = outer_radius;
                    Vec1D y = tan_out * outer_radius;

                    SkPoint ct1{
                        (x * cos_out - y * sin_out),
                        (y * cos_out + x * sin_out),
                    };

                    x = inner_radius;
                    y = -tan_in * inner_radius;

                    SkPoint ct2{(x * cos_inner - y * sin_inner), (y * cos_inner + x * sin_inner)};

                    m_path.cubicTo(ct1, ct2, in);

                    if (is_partial) {
                        Vec1D ang = initial_angle - half_angle * partial_points;
                        x = inner_radius;
                        y = tan_in * inner_radius * partial_points;
                        float cos_angle = cos(ang);
                        float sin_angle = sin(ang);
                        ct1 = SkPoint{
                            (x * cos_angle - y * sin_angle),
                            (y * cos_angle + x * sin_angle),
                        };

                        x = partial_radius;
                        y = -tan_out * partial_radius * partial_points;

                        cos_out = cosf(initial_angle);
                        sin_out = sinf(initial_angle);

                        ct2 = SkPoint{(x * cos_out - y * sin_out), (y * cos_out + x * sin_out)};

                        m_path.cubicTo(ct1, ct2, SkPoint(init_x, init_y));
                    }
                } else {
                    m_path.lineTo(in);
                }
            }
        } else {
            auto radius = outer_radius;
            auto cos_out = cosf(angle);
            auto sin_out = sinf(angle);

            auto x = radius * cos_out;
            auto y = radius * sin_out;
            auto init_x = x;
            auto init_y = y;

            bool rounded = !alive::math::zero(outer_roundness);

            m_path.moveTo(x, y);

            if (is_partial) {
                angle -= (1 - partial_points) * half_angle;
            }

            for (std::size_t i = 0; i < num_points; ++i) {
                angle += (half_angle * 2);

                auto prev_cos_out = cos_out;
                auto prev_sin_out = sin_out;
                bool is_first = i == 0;
                bool is_last = (i == (num_points - 1));

                cos_out = cosf(angle);
                sin_out = sinf(angle);
                x = radius * cos_out;
                y = radius * sin_out;

                if (is_partial && is_first) {
                    //TODO partial points fix
                    Vec1D x_t = radius;
                    Vec1D y_t = tan_out * radius * partial_points;
                    SkPoint ct1{(x_t * prev_cos_out - y_t * prev_sin_out),
                                (y_t * prev_cos_out + x_t * prev_sin_out)};

                    y_t = -tan_out * radius * (1 - partial_points);
                    SkPoint ct2{(x_t * cos_out - y_t * sin_out), (y_t * cos_out + x_t * sin_out)};
                    m_path.cubicTo(ct1, ct2, SkPoint(x, y));
                } else {
                    if (rounded) {
                        Vec1D x_t = radius;
                        Vec1D y_t = tan_out * radius;
                        SkPoint ct1{(x_t * prev_cos_out - y_t * prev_sin_out),
                                    (y_t * prev_cos_out + x_t * prev_sin_out)};

                        y_t *= -1;
                        SkPoint ct2{(x_t * cos_out - y_t * sin_out),
                                    (y_t * cos_out + x_t * sin_out)};
                        m_path.cubicTo(ct1, ct2, SkPoint(x, y));
                    } else {
                        if (!is_last || is_partial) {
                            m_path.lineTo(x, y);
                        }
                    }
                }
            }

            if (is_partial) {
                Vec1D x_t = radius;
                Vec1D y_t = tan_out * radius * (1 - partial_points);
                SkPoint ct1{(x_t * cos_out - y_t * sin_out), (y_t * cos_out + x_t * sin_out)};

                y_t = -tan_out * radius * partial_points;
                cos_out = cosf(initial_angle);
                sin_out = sinf(initial_angle);
                SkPoint ct2{(x_t * cos_out - y_t * sin_out), (y_t * cos_out + x_t * sin_out)};
                m_path.cubicTo(ct1, ct2, SkPoint(init_x, init_y));
            }
        }

        m_path.close();
    }
    m_path.offset(position.x(), position.y(), nullptr);
    apply_trim();
}
} // namespace alive::model
