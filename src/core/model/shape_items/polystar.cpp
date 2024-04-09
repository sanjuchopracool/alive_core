#include "polystar.h"

namespace inae::model {

const ObjectDescriptor &polystar_descriptor()
{
    static const auto polystar_descriptor = ObjectDescriptor("PolyStar",
                                                             ObjectDescriptor::e_PolyStarPath);
    return polystar_descriptor;
}

//const QStringView center_key = u"p";
//const QStringView rotation_key = u"r";
//const QStringView outer_radius_key = u"or";
//const QStringView outer_roundness_key = u"os";
//const QStringView inner_radius_key = u"ir";
//const QStringView inner_roundness_key = u"is";
//const QStringView star_type_key = u"sy";
//const QStringView direction_key = u"d";
//const QStringView num_points_key = u"pt";

PolyStar::PolyStar()
    : ShapeItem(ShapeType::Star)
{
}

//void PolyStar::decode(QJsonObject &in_obj, QList<QString> &out_messages)
//{
//    Lottie::decode(m_center, in_obj.take(center_key), out_messages);
//    Lottie::decode(m_rotation, in_obj.take(rotation_key), out_messages);
//    Lottie::decode(m_outer_radius, in_obj.take(outer_radius_key), out_messages);
//    Lottie::decode(m_outer_roundness, in_obj.take(outer_roundness_key), out_messages);
//    Lottie::decode(m_inner_radius, in_obj.take(inner_radius_key), out_messages);
//    Lottie::decode(m_inner_roundness, in_obj.take(inner_roundness_key), out_messages);
//    Lottie::decode(m_num_points, in_obj.take(num_points_key), out_messages);
//    m_star_type = static_cast<Type>(in_obj.take(star_type_key).toInt());
//}
} // namespace inae::model
