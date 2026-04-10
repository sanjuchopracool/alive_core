#include "path_algorithm.h"

#include <include/core/SkPath.h>
#include <include/core/SkPathMeasure.h>
#include <include/core/SkPoint.h>
#include <core/profiler.h>
#include <minitrace.h>

namespace alive::model {

void cubic_split(SkPoint *points, Vec1D t, bool from_start = true)
{
    //https://www.geogebra.org/m/WPHQ9rUt
    SkPoint p1 = points[0];
    SkPoint p2 = points[1];
    SkPoint p3 = points[2];
    SkPoint p4 = points[3];
    auto p5 = p1 * (1 - t) + p2 * t;
    auto p6 = p2 * (1 - t) + p3 * t;
    auto p7 = p3 * (1 - t) + p4 * t;
    auto p8 = p5 * (1 - t) + p6 * t;
    auto p9 = p6 * (1 - t) + p7 * t;
    auto bz = p8 * (1 - t) + p9 * t;
    if (from_start) {
        points[0] = bz;
        points[1] = p9;
        points[2] = p7;
    } else {
        points[1] = p5;
        points[2] = p8;
        points[3] = bz;
    }
}
void cubic_split(SkPoint *points, Vec1D t1, Vec1D t2)
{
    cubic_split(points, t1, true);
    t2 = (t2 - t1) / (1 - t1);
    cubic_split(points, t2, false);
}

void trim_path(SkPath &path, Vec1D start, Vec1D end, Vec1D offset)
{
    MTR_SCOPE("TRIM_PATH", "MAIN");
    if (std::abs(end - start) > std::numeric_limits<float>::epsilon()) {
        if (std::abs(end - start - 1) > std::numeric_limits<float>::epsilon()) {
            SkPath result;
            SkPathMeasure path_measure;
            path_measure.setPath(&path, false);
            Vec1D total_length = path_measure.getLength();
            Vec1D start_length = total_length * start;
            Vec1D end_length = total_length * end;
            if (start_length > end_length)
                std::swap(start_length, end_length);

            bool rotated = false;
            Vec1D offset_length = offset * total_length;
            if (offset_length >= total_length) {
                offset_length = offset_length
                                - (static_cast<int>(offset_length / total_length) * total_length);
            }
            if (offset != 0.0f) {
                start_length += offset_length;
                end_length += offset_length;
                if (start_length > total_length) {
                    start_length -= static_cast<int>(start_length / total_length) * total_length;
                }

                if (end_length > total_length) {
                    end_length -= static_cast<int>(end_length / total_length) * total_length;
                }

                if (start_length > end_length) {
                    rotated = true;
                }
            }

            SkPoint points[4] = {};
            Vec1D length_so_far = 0;
            bool found_start = false;
            auto evaluate_line_segment = [&]() -> bool {
                bool is_start_seg = false;
                bool is_end_seg = false;
                SkPath current_path;
                current_path.moveTo(points[0]);
                current_path.lineTo(points[1]);
                SkPathMeasure current_path_measure;
                current_path_measure.setPath(&current_path, false);
                Vec1D seg_length = current_path_measure.getLength();

                if (seg_length < std::numeric_limits<float>::epsilon()) {
                    return false;
                }

                Vec1D length_after_seg = length_so_far + seg_length;
                if (!found_start && length_so_far <= start_length
                    && start_length < length_after_seg) {
                    is_start_seg = true;
                    found_start = true;
                }

                bool find_end = (!rotated || found_start);
                if (find_end && length_so_far <= end_length && end_length <= length_after_seg) {
                    is_end_seg = true;
                }

                if (is_start_seg) {
                    // both start and end in this segment
                    // start in this segment but not end
                    Vec1D start_percent = (start_length - length_so_far) / seg_length;
                    points[0] = points[0] + (points[1] - points[0]) * start_percent;
                    if (is_end_seg) {
                        Vec1D end_percent = (end_length - length_so_far) / seg_length;
                        points[1] = points[0] + (points[1] - points[0]) * end_percent;
                    }
                    result.moveTo(points[0]);
                } else if (found_start && !is_end_seg) { // already started but not end
                } else if (is_end_seg) {                 // end found
                    Vec1D end_percent = (end_length - length_so_far) / seg_length;
                    points[1] = points[0] + (points[1] - points[0]) * end_percent;
                }
                if (found_start)
                    result.lineTo(points[1]);
                length_so_far = length_after_seg;

                return is_end_seg;
            };

            auto evaluate_conic_segment = [&](SkPath::Verb verb, SkScalar conic_weight) -> bool {
                bool is_start_seg = false;
                bool is_end_seg = false;

                SkPath current_path;
                current_path.moveTo(points[0]);
                if (verb == SkPath::kConic_Verb) {
                    current_path.conicTo(points[1], points[2], conic_weight);
                } else if (verb == SkPath::kCubic_Verb) {
                    current_path.cubicTo(points[1], points[2], points[3]);
                }

                SkPathMeasure current_path_measure;
                current_path_measure.setPath(&current_path, false);
                Vec1D seg_length = current_path_measure.getLength();
                Vec1D length_after_seg = length_so_far + seg_length;
                if (!found_start && length_so_far <= start_length
                    && start_length < length_after_seg) {
                    is_start_seg = true;
                }

                bool find_end = (!rotated || found_start);
                if (find_end && length_so_far <= end_length && end_length <= length_after_seg) {
                    is_end_seg = true;
                }

                SkScalar segment_start = 0.0f;
                SkScalar segment_end = seg_length;

                auto add_split_segment = [&]() {
                    SkPath dest_path;
                    if (current_path_measure.getSegment(segment_start,
                                                        segment_end,
                                                        &dest_path,
                                                        true)) {
                        result.addPath(dest_path, SkPath::kExtend_AddPathMode);
                    } else {
                        assertm(false, "Segment split issues in trim");
                    }
                };

                if (is_start_seg) {
                    // both start and end in this segment
                    // start in this segment but not end
                    found_start = true;
                    segment_start = (start_length - length_so_far);
                    if (is_end_seg) {
                        segment_end = (end_length - length_so_far);
                    }
                    add_split_segment();
                } else if (found_start && !is_end_seg) { // already started but not end
                    result.addPath(current_path, SkPath::kExtend_AddPathMode);
                } else if (is_end_seg) { // end found
                    segment_end = (end_length - length_so_far);
                    add_split_segment();
                }
                length_so_far = length_after_seg;
                return is_end_seg;
            };

            bool trimmed = false;
            length_so_far = 0;
            SkPath::Iter iter(path, false);
            SkPath::Verb verb;
            while (!trimmed && path.countVerbs()) {
                if (SkPath::kDone_Verb == (verb = iter.next(points))) {
                    iter = SkPath::Iter(path, false);
                    verb = iter.next(points);
                    if (rotated) {
                        length_so_far = 0;
                    }
                }
                switch (verb) {
                case SkPath::kMove_Verb:
                    //                    result.moveTo(points[0]);
                    break;
                case SkPath::kLine_Verb:
                    trimmed = evaluate_line_segment();
                    break;
                case SkPath::kConic_Verb:
                case SkPath::kCubic_Verb:
                    trimmed = evaluate_conic_segment(verb, iter.conicWeight());
                    break;
                default:
                    break;
                }
            }
            //            }
            path = result;
        }
    } else {
        path.reset();
    }
}

} // namespace alive::model
