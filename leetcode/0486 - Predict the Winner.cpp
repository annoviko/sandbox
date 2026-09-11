#include <vector>
#include <unordered_map>


#if 1
    #define CACHE
#endif

#if 1
    #define WORKABLE_SOLUTION_1     /* optimal, has two states only */
#else
    #define WORKABLE_SOLUTION_2     /* acceptable, but slower since there are three states (my initial solution) */
#endif

#if defined(WORKABLE_SOLUTION_1)
class Solution {
    std::unordered_map<int, std::unordered_map<int, int>> cache; /* [left index][right index] -> [advantage] */

public:
    bool predictTheWinner(const std::vector<int>& nums) {
        cache.clear();
        return dp(0, nums.size() - 1, nums) >= 0;
    }

private:
    int dp(int left_index, int right_index, const std::vector<int>& nums) {
        if (left_index == right_index) {
            return nums[left_index];
        }

        int cached_advantage = get_cache_value(left_index, right_index);
        if (cached_advantage != -1) {
            return cached_advantage;
        }

        int going_left = nums[left_index] - dp(left_index + 1, right_index, nums);
        int going_right = nums[right_index] - dp(left_index, right_index - 1, nums);

        int advantage = std::max(going_left, going_right);   /* always maximuze advantage for each player */
        cache[left_index][right_index] = advantage;

        return advantage;
    }

    int get_cache_value(int left_index, int right_index) {
        auto left_index_iter = cache.find(left_index);
        if (left_index_iter == cache.cend()) {
            return -1;
        }

        auto right_index_iter = left_index_iter->second.find(right_index);
        if (right_index_iter == left_index_iter->second.cend()) {
            return -1;
        }

        return right_index_iter->second;
    }
};
#elif defined(WORKABLE_SOLUTION_2)
class Solution {
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> cache;   /* [left index][right index][first score][first turn] -> score first */
    int total_score = 0;

public:
    bool predictTheWinner(const std::vector<int>& nums) {
        cache.clear();
        total_score = 0;
        for (int v : nums) {
            total_score += v;
        }

        int score_first = dp(0, nums.size() - 1, 0, true, nums);
        int score_second = total_score - score_first;
        return score_first >= score_second;
    }

private:
    int dp(int left_index, int right_index, int first_score, bool first_turn, const std::vector<int>& nums) {
        if (left_index > right_index) {
            return first_score;
        }

#if defined(CACHE)
        int cached_score = get_score_from_cache(left_index, right_index, first_score);
        if (cached_score != -1) {
            return cached_score;
        }
#endif

        int new_first_score = 0;

        if (first_turn) {
            int score_going_left = dp(left_index + 1, right_index, first_score + nums[left_index], !first_turn, nums);
            int score_going_right = dp(left_index, right_index - 1, first_score + nums[right_index], !first_turn, nums);

            new_first_score = std::max(score_going_left, score_going_right);   /* we trying to maximum first */
        }
        else {
            int score_going_left = dp(left_index + 1, right_index, first_score, !first_turn, nums);
            int score_going_right = dp(left_index, right_index - 1, first_score, !first_turn, nums);

            new_first_score = std::min(score_going_left, score_going_right);   /* we trying to minimize first */
        }

#if defined(CACHE)
        cache[left_index][right_index][first_score] = new_first_score;
#endif
        return new_first_score;
    }

    int get_score_from_cache(int left_index, int right_index, int first_score) {
        auto left_index_iter = cache.find(left_index);
        if (left_index_iter == cache.cend()) {
            return -1;
        }

        auto right_index_iter = left_index_iter->second.find(right_index);
        if (right_index_iter == left_index_iter->second.cend()) {
            return -1;
        }

        auto first_score_iter = right_index_iter->second.find(first_score);
        if (first_score_iter == right_index_iter->second.cend()) {
            return -1;
        }

        return first_score_iter->second;
    }
};
#endif