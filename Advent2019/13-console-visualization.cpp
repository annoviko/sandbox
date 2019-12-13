#include <algorithm>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <vector>
#include <string>
#include <sstream>
#include <thread>

#include <windows.h>

#undef max
#undef min


using namespace std::chrono_literals;


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
        p_stream << 'o';
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


struct notify_request {
public:
    int64_t x = 0;
    int64_t y = 0;
    int64_t tile = 0;

public:
    notify_request(const int64_t p_x, const int64_t p_y, const int64_t p_tile) :
        x(p_x), y(p_y), tile(p_tile)
    { }
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


class observer {
public:
    using ptr = std::shared_ptr<observer>;

public:
    virtual void notify(const std::vector <std::vector<tile_type>>& p_game_field) = 0;

    virtual void notify(const std::list<notify_request> m_event_chain) = 0;
};


class console_colored_visualizer : public observer {
private:
    HANDLE m_cout;

    const short m_y_offset = 4;

public:
    console_colored_visualizer() : m_cout(GetStdHandle(STD_OUTPUT_HANDLE)) {
        CONSOLE_CURSOR_INFO cursor_info;

        SetConsoleTextAttribute(m_cout, 0x0A);
        std::cout /*<< std::endl*/ << "   ADVENT OF CODE 2019 - ARCADE CABINET" << std::endl;
        SetConsoleTextAttribute(m_cout, 0x08);

        GetConsoleCursorInfo(m_cout, &cursor_info);
        cursor_info.bVisible = false;
        SetConsoleCursorInfo(m_cout, &cursor_info);
    }

public:
    void notify(const std::vector <std::vector<tile_type>>& p_game_field) override {
        SetConsoleCursorPosition(m_cout, { 0, m_y_offset });

        for (int64_t row = 0; row < p_game_field.size(); row++) {
            for (int64_t col = 0; col < p_game_field.at(0).size(); col++) {
                draw_tile(p_game_field[row][col]);
            }
            std::cout << std::endl;
        }

        std::this_thread::sleep_for(100ms);
    }

    void notify(const std::list<notify_request> m_event_chain) override {
        for (const auto& ev : m_event_chain) {
            if ((ev.x == -1) && (ev.y == 0)) {
                update_score(ev.tile);
            }
            else {
                update_tile(ev.x, ev.y + m_y_offset, static_cast<tile_type>(ev.tile));
            }
        }

        std::this_thread::sleep_for(25ms);
    }

private:
    void update_score(const std::int64_t p_score) {
        SetConsoleCursorPosition(m_cout, { 0, 2 });

        SetConsoleTextAttribute(m_cout, 0x0F);
        std::cout << "  SCORE: " << p_score;
        SetConsoleTextAttribute(m_cout, 0x08);
    }

    void update_tile(const std::int64_t p_x, const std::int64_t p_y, const tile_type p_tile) {
        SetConsoleCursorPosition(m_cout, { static_cast<short>(p_x), static_cast<short>(p_y) });
        draw_tile(p_tile);
    }

    void draw_tile(const tile_type p_tile) {
        switch (p_tile) {
        case tile_type::empty:
            std::cout << ' ';
            break;

        case tile_type::wall:
            SetConsoleTextAttribute(m_cout, 0xFF);
            std::cout << '*';
            SetConsoleTextAttribute(m_cout, 0x08);
            break;

        case tile_type::block:
            SetConsoleTextAttribute(m_cout, 0x0E);
            std::cout << '#';
            SetConsoleTextAttribute(m_cout, 0x08);
            break;

        case tile_type::horizontal_paddle:
            SetConsoleTextAttribute(m_cout, 0x09);
            std::cout << '_';
            SetConsoleTextAttribute(m_cout, 0x08);
            break;

        case tile_type::ball:
            SetConsoleTextAttribute(m_cout, 0x0A);
            std::cout << 'o';
            SetConsoleTextAttribute(m_cout, 0x08);
            break;

        default:
            throw std::exception("Error: Unknown type of tile is specified.");
        }
    }
};


class arcade_cabinet {
private:
    integer_computer<int64_t>& m_computer;

    std::map<int64_t, std::map<int64_t, tile_type>> m_map;
    std::vector <std::vector<tile_type>> m_fast_map;

    int64_t x_max = std::numeric_limits<int64_t>::min();
    int64_t x_min = std::numeric_limits<int64_t>::max();
    int64_t y_max = std::numeric_limits<int64_t>::min();
    int64_t y_min = std::numeric_limits<int64_t>::max();

    position ball_position;
    position paddle_position;

    int64_t m_score = 0;
    bool m_play_for_free = false;

    std::vector<std::shared_ptr<observer>> m_observers;

public:
    arcade_cabinet(integer_computer<int64_t>& p_computer, const bool p_play_for_free) :
        m_computer(p_computer),
        m_play_for_free(p_play_for_free)
    {
        if (m_play_for_free) {
            m_computer.get_memory().at(0) = 2;
        }

        execute_and_process_computer_output();
        build_fast_field();
    }

public:
    void register_observer(std::shared_ptr<observer> p_observer) {
        if (!p_observer) {
            throw std::exception("Error: Nullptr observer is specified.");
        }

        m_observers.push_back(p_observer);
        p_observer->notify(m_fast_map);
    }

    integer_computer<int64_t>::status make_step() {
        auto status = execute_and_process_computer_output();

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

    int64_t get_score() {
        return m_score;
    }

private:
    integer_computer<int64_t>::status execute_and_process_computer_output() {
        auto status = m_computer.execute();

        std::vector<int64_t> tile_description;
        std::list<notify_request> event_chain;
        for (auto value : m_computer.get_output()) {
            tile_description.push_back(value);

            if (tile_description.size() == 3) {
                bool result = create_tile(tile_description[0], tile_description[1], tile_description[2]);

                if (result) {
                    event_chain.emplace_back(tile_description[0] - x_min, tile_description[1] - y_min, tile_description[2]);
                }
                else {
                    event_chain.emplace_back(tile_description[0], tile_description[1], tile_description[2]);
                }

                tile_description.clear();
            }
        }

        notify(event_chain);

        return status;
    }

    tile_type& get_tile_by_coordinates(const int64_t p_x, const int64_t p_y) {
        if (m_fast_map.empty()) {
            return m_map[p_y][p_x];
        }

        return m_fast_map[p_y - y_min][p_x - x_min];
    }

    bool create_tile(const int64_t p_x, const int64_t p_y, const int64_t p_type) {
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
                return false;
            }

            throw std::exception("Error: Unknown type of tile is specified.");
        }

        get_tile_by_coordinates(p_x, p_y) = static_cast<tile_type>(p_type);

        x_max = std::max(p_x, x_max);
        x_min = std::min(p_x, x_min);
        y_max = std::max(p_y, y_max);
        y_min = std::min(p_y, y_min);

        if (static_cast<tile_type>(p_type) == tile_type::ball) {
            ball_position.x = p_x;
            ball_position.y = p_y;
        }

        return true;
    }

    void notify() {
        for (auto& observer : m_observers) {
            if (observer != nullptr) {
                observer->notify(m_fast_map);
            }
        }
    }

    void notify(const std::list<notify_request> & p_event_chain) {
        for (auto& observer : m_observers) {
            if (observer != nullptr) {
                observer->notify(p_event_chain);
            }
        }
    }

    void build_fast_field() {
        for (int64_t row = y_min; row <= y_max; row++) {
            m_fast_map.push_back({ });
            for (int64_t col = x_min; col <= x_max; col++) {
                m_fast_map.back().push_back(m_map[col][row]);
            }
        }
    }
};



int main() {
    getchar();
    integer_computer<int64_t> computer;
    arcade_cabinet free_game(computer, true);

    observer::ptr visualizer = std::make_shared<console_colored_visualizer>();
    free_game.register_observer(visualizer);

    while (free_game.make_step() != integer_computer<int64_t>::status::terminated) {}

    std::cout << "Score after the last block is broken: " << free_game.get_score() << std::endl;

    return 0;
}
