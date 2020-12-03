class RandomizedSet {
private:
    std::unordered_map<int, std::size_t> m_values = { };
    std::vector<int> m_sequence = { };

public:
    /** Initialize your data structure here. */
    RandomizedSet() = default;

    /** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
    bool insert(int val) {
        auto iter = m_values.find(val);

        if (iter == m_values.end()) {
            m_values.insert({ val, m_sequence.size() });
            m_sequence.push_back(val);

            return true;
        }

        return false;
    }

    /** Removes a value from the set. Returns true if the set contained the specified element. */
    bool remove(int val) {
        auto iter = m_values.find(val);

        if (iter != m_values.end()) {
            const std::size_t index = iter->second;
            if (index != m_sequence.size() - 1)
            {
                m_values[m_sequence.back()] = index;
                std::swap(m_sequence[index], m_sequence.back());
            }
            
            m_sequence.pop_back();
            m_values.erase(iter);

            return true;
        }

        return false;
    }

    /** Get a random element from the set. */
    int getRandom() {
        /* The correct solution - the provides real random mechanism */
        if (!m_values.empty()) {
            std::size_t index = std::rand() % m_values.size();
            return m_sequence[index];
        }
        
        return 0;
    }
};