#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <vector>
#include <string>
#include <sstream>


template <typename T>
class integer_computer {
private:
    enum class command_t {
        nop = 0,
        add,
        multiply,
        read_input,
        write_output,
        jump_if_true,
        jump_if_false,
        less_than,
        equals,
        adjust_relative_base,
        halt = 99
    };

    enum class mode_t {
        positional = 0,
        parameter,
        relative
    };

    struct instruction {
        command_t m_command = command_t::nop;
        std::vector<mode_t> m_mode = { };
    };

    static const std::size_t INITIAL_MEMORY_VALUE = 1024;

public:
    enum class status {
        waiting_for_input,
        terminated,
    };

private:
    std::size_t     m_instruction_counter;
    T               m_relative_base;

    std::vector<T>  m_memory;
    std::vector<T>  m_software_image;

    std::size_t     m_size_memory = INITIAL_MEMORY_VALUE;

    std::list<T> m_input;
    std::list<T> m_output;

public:
    integer_computer() :
        m_software_image(load_software()),
        m_size_memory(std::max(INITIAL_MEMORY_VALUE, m_software_image.size()))
    {
        restart();
    }

public:
    void restart(void) {
        m_instruction_counter = 0;
        m_relative_base = 0;

        m_memory = std::vector<T>(m_size_memory, 0);
        m_input.clear();
        m_output.clear();

        std::copy(m_software_image.begin(), m_software_image.end(), m_memory.begin());
    }

    status execute() {
        for (; m_instruction_counter < m_memory.size();) {
            auto operation = get_instruction();

            switch (operation.m_command) {
            case command_t::add:
                add(operation);
                break;

            case command_t::multiply:
                multiply(operation);
                break;

            case command_t::read_input:
                if (m_input.empty()) {
                    m_instruction_counter -= 1;
                    return status::waiting_for_input;
                }
                read_input(operation);
                break;

            case command_t::write_output:
                write_output(operation);
                break;

            case command_t::jump_if_true:
                jump_if_true(operation);
                break;

            case command_t::jump_if_false:
                jump_if_false(operation);
                break;

            case command_t::less_than:
                less_than(operation);
                break;

            case command_t::equals:
                equals(operation);
                break;

            case command_t::adjust_relative_base:
                adjust_relative_base(operation);
                break;

            case command_t::halt:
                return status::terminated;

            default:
                std::stringstream stream;
                stream << "Error: Unknown command '" << get_address_value(m_instruction_counter - 1) << "' is specified (counter: '"
                    << m_instruction_counter - 1 << "')!";
                throw std::exception(stream.str().c_str());
            }
        }

        throw std::exception("Error: Unexpected end of the program!");
    }

    std::list<T>& get_input(void) {
        return m_input;
    }

    std::list<T>& get_output(void) {
        return m_output;
    }

private:
    std::vector<T> load_software() {
        std::vector<T> result;

        std::ifstream infile("input.txt");
        T value;
        while (infile >> value) {
            result.push_back(value);
            infile.ignore(1);
        }
        infile.close();

        return result;
    }

    instruction get_instruction() {
        T code = get_address_value(m_instruction_counter++);

        instruction result;
        result.m_command = static_cast<command_t>(static_cast<std::size_t>(code % 100));

        T code_mode = code / 100;

        result.m_mode = std::vector<mode_t>(3, mode_t::positional);
        for (std::size_t i = 0; code_mode > 0; i++) {
            result.m_mode[i] = get_mode(code_mode % 10);
            code_mode /= 10;
        }

        return result;
    }

    mode_t get_mode(const T p_value) {
        mode_t result = static_cast<mode_t>(static_cast<std::size_t>(p_value));
        switch (result) {
        case mode_t::positional:
        case mode_t::parameter:
        case mode_t::relative:
            return result;
        default:
            std::stringstream stream;
            stream << "Error: Unknown mode is specified (" << p_value << ").";
            throw std::exception(stream.str().c_str());
        }
    }

    T& get_address_value(const std::size_t p_address) {
        if (p_address >= m_memory.size()) {
            do {
                m_size_memory *= 2;
            } while (m_size_memory < p_address);

            std::vector<T> memory(m_size_memory, 0);
            std::copy(m_memory.begin(), m_memory.end(), memory.begin());
            m_memory = std::move(memory);
        }

        return m_memory[p_address];
    }

    T& get_ref_argument() {
        return get_address_value(m_instruction_counter++);
    }

    T& get_ref_value(const mode_t p_mode, T& p_code) {
        switch (p_mode) {
        case mode_t::positional:
            return get_address_value(static_cast<std::size_t>(p_code));
        case mode_t::parameter:
            return p_code;
        case mode_t::relative:
            return get_address_value(static_cast<std::size_t>(m_relative_base + p_code));
        default:
            std::stringstream stream;
            stream << "Error: Unsupported mode is specified '" << static_cast<T>(p_mode) << "'.";
            throw std::exception(stream.str().c_str());
        }
    }

    void add(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = value1 + value2;
    }

    void adjust_relative_base(const instruction& operation) {
        auto value = get_ref_value(operation.m_mode[0], get_ref_argument());
        m_relative_base += value;
    }

    void multiply(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = value1 * value2;
    }

    void read_input(const instruction& operation) {
        switch (operation.m_mode[0]) {
        case mode_t::parameter:
            throw std::exception("Error: Immediate mode is forbidden for write operation.");
        case mode_t::relative:
        case mode_t::positional: {
            auto& result = get_ref_value(operation.m_mode[0], get_ref_argument());
            result = read_input_buffer();
        }
        }
    }

    void write_output(const instruction& operation) {
        auto value = get_ref_value(operation.m_mode[0], get_ref_argument());
        m_output.push_back(value);
    }

    void jump_if_true(const instruction& operation) {
        auto condition = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto position = get_ref_value(operation.m_mode[1], get_ref_argument());

        if (condition) {
            m_instruction_counter = static_cast<std::size_t>(position);
        }
    }

    void jump_if_false(const instruction& operation) {
        auto condition = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto position = get_ref_value(operation.m_mode[1], get_ref_argument());

        if (!condition) {
            m_instruction_counter = static_cast<std::size_t>(position);
        }
    }

    void less_than(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = (value1 < value2) ? 1 : 0;
    }

    void equals(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = (value1 == value2) ? 1 : 0;
    }

    T read_input_buffer() {
        T value = m_input.front();
        m_input.erase(m_input.begin());
        return value;
    }
};


enum class panel_color {
    black = 0,
    white = 1
};


class tracker {
private:
    long m_x = 0;
    long m_y = 0;

    std::map<long, std::map<long, panel_color>> m_map;

public:
    panel_color get_color() {
        return m_map[m_y][m_x];
    }

    void color_panel(const panel_color p_color) {
        m_map[m_y][m_x] = p_color;
    }

    void move(const long dx, const long dy) {
        m_x += dx;
        m_y += dy;
    }

    std::size_t get_amount_colored_panels() const {
        std::size_t amount = 0;

        for (const auto& row : m_map) {
            amount += row.second.size();
        }

        return amount;
    }

    void show_map() {
        long left_border = 0;
        long right_border = 0;

        for (const auto& row : m_map) {
            if (row.second.begin()->first < left_border) {
                left_border = row.second.begin()->first;
            }

            if (row.second.rbegin()->first > right_border) {
                right_border = row.second.rbegin()->first;
            }
        }

        for (const auto& row : m_map) {
            for (long i = left_border; i < right_border + 1; i++) {
                auto iter_color = row.second.find(i);
                char panel = ' ';
                if (iter_color != row.second.end()) {
                    panel_color color = iter_color->second;
                    panel = (color == panel_color::black) ? ' ' : '#';
                }

                std::cout << panel;
            }
            std::cout << std::endl;
        }
    }
};



class painting_robot {
private:
    enum class direction {
        up = 0,
        right,
        down,
        left
    };

private:
    direction   m_direction = direction::up;
    tracker     m_tracker;
    integer_computer<std::int64_t> m_computer;

public:
    std::size_t process(const panel_color p_initial_color) {
        integer_computer<std::int64_t>::status status = integer_computer<std::int64_t>::status::waiting_for_input;
        update_color(p_initial_color);

        while (status != integer_computer<std::int64_t>::status::terminated) {
            panel_color color = m_tracker.get_color();
            m_computer.get_input().push_back(static_cast<std::int64_t>(color));

            status = m_computer.execute();

            panel_color new_color = get_robot_result_color();
            std::int64_t robot_turn_code = get_robot_result_turn();

            update_color(new_color);
            update_direction(robot_turn_code);
            update_position();
        }

        return m_tracker.get_amount_colored_panels();
    }

    void show_identifier() {
        m_tracker.show_map();
    }

private:
    panel_color get_robot_result_color() {
        panel_color new_color = (panel_color) *(m_computer.get_output().begin());
        m_computer.get_output().erase(m_computer.get_output().begin());
        return new_color;
    }

    std::int64_t get_robot_result_turn() {
        std::int64_t robot_turn_code = *(m_computer.get_output().begin());
        m_computer.get_output().erase(m_computer.get_output().begin());
        return robot_turn_code;
    }

    void update_color(const panel_color p_color) {
        m_tracker.color_panel(p_color);
    }

    void update_direction(const std::int64_t p_value) {
        long next_code_direction = static_cast<long>(m_direction);
        switch (p_value) {
        case 0:
            next_code_direction -= 1;
            break;

        case 1:
            next_code_direction += 1;
            break;

        default:
            throw std::exception("Error: Unknown direction change.");
        }

        if (next_code_direction < static_cast<long>(direction::up)) {
            m_direction = direction::left;
        }
        else if (next_code_direction > static_cast<long>(direction::left)) {
            m_direction = direction::up;
        }
        else {
            m_direction = static_cast<direction>(next_code_direction);
        }
    }

    void update_position() {
        switch (m_direction) {
        case direction::up:
            m_tracker.move(0, -1);
            break;
        case direction::right:
            m_tracker.move(1, 0);
            break;
        case direction::down:
            m_tracker.move(0, 1);
            break;
        case direction::left:
            m_tracker.move(-1, 0);
            break;
        default:
            throw std::exception("Error: Unkown type of direction is specified for robot.");
        }
    }
};


int main() {
    try {
        painting_robot robot;

        std::size_t amount_colored_pannels = robot.process(panel_color::black);
        std::cout << "How many panels does the painting robot paint at least once: " << amount_colored_pannels << std::endl << std::endl;

        std::cout << "Ops, initial color is wrong! The Space Police are getting impatient. Restart robot from white position." << std::endl;
        std::cout << "Registration identifier for the police: " << std::endl << std::endl;

        robot = painting_robot();
        robot.process(panel_color::white);
        robot.show_identifier();
    }
    catch (std::exception & error) {
        std::cout << error.what() << std::endl;
        return -1;
    }

    return 0;
}