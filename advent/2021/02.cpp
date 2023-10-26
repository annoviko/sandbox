#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


enum class direction {
    forward = 0,
    up,
    down
};


const std::map<std::string, direction> direction_string_map = {
    { "forward", direction::forward },
    { "up", direction::up },
    { "down", direction::down }
};


direction string_to_direction(const std::string& p_string) {
    const auto iter = direction_string_map.find(p_string);
    if (iter == direction_string_map.cend()) {
        throw std::exception("unknown direction name");
    }

    return iter->second;
}


class instruction {
private:
    direction m_direction;
    std::size_t m_distance;

public:
    instruction(const direction p_direction, const std::size_t p_distance) :
        m_direction(p_direction),
        m_distance(p_distance)
    { }

    direction get_direction() const { return m_direction; }

    std::size_t get_distance() const { return m_distance; }

public:
    static instruction parse(const std::string& p_string) {
        std::stringstream stream;
        stream << p_string;

        std::string string_direction;
        std::size_t distance = 0;

        stream >> string_direction >> distance;
        return instruction{ string_to_direction(string_direction), distance };
    }
};


using path = std::vector<instruction>;


path read_input() {
    std::ifstream stream("input.txt");

    path result;
    for (std::string line; std::getline(stream, line); ) {
        result.push_back(instruction::parse(line));
    }

    return result;
}


class submarine_navigator {
protected:
    path m_path;
    std::size_t m_x = 0;
    std::size_t m_y = 0;

public:
    submarine_navigator(const path& p_path) : m_path(p_path) { }

public:
    std::size_t get_position() {
        return m_x * m_y;
    }

    submarine_navigator & traverse() {
        for (const auto & command : m_path) {
            handle_command(command);
        }

        return *this;
    }

protected:
    virtual void handle_command(const instruction& p_instruction) {
        switch(p_instruction.get_direction()) {
        case direction::up:
            m_y -= p_instruction.get_distance();
            break;

        case direction::down:
            m_y += p_instruction.get_distance();
            break;

        case direction::forward:
            m_x += p_instruction.get_distance();
            break;

        default:
            throw std::exception("unknown direction");
        }
    }
};


class submarine_navigator_real : public submarine_navigator {
private:
    std::size_t m_aim = 0;

public:
    submarine_navigator_real(const path& p_path) : submarine_navigator(p_path) { }

protected:
    virtual void handle_command(const instruction& p_instruction) override {
        switch(p_instruction.get_direction()) {
        case direction::up:
            m_aim -= p_instruction.get_distance();
            break;

        case direction::down:
            m_aim += p_instruction.get_distance();
            break;

        case direction::forward:
            m_x += p_instruction.get_distance();
            m_y += m_aim * p_instruction.get_distance();
            break;

        default:
            throw std::exception("unknown direction");
        }
    }
};


int main() {
    const path input = read_input();

    std::cout << "Suggested final destination (x * y): " << submarine_navigator(input).traverse().get_position() << std::endl;
    std::cout << "Real final destination (x * y): " << submarine_navigator_real(input).traverse().get_position() << std::endl;

    return 0;
}