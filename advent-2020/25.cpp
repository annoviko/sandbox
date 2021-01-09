#include <iostream>


class key_finder {
private:
    constexpr static std::size_t INITIAL_SUBJECT_NUMBER = 7;

private:
    std::size_t m_card_public_key;
    std::size_t m_door_public_key;

public:
    key_finder(const std::size_t p_card_public_key, const std::size_t p_door_public_key) : 
        m_card_public_key(p_card_public_key),
        m_door_public_key(p_door_public_key)
    { }

public:
    std::size_t find_encryption_key() {
        const std::size_t card_loop_size = find_loop_size(m_card_public_key);
        const std::size_t door_loop_size = find_loop_size(m_door_public_key);

        const std::size_t card_encryption_key = calculate_key(m_door_public_key, card_loop_size);
        const std::size_t door_encryption_key = calculate_key(m_card_public_key, door_loop_size);

        if (card_encryption_key != door_encryption_key) {
            throw std::exception("Impossible to calculate encryption key.");
        }

        return card_encryption_key;
    }

private:
    std::size_t find_loop_size(const std::size_t p_public_key) {
        std::size_t value = 1;
        std::size_t loop_size = 0;

        while (value != p_public_key) {
            value = calculate_value(value, INITIAL_SUBJECT_NUMBER);
            loop_size++;
        }

        return loop_size;
    }

    std::size_t calculate_key(const std::size_t p_subject_number, const std::size_t p_loop_size) {
        std::size_t key = 1;
        for (std::size_t i = 0; i < p_loop_size; i++) {
            key = calculate_value(key, p_subject_number);
        }

        return key;
    }

    std::size_t calculate_value(const std::size_t p_value, const std::size_t p_subject_number) {
        std::size_t result = p_value * p_subject_number;
        return result % 20201227;
    }
};


int main() {
    const std::size_t card_public_key = 335121;
    const std::size_t door_public_key = 363891;

    const std::size_t encryption_key = key_finder(card_public_key, door_public_key).find_encryption_key();

    std::cout << "The encryption key: " << encryption_key << std::endl;

    return 0;
}
