#include <vector>


class disjoint_sets {
private:
    std::vector<int> m_ptr;
    std::vector<int> m_rank;
    
public:
    disjoint_sets(const int n) :
        m_ptr(n, 0),
        m_rank(n, 1)
    {
        for (int i = 0; i < n; i++) {
            m_ptr[i] = i;
        }
    }
    
public:
    int find_set(const int p_index) {
        if (m_ptr[p_index] != p_index) {
            m_ptr[p_index] = find_set(m_ptr[p_index]);
        }
        
        return m_ptr[p_index];
    }
    
    bool is_same_set(const int l, const int r) {
        int lhead = find_set(l);
        int rhead = find_set(r);
        
        return lhead == rhead;
    }
    
    void merge(const int l, const int r) {
        int lhead = find_set(l);
        int rhead = find_set(r);
        
        if (lhead == rhead) { return; }
        
        if (m_rank[lhead] > m_rank[rhead]) {
            m_ptr[rhead] = lhead;
            m_rank[lhead] += m_rank[rhead];
        }
        else {
            m_ptr[lhead] = rhead;
            m_rank[rhead] += m_rank[lhead];
        }
    }
};


struct edge {
    int from = 0;
    int to = 0;
    int cost = 0;
};


class Solution {
private:
    int manhattan(const std::vector<int> & l, const std::vector<int> & r) {
        return std::abs(r[0] - l[0]) + std::abs(r[1] - l[1]);
    }
    
public:
    int minCostConnectPoints(std::vector<std::vector<int>>& points) {
        std::vector<edge> edges;
        
        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                int cost = manhattan(points[i], points[j]);
                edges.push_back({ i, j, cost });
            }
        }
        
        std::sort(edges.begin(), edges.end(), [](const edge& l, const edge& r) { 
            return l.cost < r.cost;
        });
        
        int total_cost = 0;
        disjoint_sets sets(points.size());
        for (const edge & e : edges) {
            if (sets.is_same_set(e.from, e.to)) {
                continue;
            }
            
            sets.merge(e.from, e.to);
            total_cost += e.cost;
        }
        
        return total_cost;
    }
};
