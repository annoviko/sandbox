#include <vector>


class LUPrefix {
private:
    std::vector<bool> m_set;
    int m_longest = 0;

public:
    LUPrefix(int n) : m_set(n + 1) { }

    void upload(int video) {
        m_set[video] = true;

        int video_id = m_longest + 1;
        for (int video_id = m_longest + 1; m_set[video_id]; video_id++) {
            m_longest++;
        }
    }

    int longest() {
        return m_longest;
    }
};