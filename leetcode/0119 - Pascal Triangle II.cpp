#include <vector>


class Solution {
public:
    std::vector<int> getRow(int row_index) {
        std::vector<int> row{ 1 };

        for (int cur_index = 0; cur_index < row_index; cur_index++) {
            std::vector<int> next_row(row.size() + 1, 1);
            for (int i = 1; i < next_row.size() - 1; i++) {
                next_row[i] = row[i - 1] + row[i];
            }

            row = std::move(next_row);
        }

        return row;
    }
};