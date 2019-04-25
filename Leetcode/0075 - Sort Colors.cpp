class Solution {
public:
    void sortColors(std::vector<int>& nums) {
#if defined(QUICKSORT_SOLUTION)
        quicksort(nums, 0, nums.size() - 1);
#else
        optimalSort(nums);
#endif
    }

    void optimalSort(std::vector<int> & nums) {
        int low = 0;
        int high = nums.size() - 1;
        int mid = 0;

        while (mid <= high) {
            if (nums[mid] == 0) {
                std::swap(nums[mid], nums[low]);
                low++;
                mid++;
            }
            else if (nums[mid] == 1) {
                mid++;
            }
            else if (nums[mid] == 2) {
                std::swap(nums[mid], nums[high]);
                high--;
            }
        }
    }

#if defined(QUICKSORT_SOLUTION)
    void quicksort(std::vector<int> & nums, int lborder, int rborder) {
        if (lborder >= rborder) { return; }
        
        int position = partition(nums, lborder, rborder);
        quicksort(nums, lborder, position - 1);
        quicksort(nums, position + 1, rborder);
    }
    
    int partition(std::vector<int> & nums, int lborder, int rborder) {
        int pivot = nums[rborder];
        int lpos = lborder;
        
        for (int i = lborder; i < rborder; i++) {
            if (nums[i] < pivot) {
                std::swap(nums[i], nums[lpos]);
                lpos++;
            }
        }
        
        std::swap(nums[rborder], nums[lpos]);
        
        return lpos;
    }
#endif
};
