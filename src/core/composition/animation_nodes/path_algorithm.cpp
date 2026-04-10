#include "path_algorithm.h"

#include <include/core/SkPath.h>
#include <include/core/SkPathMeasure.h>
#include <include/core/SkPoint.h>
#include <profiler.h>
#include <third_party/minitrace/minitrace.h>

namespace inae::model {

void cubic_split(std::vector<SkPoint> &points, Vec1D t, bool from_start = true)
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
void cubic_split(std::vector<SkPoint> &points, Vec1D t1, Vec1D t2)
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
            if (offset != 0.0f) {
                start_length += offset_length;
                end_length += offset_length;
                start_length -= static_cast<int>(start_length / total_length) * total_length;
                end_length -= static_cast<int>(end_length / total_length) * total_length;

                if (start_length > end_length) {
                    rotated = true;
                }
            }

            int element_count = path.countVerbs();
            std::vector<SkPoint> points(4);
            int index = 0;

            Vec1D length_so_far = 0;
            bool found_start = false;
            bool end_found = false;
            auto check_start_end = [&]() -> bool {
                bool is_start_seg = false;
                bool is_end_seg = false;
                if (index != 0) {
                    auto last_point = points[index];
                    // new qpainterpath

                    if (index == 3) {
                        SkPath current_path;
                        current_path.moveTo(points[0]);
                        current_path.cubicTo(points[1], points[2], points[3]);
                        SkPathMeasure current_path_measure;
                        current_path_measure.setPath(&current_path, false);
                        Vec1D seg_length = current_path_measure.getLength();
                        Vec1D length_after_seg = length_so_far + seg_length;
                        if (!found_start && length_so_far <= start_length
                            && start_length <= length_after_seg) {
                            is_start_seg = true;
                        }

                        bool find_end = (!rotated || found_start);
                        if (find_end && length_so_far <= end_length
                            && end_length <= length_after_seg) {
                            is_end_seg = true;
                        }

                        if (is_start_seg) {
                            // both start and end in this segment
                            // start in this segment but not end
                            Vec1D start_percent = (start_length - length_so_far) / seg_length;
                            found_start = true;
                            if (is_end_seg) {
                                Vec1D end_percent = (end_length - length_so_far) / seg_length;
                                cubic_split(points, start_percent, end_percent);
                            } else {
                                cubic_split(points, start_percent, true);
                            }
                            result.moveTo(points[0]);
                        } else if (found_start && !is_end_seg) { // already started but not end
                        } else if (is_end_seg) {                 // end found
                            Vec1D end_percent = (end_length - length_so_far) / seg_length;
                            cubic_split(points, end_percent, false);
                        }
                        if (found_start)
                            result.cubicTo(points[1], points[2], points[3]);
                        length_so_far = length_after_seg;
                    } else if (1 == index) {
                        SkPath current_path;
                        current_path.moveTo(points[0]);
                        current_path.lineTo(points[1]);
                        SkPathMeasure current_path_measure;
                        current_path_measure.setPath(&current_path, false);
                        Vec1D seg_length = current_path_measure.getLength();
                        Vec1D length_after_seg = length_so_far + seg_length;
                        if (!found_start && length_so_far <= start_length
                            && start_length <= length_after_seg) {
                            is_start_seg = true;
                        }

                        bool find_end = (!rotated || found_start);
                        if (find_end && length_so_far <= end_length
                            && end_length <= length_after_seg) {
                            is_end_seg = true;
                        }

                        if (is_start_seg) {
                            // both start and end in this segment
                            // start in this segment but not end
                            Vec1D start_percent = (start_length - length_so_far) / seg_length;
                            points[0] = points[0] + (points[1] - points[0]) * start_percent;
                            found_start = true;
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
                    }

                    points[0] = last_point;
                    index = 0;
                }
                return is_end_seg;
            };

            auto evaluate = [&](SkPoint p) -> bool {
                points[++index] = p;
                bool result = check_start_end();
                return result;
            };

            uint8_t verbs[256] = {};
            int verbs_count = path.getVerbs(verbs, 256);
            assert(element_count <= 256);

            SkPoint path_points[256];
            bool trimmed = false;
            int points_count = path.getPoints(path_points, 256);
            assert(path.countPoints() <= 256);

            int point_index = 0;

            auto get_next_point = [&]() {
                SkPoint point = path_points[point_index % points_count];
                ++point_index;
                return point;
            };

            while (!trimmed) {
                length_so_far = 0;
                for (int i = 0; i < element_count; ++i) {
                    if (trimmed)
                        break;

                    uint8_t type = verbs[i];
                    switch (type) {
                    case SkPath::Verb::kMove_Verb:
                        index = 0;
                        points[index] = get_next_point();
                        break;
                    case SkPath::Verb::kLine_Verb:
                        trimmed = evaluate(get_next_point());
                        break;
                    case SkPath::Verb::kConic_Verb:
                        points[++index] = get_next_point();
                        trimmed = evaluate(get_next_point());
                        break;
                    case SkPath::Verb::kClose_Verb:
                        trimmed = evaluate(get_next_point());
                        break;
                    default:
                        break;
                    }
                }
                if (!trimmed)
                    trimmed = check_start_end();
            }
            path = result;
        }
    } else {
        path.reset();
    }
}

} // namespace inae::model
