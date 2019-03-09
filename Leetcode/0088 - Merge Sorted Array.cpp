class Solution {
public:
#if defined(LONG_SOLUTION)
    void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
        int index_zero = nums1.size() - 1;
        int index_real = m - 1;
        
        while(index_real >= 0) {
            std::swap(nums1[index_zero], nums1[index_real]);

            index_zero--;
            index_real--;
        }

        int inum1 = nums1.size() - m;
        int inum2 = 0;
        int free_index = 0;
        for (; (inum1 < nums1.size()) && (inum2 < nums2.size()); free_index++) {
            if (nums1[inum1] > nums2[inum2]) {
                nums1[free_index] = nums2[inum2];
                inum2++;
            }
            else {
                nums1[free_index] = nums1[inum1];
                inum1++;
            }
        }

        while(inum2 < nums2.size()) {
            nums1[free_index] = nums2[inum2];
            free_index++;
            inum2++;
        }
    }
#else
    void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
        int inum1 = m - 1;
        int inum2 = n - 1;
        int free_index = nums1.size() - 1;

        while (inum2 >= 0) {
            if (inum1 >= 0 && nums1[inum1] > nums2[inum2]) {
                nums1[free_index] = nums1[inum1];
                inum1--;
            }
            else {
                nums1[free_index] = nums2[inum2];
                inum2--;
            }

            free_index--;
        }
    }
#endif
};