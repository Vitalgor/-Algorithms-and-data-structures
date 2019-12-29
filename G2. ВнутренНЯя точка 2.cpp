#pragma once

#include <exception>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/detail/standard_policies.hpp>
#include <iostream>
#include <vector>

namespace NGeometry {
    const long long INF = 1e10;

    enum TPointType {START, FINISH, QUERY};
    enum TPointLocation {INSIDE, BORDER, OUTSIDE};

    struct TPoint{
        long long x, y;
    };

    bool point_compare_by_x_and_y(const TPoint& a, const TPoint& b){
        if (a.x != b.x){
            return a.x < b.x;
        }
        return a.y < b.y;
    }

    bool are_points_equal(const TPoint& a, const TPoint& b){
        return ((a.x == b.x) && (a.y == b.y));
    }

    long long pseudodot(TPoint& a, TPoint& b){
        return a.x*b.y - b.x*a.y;
    }

    bool is_collinear(TPoint& x, TPoint& y){
        return (pseudodot(x, y) == 0);
    }

    struct TSegment{
        TPoint start, finish;
    };

    bool is_vertical(TSegment& segment){
        return segment.start.x == segment.finish.x;
    }

    bool point_on_segment(TPoint a, TSegment s) {
        if (are_points_equal(s.start, s.finish)) {
            return are_points_equal(s.start, a);
        }

        TPoint line_vector;
        line_vector.x = s.finish.x - s.start.x;
        line_vector.y = s.finish.y - s.start.y;
        TPoint point_vector;
        point_vector.x = a.x - s.start.x;
        point_vector.y = a.y - s.start.y;
        if (!is_collinear(point_vector, line_vector)){
            return false;
        }
        long max_x = std::max(s.start.x, s.finish.x);
        long min_x = std::min(s.start.x, s.finish.x);
        long max_y = std::max(s.start.y, s.finish.y);
        long min_y = std::min(s.start.y, s.finish.y);
        return (((a.x <= max_x) && (a.x >= min_x)) && ((a.y <= max_y) && (a.y >= min_y)));
    }

    class ScanlineSolver {
    public:
        ScanlineSolver(std::vector<TPoint>& points, std::vector<TSegment>& segments){
            TPoint point;
            for (long i = 0; i < points.size(); i++){
                TEvent event;
                event.point = points[i];
                event.type = QUERY;
                event.segment_num = -1;
                event.point_num = i;
                events_.push_back(event);
                events_vertical_.push_back(event);
            }
            for (auto segment: segments){
                if (are_points_equal(segment.start, segment.finish)){
                    continue;
                }

                if (point_compare_by_x_and_y(segment.finish, segment.start)){
                    std::swap(segment.start, segment.finish);
                }

                if (is_vertical(segment)){
                    vertical_segments_.push_back(segment);
                    TEvent event;

                    event.point = segment.start;
                    event.segment_num = vertical_segments_.size() - 1;
                    event.type = START;
                    events_vertical_.push_back(event);

                    event.point = segment.finish;
                    event.segment_num = vertical_segments_.size() - 1;
                    event.type = FINISH;
                    events_vertical_.push_back(event);
                    continue;
                }

                TEvent event;

                non_vertical_segments_.push_back(segment);
                event.point = segment.start;
                event.segment_num = non_vertical_segments_.size() - 1;
                event.type = START;
                events_.push_back(event);

                event.point = segment.finish;
                event.segment_num = non_vertical_segments_.size() - 1;
                event.type = FINISH;
                events_.push_back(event);
            }
            sort(events_.begin(), events_.end(), TEventComparator());
            sort(events_vertical_.begin(), events_vertical_.end(), TEventComparatorVertical());
            answers_.resize(points.size());
        }

        std::vector<TPointLocation> run(){
            std::set<std::pair<long, long>> end_points;
            for (auto event : events_) {
                TPoint point = event.point;
                if ((!end_points.empty()) && (point.x != end_points.begin()->first)) end_points.clear();
                if (event.type == START) {
                    segment_set_.insert(non_vertical_segments_[event.segment_num]);
                }
                else if (event.type == FINISH) {
                    end_points.insert(std::make_pair(point.x, point.y));
                    segment_set_.erase(non_vertical_segments_[event.segment_num]);
                }
                else {
                    auto it = end_points.find(std::make_pair(point.x, point.y));
                    if (it != end_points.end()){
                        answers_[event.point_num] = BORDER;
                        continue;
                    }
                    process_event_(event);
                }
            }
            TSegment segment;
            for (auto event : events_vertical_){
                TPoint point = event.point;
                if (event.type == START) {
                    vertical_segments_set_.insert(vertical_segments_[event.segment_num]);
                }
                else if (event.type == FINISH) {
                    vertical_segments_set_.erase(vertical_segments_[event.segment_num]);
                }
                else if (event.type == QUERY){
                    segment.start = point;
                    segment.finish = point;
                    segment.finish.y = INF;
                    auto lower_bound_segment_ptr = vertical_segments_set_.lower_bound(segment);
                    if (lower_bound_segment_ptr != vertical_segments_set_.begin() && !vertical_segments_set_.empty()){
                        lower_bound_segment_ptr = std::prev(lower_bound_segment_ptr);
                        if (point_on_segment(point, (*lower_bound_segment_ptr))){
                            answers_[event.point_num] = BORDER;
                        }
                    }
                }

            }

            return answers_;
        }

    private:
        struct TEvent{
            TPoint point;
            TPointType type;
            long segment_num;
            long point_num;
        };

        struct TEventComparator{
            bool operator() (const TEvent& e1, const TEvent& e2){
                TPoint a = e1.point;
                TPoint b = e2.point;
                if (a.x != b.x){
                    return a.x < b.x;
                }
                if (e1.type != e2.type){
                    return e1.type < e2.type;
                }
                return a.y < b.y;
            }
        };

        struct TEventComparatorVertical{
            bool operator() (const TEvent& e1, const TEvent& e2){
                TPoint a = e1.point;
                TPoint b = e2.point;
                if (a.x != b.x){
                    return a.x < b.x;
                }
                if (e1.type != e2.type){
                    if (e1.type == START){
                        return true;
                    }
                    if (e2.type == START){
                        return false;
                    }
                    if (e1.type == QUERY){
                        return true;
                    }
                    return false;
                }
                return a.y < b.y;
            }
        };

        struct TSegmentComparator{
            bool operator()(const TSegment& a, const TSegment& b) const{
                TPoint first_line_vector;
                first_line_vector.x = a.finish.x - a.start.x;
                first_line_vector.y = a.finish.y - a.start.y;
                TPoint second_line_vector;
                second_line_vector.x = b.finish.x - b.start.x;
                second_line_vector.y = b.finish.y - b.start.y;
                if (a.start.x < b.start.x){
                    TPoint cross_vector;
                    cross_vector.x = b.start.x - a.start.x;
                    cross_vector.y = b.start.y - a.start.y;
                    if (pseudodot(first_line_vector, cross_vector) != 0){
                        return pseudodot(first_line_vector, cross_vector) > 0;
                    }
                }
                if (a.start.x > b.start.x){
                    TPoint cross_vector;
                    cross_vector.x = a.start.x - b.start.x;
                    cross_vector.y = a.start.y - b.start.y;
                    if (pseudodot(second_line_vector, cross_vector) != 0){
                        return (pseudodot(second_line_vector, cross_vector) < 0);
                    }
                }
                if (are_points_equal(a.start, b.start)){
                    if (are_points_equal(a.start, a.finish)){
                        return false;
                    }
                    if (are_points_equal(b.start, b.finish)){
                        return true;
                    }

                    return (pseudodot(first_line_vector, second_line_vector) > 0);
                }

                if (a.start.x == b.start.x){
                    return a.start.y < b.start.y;
                }

                return a.start.x < b.start.x;
            }
        };

        struct TVerticalSegmentComparator{
            bool operator()(const TSegment& a, const TSegment& b) const {
                if (a.start.y != b.start.y){
                    return a.start.y < b.start.y;
                }
                return a.finish.y < b.finish.y;
            }
        };

        typedef __gnu_pbds::tree<
            TSegment,
            __gnu_pbds::null_type,
            TSegmentComparator,
            __gnu_pbds::rb_tree_tag,
            __gnu_pbds::tree_order_statistics_node_update>
        indexed_segment_set;

        std::vector<TEvent> events_, events_vertical_;
        std::vector<TSegment> non_vertical_segments_, vertical_segments_;
        std::set<TSegment, TVerticalSegmentComparator> vertical_segments_set_;
        indexed_segment_set segment_set_;
        std::vector<TPointLocation> answers_;

        void process_event_(TEvent event){
            TPoint point = event.point;
            TSegment segment;
            segment.start = point;
            segment.finish = point;
            auto lower_bound_segment_ptr = segment_set_.lower_bound(segment);
            if (lower_bound_segment_ptr != segment_set_.begin() && !segment_set_.empty()){
                lower_bound_segment_ptr = std::prev(lower_bound_segment_ptr);
                if (point_on_segment(point, (*lower_bound_segment_ptr))){
                    answers_[event.point_num] = BORDER;
                    return;
                }
            }
            long intersect_count = segment_set_.order_of_key(segment);
            if (intersect_count % 2){
                answers_[event.point_num] = INSIDE;
            }
            else{
                answers_[event.point_num] = OUTSIDE;
            }
        }
    };
}

class ProblemSolver{
public:
    void ReadInput(std::istream& stream){
        stream >> vertex_cnt_;
        Vertexes_.resize(vertex_cnt_);
        for (long i = 0; i < vertex_cnt_; i++){
            stream >> Vertexes_[i].x >> Vertexes_[i].y;
        }
        stream >> points_cnt_;
        Points_.resize(points_cnt_);
        for (long i = 0; i < points_cnt_; i++){
            stream >> Points_[i].x >> Points_[i].y;
        }
    }

    void Solve(){
        Sides_.resize(vertex_cnt_);
        for (long i = 0; i < vertex_cnt_ - 1; i++){
            Sides_[i].start = Vertexes_[i];
            Sides_[i].finish = Vertexes_[i+1];
        }
        Sides_[vertex_cnt_-1].start = Vertexes_[vertex_cnt_-1];
        Sides_[vertex_cnt_-1].finish = Vertexes_[0];
        NGeometry::ScanlineSolver solver = NGeometry::ScanlineSolver(Points_, Sides_);
        Answers_ = solver.run();
    }

    void PrintResult(std::ostream& stream){
        for (long i = 0; i < points_cnt_; i++){
            NGeometry::TPointLocation answer = Answers_[i];
            if (answer == NGeometry::BORDER){
                stream << "BORDER" << std::endl;
            }
            else if (answer == NGeometry::INSIDE){
                stream << "INSIDE" << std::endl;
            }
            else if (answer == NGeometry::OUTSIDE){
                stream << "OUTSIDE" << std::endl;
            }
        }
        stream << std::endl;
    }

private:
    long vertex_cnt_;
    long points_cnt_;
    std::vector<NGeometry::TPoint> Points_;
    std::vector<NGeometry::TPoint> Vertexes_;
    std::vector<NGeometry::TSegment> Sides_;
    std::vector<NGeometry::TPointLocation> Answers_;
};

int main(){
    long test_cnt;
    std::cin >> test_cnt;
    for (long j = 0; j < test_cnt; j++){
        ProblemSolver solver = ProblemSolver();
        solver.ReadInput(std::cin);
        solver.Solve();
        solver.PrintResult(std::cout);
    }
}
