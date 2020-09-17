class Solution {
public:
#if defined(BAD_SOLUTION)
    std::vector<Interval> merge(std::vector<Interval>& intervals) {
        if (intervals.size() < 2) { return intervals; }
        
        std::vector<Interval> result = { intervals[0] };
        std::vector<Interval> & to_consider = intervals;

        bool changed = true;
        bool global_changed = true;

        while(global_changed) {
            global_changed = false;
            result = { to_consider[0] };

            for (std::size_t i = 1; i < to_consider.size(); i++) {
                changed = false;
                for (std::size_t j = 0; j < result.size(); j++) {
                    auto & inter = result[j];
                    if ( (inter.start <= to_consider[i].start && inter.end >= to_consider[i].start) ||
                         (inter.start <= to_consider[i].end && inter.end >= to_consider[i].end) ||
                         (inter.start <= to_consider[i].start && inter.end >= to_consider[i].end) ||
                         (inter.start >= to_consider[i].start && inter.end <= to_consider[i].end)) 
                    {
                        inter.start = std::min(inter.start, to_consider[i].start);
                        inter.end = std::max(inter.end, to_consider[i].end);

                        global_changed = true;
                        changed = true;
                        break;
                    }
                }

                if (changed != true) {
                    result.push_back(to_consider[i]);
                }
            }

            to_consider = result;
        }

        return result;
    }
#else
    std::vector<Interval> merge(std::vector<Interval>& intervals) {
        if (intervals.size() < 2) { return intervals; }

        std::sort(intervals.begin(), intervals.end(), [](auto & left, auto & right) {
            return left.start < right.start;
        });

        std::vector<Interval> result = { intervals[0] };
        for (std::size_t i = 1; i < intervals.size(); i++) {
            if (result.back().end >= intervals[i].start) {
                result.back().end = std::max(result.back().end, intervals[i].end);
            }
            else {
                result.push_back(intervals[i]);
            }
        }

        return result;
    }
#endif
};