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


enum class tile_type {
    empty = 0,
    wall,
    block,
    horizontal_paddle,
    ball
};


std::ostream& operator << (std::ostream& p_stream, const tile_type& p_type) {
    switch (p_type) {
    case tile_type::empty:
        p_stream << ' ';
        break;

    case tile_type::wall:
        p_stream << '*';
        break;

    case tile_type::block:
        p_stream << '#';
        break;
        
    case tile_type::horizontal_paddle:
        p_stream << '-';
        break;

    case tile_type::ball:
        p_stream << 'O';
        break;

    default:
        throw std::exception("Error: Unknown type of tile is specified.");
    }

    return p_stream;
}


struct position {
    int64_t x = 0;
    int64_t y = 0;
};


class player {
public:
    static int64_t get_joystick(const position& p_ball, const position& p_paddle) {
        if (p_ball.x > p_paddle.x) {
            return 1;
        }
        else if (p_ball.x < p_paddle.x) {
            return -1;
        }
        else {
            return 0;
        }
    }
};


class arcade_cabinet {
private:
    integer_computer<int64_t> & m_computer;

    std::map<int64_t, std::map<int64_t, tile_type>> m_map;

    int64_t x_max = 0;
    int64_t x_min = 0;
    int64_t y_max = 0;
    int64_t y_min = 0;

    position ball_position;
    position paddle_position;

    int64_t m_score = 0;
    bool m_play_for_free = false;

public:
    arcade_cabinet(integer_computer<int64_t>& p_computer, const bool p_play_for_free) : 
        m_computer(p_computer),
        m_play_for_free(p_play_for_free)
    {
        if (m_play_for_free) {
            m_computer.get_memory().at(0) = 2;
        }
    }

public:
    integer_computer<int64_t>::status make_step() {
        auto status = m_computer.execute();

        std::vector<int64_t> tile_description;
        for (auto value : m_computer.get_output()) {
            tile_description.push_back(value);

            if (tile_description.size() == 3) {
                create_tile(tile_description[0], tile_description[1], tile_description[2]);
                tile_description.clear();
            }
        }

        if (m_play_for_free) {
            int64_t action = player::get_joystick(ball_position, paddle_position);

            m_computer.get_output().clear();
            m_computer.get_input().push_back(action);
        }

        return status;
    }

    std::size_t count_tile_amount(const tile_type p_type) {
        std::size_t result = 0;
        for (const auto& p_row : m_map) {
            for (const auto tile_object : p_row.second) {
                if (tile_object.second == p_type) {
                    result++;
                }
            }
        }

        return result;
    }

    void show() {
        for (int64_t row = y_min; row < y_max; row++) {
            for (int64_t col = x_min; col < x_max; col++) {
                std::cout << m_map[row][col];
            }
            std::cout << std::endl;
        }
    }

    int64_t get_score() { return m_score; }

private:
    void create_tile(const int64_t p_x, const int64_t p_y, const int64_t p_type) {
        switch (static_cast<tile_type>(p_type)) {
        case tile_type::ball:
            ball_position.x = p_x;
            ball_position.y = p_y;
            break;

        case tile_type::horizontal_paddle:
            paddle_position.x = p_x;
            paddle_position.y = p_y;
            break;

        case tile_type::block:
        case tile_type::wall:
        case tile_type::empty:
            break;

        default:
            if ((p_x == -1) && (p_y == 0)) {
                m_score = p_type;
                break;
            }

            throw std::exception("Error: Unknown type of tile is specified.");
        }

        m_map[p_y][p_x] = static_cast<tile_type>(p_type);

        x_max = std::max(p_x, x_max);
        x_min = std::min(p_x, x_min);
        y_max = std::max(p_y, y_max);
        y_min = std::min(p_y, y_min);

        if (static_cast<tile_type>(p_type) == tile_type::ball) {
            ball_position.x = p_x;
            ball_position.y = p_y;
        }
    }
};



int main() {
    integer_computer<int64_t> computer;
    arcade_cabinet non_free_game(computer, false);

    non_free_game.make_step();

    std::cout << "Amount of block tiles on the screen when the game exits: " << non_free_game.count_tile_amount(tile_type::block) << std::endl;


    std::cout << std::endl << "Let's play now..." << std::endl;

    computer.restart();
    arcade_cabinet free_game(computer, true);

    while (free_game.make_step() != integer_computer<int64_t>::status::terminated) { }

    std::cout << "Score after the last block is broken: " << free_game.get_score() << std::endl;

    return 0;
}
