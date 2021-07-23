#include <queue>
#include <set>


class MedianFinder {
private:
#if defined(BALANCED_TREE)
    std::multiset<int> m_left;   /* store as negative value, the smallest is a biggest */
    std::multiset<int> m_right;  /* store in common way */
#else
    std::priority_queue<int> m_left;        /* store as negative value, the smallest is a biggest */
    std::priority_queue<int> m_right;       /* store in common way */
#endif

public:
    /** initialize your data structure here. */
    MedianFinder() {

    }

    void addNum(int num) {
        if (m_left.size() > m_right.size()) {
            /* we have to add the element to right */
            add_left(num);   /* add to left to get maximum value that should be added to right */ /* O(log(N)) */

            int max_value = pop_left(); /* get maximum from the left and remove maxumum from the left */ /* O(log(N)) */
            add_right(max_value);       /* and add maximum to right */  /* O(log(N)) */
        }
        else {
            /* we have to add the element to left */
            add_right(num);    /* add to right to get minimum value that should be added to left */ /* O(log(N)) */

            int min_value = pop_right(); /* get minimum and remove minimum from the right */    /* O(log(N)) */
            add_left(min_value);           /* and add minimum to left */    /* O(log(N)) */
        }

        /* Total complexity: O(log(N)) */
    }

    double findMedian() {
        const double left = top_left(); /* O(1) */
        if (m_left.size() > m_right.size()) {
            return left;      /* get the biggest value and do not forget to set proper sign */
        }

        const double right = top_right();   /* O(1) */
        return (left + right) / 2.0;

        /* Total complexity: O(1) */
    }

private:
    void add_right(int p_value) {
#if defined(BALANCED_TREE)
        m_right.insert(p_value);
#else
        m_right.push(p_value);
#endif
    }

    void add_left(int p_value) {
#if defined(BALANCED_TREE)
        m_left.insert(-p_value);
#else
        m_left.push(-p_value);
#endif
    }

    int pop_right() {
#if defined(BALANCED_TREE)
        int value = *(m_right.begin());
        m_right.erase(m_right.begin());
        return value;
#else
        int value = m_right.top();
        m_right.pop();
        return value;
#endif
    }

    int pop_left() {
#if defined(BALANCED_TREE)
        int value = -*(m_left.begin());
        m_left.erase(m_left.begin());
        return value;
#else
        int value = -m_left.top();
        m_left.pop();
        return value;
#endif
    }

    int top_right() {
#if defined(BALANCED_TREE)
        return *(m_right.begin());
#else
        return m_right.top();
#endif
    }

    int top_left() {
#if defined(BALANCED_TREE)
        return -*(m_left.begin());
#else
        return -m_left.top();
#endif
    }
};
