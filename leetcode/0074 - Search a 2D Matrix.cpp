class Solution {
public:
    bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
        const int row_size = matrix[0].size();

        int start = 0;
        int end = matrix.size() * row_size - 1;

        while (start <= end) {
              const int mid = (start + end) >> 1;
              
              const int row = mid / row_size;
              const int col = mid % row_size;
                     
              if (matrix[row][col] < target) {
          	    start = mid + 1;
              }
              else if (matrix[row][col] > target) {
          	    end = mid - 1;
              }
              else {
          	    return true;
              }
        }
        
        return false;
    }
};