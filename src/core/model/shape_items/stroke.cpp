#include "stroke.h"

namespace inae::model {

const ObjectDescriptor &stroke_descriptor()
{
    static const auto stroke_descriptor = ObjectDescriptor("Stroke", ObjectDescriptor::e_Stroke);
    return stroke_descriptor;
}

//const QString opacity_key ="o";
//const QString color_key ="c";
//const QString width_key ="w";
//const QString line_cap_key = "lc";
//const QString line_join_key = "lj";
//const QString miter_limit_key = "ml";
//const QString dash_pattern_key = "d";
//const QString fill_enabled_key ="fillEnabled";

//Qt::PenCapStyle int_to_cap_style(int i) {
//    auto result = Qt::RoundCap;
//    switch (i) {
//    case 1:
//        result = Qt::FlatCap;
//        break;
//    case 3:
//        result = Qt::SquareCap;
//        break;
//    default:
//        break;
//    }
//    return result;
//}

//Qt::PenJoinStyle int_to_join_style(int i) {
//    auto result = Qt::RoundJoin;
//    switch (i) {
//    case 1:
//        result = Qt::MiterJoin;
//        break;
//    case 3:
//        result = Qt::BevelJoin;
//        break;
//    default:
//        break;
//    }
//    return result;
//}

Stroke::Stroke(Object *object)
    : ShapeItem(ShapeType::Stroke, object, stroke_descriptor())
{
}

//void Stroke::decode(QJsonObject &in_obj, QList<QString>& out_messages)
//{
//    m_pen.setCapStyle(int_to_cap_style(in_obj.take(line_cap_key).toInt()));
//    m_pen.setJoinStyle(int_to_join_style(in_obj.take(line_join_key).toInt()));
//    m_pen.setMiterLimit(in_obj.take(miter_limit_key).toDouble(4));
//    Lottie::decode(m_opacity, in_obj.take(opacity_key), out_messages);
//    Lottie::decode(m_width, in_obj.take(width_key), out_messages);
//    Lottie::decode(m_color, in_obj.take(color_key), out_messages);
//    //    if (in_obj.contains(dash_pattern_key))
//    //        decode_dashes(m_dashPattern, in_obj.take(dash_pattern_key), out_messages);
//    m_fillEnabled = in_obj.take(fill_enabled_key).toBool();
//}
} // namespace inae::model
