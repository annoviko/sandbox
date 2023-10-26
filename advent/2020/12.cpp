#include <fstream>
#include <iostream>
#include <vector>
#include <string>


struct instruction {
    char        m_function = '0';
    long long   m_argument = 0;
};


using instruction_sequence = std::vector<instruction>;


instruction_sequence read_instructions(const std::string & p_file) {
    instruction_sequence result;
    std::ifstream stream(p_file);

    for (std::string line; std::getline(stream, line); ) {
        instruction action;
        action.m_function = line[0];
        action.m_argument = std::stoll(line.substr(1));

        result.push_back(action);
    }

    return result;
}


struct position {
public:
    long long x = 0;
    long long y = 0;

public:
    position(long long p_x, long long p_y) : 
        x(p_x), y(p_y)
    { }
};


class navigator {
protected:
    instruction_sequence m_actions;
    position             m_position;
    char                 m_direction = 'E';

public:
    navigator(const instruction_sequence & p_actions) : 
        m_actions(p_actions), m_position(0, 0) 
    { }

public:
    virtual long long process() {
        for (const auto & action : m_actions) {
            handle_action(action);
        }

        return std::abs(m_position.x) + std::abs(m_position.y);
    }

protected:
    void handle_action(const instruction & p_instruction) {
        switch (p_instruction.m_function) {
        case 'F':
            forward(p_instruction.m_argument);
            break;

        case 'N':
        case 'S':
        case 'W':
        case 'E':
            move(p_instruction.m_function, p_instruction.m_argument);
            break;

        case 'L':
            turn_left(p_instruction.m_argument);
            break;

        case 'R':
            turn_right(p_instruction.m_argument);
            break;

        default:
            throw std::invalid_argument("Unknown command");
        }
    }

    virtual void turn_right(const long long p_argument) {
        const static std::vector<char> sequence = { 'N', 'E', 'S', 'W' };
        turn(p_argument, sequence);
    }

    virtual void turn_left(const long long p_argument) {
        const static std::vector<char> sequence = { 'N', 'W', 'S', 'E' };
        turn(p_argument, sequence);
    }

    virtual void forward(const long long p_argument) {
        move(m_direction, p_argument);
    }

    virtual void move(const long long p_direction, const long long p_argument) {
        move_object(p_direction, p_argument, m_position);
    }

    void turn(const long long p_argument, const std::vector<char> & p_sequence) {
        const long long p_sectors = p_argument / 90;

        const long long position = std::find(p_sequence.begin(), p_sequence.end(), m_direction) - p_sequence.begin();

        const auto next_position = (position + p_sectors) % p_sequence.size();
        m_direction = p_sequence[next_position];
    }

    void move_object(const long long p_direction, const long long p_argument, position & m_object) {
        switch (p_direction) {
        case 'N':
            m_object.y += p_argument;
            break;

        case 'S':
            m_object.y -= p_argument;
            break;

        case 'E':
            m_object.x += p_argument;
            break;

        case 'W':
            m_object.x -= p_argument;
            break;

        default:
            throw std::exception("Unknown direction.");
        }
    }
};


class waypoint_navigator : public navigator {
private:
    position m_waypoint;

public:
    waypoint_navigator(const instruction_sequence & p_actions) :
        navigator(p_actions),
        m_waypoint(10, 1)
    { }

protected:
    virtual void forward(const long long p_argument) override {
        const long long dx = p_argument * m_waypoint.x;
        const long long dy = p_argument * m_waypoint.y;

        m_position.x += dx;
        m_position.y += dy;
    }

    virtual void turn_left(const long long p_argument) override {
        long long sectors = p_argument / 90;
        for (; sectors > 0; sectors--) {
            long long next_y = m_waypoint.x;
            long long next_x = -m_waypoint.y;

            m_waypoint.x = next_x;
            m_waypoint.y = next_y;
        }
    }

    virtual void turn_right(const long long p_argument) override {
        long long sectors = p_argument / 90;
        for (; sectors > 0; sectors--) {
            long long next_y = -m_waypoint.x;
            long long next_x = m_waypoint.y;
            
            m_waypoint.x = next_x;
            m_waypoint.y = next_y;
        }
    }

    virtual void move(const long long p_direction, const long long p_argument) {
        move_object(p_direction, p_argument, m_waypoint);
    }
};



int main() {
    const auto instructions = read_instructions("input.txt");

    std::cout << "The Manhattan distance between that location and the ship's starting position (without waypoint): " << navigator(instructions).process() << std::endl;
    std::cout << "The Manhattan distance between that location and the ship's starting position (with waypoint): " << waypoint_navigator(instructions).process() << std::endl;

    return 0;
}
