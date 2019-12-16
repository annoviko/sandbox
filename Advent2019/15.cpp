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

    std::list<T>& get_input() {
        return m_input;
    }

    std::list<T>& get_output() {
        return m_output;
    }

    std::vector<T>& get_memory() {
        return m_memory;
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


struct maze_object {
    enum class type {
        unknown,
        wall,
        empty,
        oxygen_system,
    };

    type    m_type      = type::unknown;
    bool    m_visited   = false;
    bool    m_traversed = false;
};


std::ostream& operator<<(std::ostream& p_stream, const maze_object p_object) {
    switch (p_object.m_type) {
    case maze_object::type::unknown:
        p_stream << " ";

        break;
    case maze_object::type::wall:
        p_stream << "#";
        break;
    case maze_object::type::empty:
        p_stream << ".";
        break;
    case maze_object::type::oxygen_system:
        p_stream << "$";
        break;
    default:
        throw std::exception();
    }

    return p_stream;
}


struct position {
    int64_t     x = 0;
    int64_t     y = 0;
};


enum class direction {
    north = 1,
    south,
    west,
    east
};


class robot {
private:
    std::map<std::int64_t, std::map<std::int64_t, maze_object>> m_map;
    int64_t x_min = 0;
    int64_t x_max = 0;
    int64_t y_min = 0;
    int64_t y_max = 0;

    integer_computer<int64_t> m_computer;
    direction m_dir = direction::north;

    position m_final = { 0, 0 };
    position m_current = { 0, 0 };

    bool m_found = false;

public:
    std::size_t find_oxygen_system() {
        m_map[m_current.y][m_current.x] = { maze_object::type::empty, false };
        while (!m_found) {
            make_step();
        }

        return find_optimal_steps();
    }

private:
    void make_step() {
        turn_right(); /* turn right */
        while (!execute()) { /* and try to go */
            turn_left();    /* if can't than turn left */
        }
    }

    direction get_oposite(const direction& p_dir) {
        switch (m_dir) {
        case direction::north:
            m_dir = direction::south;
            break;
        case direction::east:
            m_dir = direction::west;
            break;
        case direction::south:
            m_dir = direction::north;
            break;
        case direction::west:
            m_dir = direction::east;
            break;
        default:
            throw std::exception("Error: Incorrect state of direction.");
        }
    }

    void turn_right() {
        switch (m_dir) {
        case direction::north:
            m_dir = direction::east;
            break;
        case direction::east:
            m_dir = direction::south;
            break;
        case direction::south:
            m_dir = direction::west;
            break;
        case direction::west:
            m_dir = direction::north;
            break;
        default:
            throw std::exception("Error: Incorrect state of direction.");
        }
    }

    void turn_left() {
        switch (m_dir) {
        case direction::north:
            m_dir = direction::west;
            break;
        case direction::west:
            m_dir = direction::south;
            break;
        case direction::south:
            m_dir = direction::east;
            break;
        case direction::east:
            m_dir = direction::north;
            break;
        default:
            throw std::exception("Error: Incorrect state of direction.");
        }
    }

    bool execute() {
        m_computer.get_input().push_back(static_cast<int64_t>(m_dir));
        auto result = run_integer_computer();

        auto next_position = get_next_position(m_current, m_dir);
        auto& cell = m_map[next_position.y][next_position.x];
        if (cell.m_type == maze_object::type::unknown) {
            update_maze_info(next_position, result, cell);
        }

        if (cell.m_type == maze_object::type::oxygen_system) {
            m_final = next_position;
            m_found = true;
        }

        bool position_changed = (cell.m_type != maze_object::type::wall);
        if (position_changed) {
            m_current = next_position;
        }
        return position_changed;
    }

    void update_borders(const position & p_pos) {
        x_min = std::min(x_min, p_pos.x);
        x_max = std::max(x_max, p_pos.x);
        y_min = std::min(y_min, p_pos.y);
        y_max = std::max(y_max, p_pos.y);
    }

    int64_t run_integer_computer() {
        while (m_computer.get_output().empty()) {
            m_computer.execute();
        }

        auto result = m_computer.get_output().back();
        m_computer.get_output().clear();

        return result;
    }

    void update_maze_info(const position& next_position, const int64_t code, maze_object& cell) {
        switch (code) {
        case 0:
            cell.m_type = maze_object::type::wall;
            break;
        case 1:
            cell.m_type = maze_object::type::empty;
            break;
        case 2:
            cell.m_type = maze_object::type::oxygen_system;
            break;
        default:
            throw std::exception("Error: Incorrect response from integer computer.");
        }

        update_borders(next_position);
    }

    position get_next_position(const position p_cur, const direction& p_dir) const {
        switch (p_dir) {
        case direction::north:
            return { p_cur.x, p_cur.y - 1 };
        case direction::south:
            return { p_cur.x, p_cur.y + 1 };
        case direction::east:
            return { p_cur.x + 1, p_cur.y };
        case direction::west:
            return { p_cur.x - 1, p_cur.y };
        default:
            throw std::exception("Error: Unknown direction.");
        }
    }

    void show() {
        std::cout << std::endl << std::endl;
        for (auto row = y_min; row <= y_max; row++) {
            for (auto col = x_min; col <= x_max; col++) {
                if (row == m_current.y && col == m_current.x) {
                    std::cout << "@";
                }
                else {
                    std::cout << m_map[row][col];
                }
            }
            std::cout << std::endl;
        }
    }

    std::size_t find_optimal_steps() {
        std::size_t result = 0;

        std::list<position> cur_level;
        std::list<position> next_level;

        next_level.push_back({ 0, 0 });
        m_map[0][0].m_traversed = true;

        while (!next_level.empty()) {
            cur_level = std::move(next_level);
            result++;

            for (auto & cur_pos : cur_level) {
                for (std::size_t i = 1; i <= 4; i++) {
                    auto dir = static_cast<direction>(i);
                    auto pos = get_next_position(cur_pos, dir);

                    auto& cell = m_map[pos.y][pos.x];

                    if (cell.m_traversed) {
                        continue;
                    }

                    cell.m_traversed = true;

                    if (cell.m_type == maze_object::type::empty) {
                        next_level.push_back(pos);
                    }
                    else if (cell.m_type == maze_object::type::oxygen_system) {
                        return result;
                    }
                }
            }
        }

        return result;
    }
};


int main() {
    robot repair_robot;
    auto steps = repair_robot.find_oxygen_system();
    std::cout << "Optimal amount of steps: " << steps << std::endl;
    return 0;
}
