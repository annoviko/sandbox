#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
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


std::vector<std::string> get_map() {
    std::vector<std::string> ascii_map = { "" };

    integer_computer<int32_t> computer;
    computer.execute();
    auto raw_map = computer.get_output();

    for (auto symbol : raw_map) {
        if (symbol != 10) {
            ascii_map.back().push_back(symbol);
        }
        else {
            if (!ascii_map.back().empty()) {
                ascii_map.push_back(std::string());
            }
        }
    }

    if (ascii_map.back().empty()) {
        ascii_map.erase(ascii_map.end() - 1);
    }

    return ascii_map;
}


struct position {
    int x = -1;
    int y = -1;

    bool invalid() {
        return (x == -1) && (y == -1);
    }
};


class intersection_finder {
private:
    std::vector<std::string> m_map;

public:
    intersection_finder(const std::vector<std::string> & p_map) :
        m_map(p_map)
    { }

public:
    std::size_t get_sum_alignment() {
        std::size_t result = 0;

        auto intersections = find_intersections();
        for (const auto& pos : intersections) {
            result += pos.x * pos.y;
        }

        return result;
    }

private:
    std::vector<position> find_intersections() {
        std::vector<position> result;

        for (int i = 1; i < m_map.size() - 1; i++) {
            for (int j = 1; j < m_map.at(i).size() - 1; j++) {
                if (m_map[i][j] != '#') { continue; }

                std::size_t energy = calculate_energy(i, j);
                if (energy > 2) {
                    result.push_back({ j, i });
                }
            }
        }

        return result;
    }

    std::size_t calculate_energy(std::size_t row, std::size_t col) {
        return get_energy(row, col - 1) + get_energy(row, col + 1) +
            get_energy(row - 1, col) + get_energy(row + 1, col);
    }

    std::size_t get_energy(std::size_t row, std::size_t col) {
        if (row >= m_map.size() || col >= m_map.at(row).size()) {
            return 0;
        }
        return m_map[row][col] == '#' ? 1 : 0;
    }
};


class traverser {
private:
    std::vector<std::string> m_map;
    position m_robot;
    char m_orientation = ' ';

public:
    traverser(const std::vector<std::string> & p_map) : m_map(p_map) {
        find_robot();
    }

public:
    std::vector<std::string> traverse() {
        std::vector<std::string> path;

        std::size_t steps = 0;

        while (true) {
            auto next_pos = get_next_position();
            if (is_able_go_forward(next_pos)) {
                steps++;
                m_robot = next_pos;
            }
            else {
                if (steps > 0) {
                    path.push_back(std::to_string(steps));
                }

                auto dir = get_next_direction(m_robot);
                if (std::get<0>(dir) == 0) {
                    return path;
                }

                m_orientation = std::get<0>(dir);
                path.push_back(std::string(1, std::get<1>(dir)));

                steps = 0;
            }
        }

        throw std::exception("Error: Impossible to traverse the map.");
    }

private:
    std::tuple<char, char> get_next_direction(const position& p_pos) {
        auto right = get_right();
        auto left = get_left();

        if (!std::get<0>(right).invalid()) {
            if (is_valid_path(std::get<0>(right))) {
                return { std::get<1>(right), 'R' };
            }
        }

        if (!std::get<0>(left).invalid()) {
            if (is_valid_path(std::get<0>(left))) {
                return { std::get<1>(left), 'L' };
            }
        }

        return { 0, 0 };
    }

    bool is_valid_path(const position & p_pos) {
        if (p_pos.y < 0 || p_pos.y >= m_map.size()) {
            return false;
        }

        if (p_pos.x < 0 || p_pos.x >= m_map.at(p_pos.y).size()) {
            return false;
        }

        return m_map[p_pos.y][p_pos.x] == '#';
    }

    std::tuple<position, char> get_right() {
        position cur = m_robot;
        switch (m_orientation) {
        case '^':
            return { { cur.x + 1, cur.y }, '>' };
        case 'v':
            return { { cur.x - 1, cur.y }, '<' };
        case '>':
            return { { cur.x, cur.y + 1 }, 'v' };
        case '<':
            return { { cur.x, cur.y - 1 }, '^' };
        default:
            throw std::exception("Error: robot is not found.");
        }
    }

    std::tuple<position, char> get_left() {
        position cur = m_robot;
        switch (m_orientation) {
        case '^':
            return { { cur.x - 1, cur.y }, '<' };
        case 'v':
            return { { cur.x + 1, cur.y }, '>' };
        case '>':
            return { { cur.x, cur.y - 1 }, '^' };
        case '<':
            return { { cur.x, cur.y + 1 }, 'v' };
        default:
            throw std::exception("Error: robot is not found.");
        }
    }

    bool is_able_go_forward(const position & p_pos) {
        return is_valid_path(p_pos);
    }

    position get_next_position() {
        position cur = m_robot;
        switch (m_orientation) {
        case '^':
            return { cur.x, cur.y - 1 };
        case 'v':
            return { cur.x, cur.y + 1 };
        case '>':
            return { cur.x + 1, cur.y };
        case '<':
            return { cur.x - 1, cur.y };
        default:
            throw std::exception("Error: robot is not found.");
        }
    }

    void find_robot() {
        for (int row = 0; row < m_map.size(); row++) {
            for (int col = 0; col < m_map.at(row).size(); col++) {
                const auto sym = m_map[row][col];

                switch (sym) {
                case '^':
                case 'v':
                case '>':
                case '<':
                    m_orientation = sym;
                    m_robot = { col, row };
                    return;
                default:
                    break;
                }
            }
        }
    }
};


struct function {
    char name = ' ';
    std::vector<std::string> steps;
};


std::ostream& operator<<(std::ostream& p_stream, const function& p_func) {
    p_stream << p_func.name << ": ";
    for (std::size_t i = 0; i < p_func.steps.size() - 1; i++) {
        p_stream << p_func.steps.at(i) << ",";
    }
    
    if (!p_func.steps.empty()) {
        p_stream << p_func.steps.back();
    }

    return p_stream;
}


class compressor {
private:
    std::vector<std::string> m_actions;

public:
    compressor(const std::vector<std::string> & p_actions) : m_actions(p_actions) { }

public:
    std::tuple<std::vector<function>, std::string> compress() {
        function fa = { 'A', {} }, fb = { 'B', {} }, fc = { 'C', {} };

        constexpr std::size_t max_value = 20;

        std::vector<std::size_t> limits = { 4, 4, 4 };
        std::vector<char> belong;

        bool wrong = false;
        do {
            belong = std::vector<char>(m_actions.size(), ' ');

            find_longest_function(belong, fa.name, fa.steps, limits[0]);
            find_longest_function(belong, fb.name, fb.steps, limits[1]);
            find_longest_function(belong, fc.name, fc.steps, limits[2]);

            wrong = false;
            if (fb.steps.size() > 20) {
                wrong = true;
            }
            if (fc.steps.size() > 20) {
                wrong = true;
            }
            for (const auto& item : belong) {
                if (item == ' ') {
                    wrong = true;
                    break;
                }
            }

            if (wrong) {
                increment(limits);
            }
        } while (wrong);

        std::string path;
        for (std::size_t i = 0; i < belong.size();) {
            path.push_back(belong[i]);
            switch (belong[i]) {
            case 'A':
                i += fa.steps.size();
                break;
            case 'B':
                i += fb.steps.size();
                break;
            case 'C':
                i += fc.steps.size();
                break;
            }
        }

        return { std::vector<function>({ fa, fb, fc }), path };
    }

private:
    void increment(std::vector<std::size_t>& p_limits) {
        p_limits[0] += 2;
        if (p_limits[0] <= 20) { return; }

        p_limits[0] = 4;
        p_limits[1] += 2;

        if (p_limits[1] <= 20) { return; }

        p_limits[1] = 4;
        p_limits[2] += 2;
        if (p_limits[2] > 20) {
            throw std::exception();
        }
    }

    void find_longest_function(std::vector<char> & p_belong, char p_name, std::vector<std::string> & p_func, std::size_t p_limit) {
        std::size_t offset = 0;

        std::vector<std::string> candidate;
        std::vector<char> belong_candidate = p_belong;

        for (; offset < m_actions.size(); offset += 2) {
            if (belong_candidate[offset] == ' ') {
                break;
            }
        }

        for (std::size_t k = offset; (k < m_actions.size()) && (k < offset + p_limit); k++) {
            if (belong_candidate[k] != ' ') {
                return;
            }

            candidate.push_back(m_actions[k]);
            belong_candidate[k] = p_name;
        }

        std::size_t amount = 1;
        std::size_t size = candidate.size();

        for (std::size_t j = offset + size; j <= m_actions.size() - size;) {
            if (belong_candidate[j] != ' ') {
                j += 2;
                continue;
            }

            if (is_same(j, j + size, candidate)) {
                amount++;
                mark_function(belong_candidate, p_name, j, j + size);
                j += size;
            }
            else {
                j += 2;
            }
        }

        if (amount >= 2) {
            p_belong = belong_candidate;
            p_func = candidate;
        }
    }

    void mark_function(std::vector<char> & p_belong, char p_name, std::size_t p_begin, std::size_t p_end) {
        if (p_end > p_belong.size()) {
            return;
        }

        for (std::size_t i = p_begin; i < p_end; i++) {
            p_belong[i] = p_name;
        }
    }

    bool is_same(std::size_t p_begin, std::size_t p_end, std::vector<std::string> p_func) {
        if (p_end > m_actions.size()) {
            return false;
        }

        for (std::size_t i = p_begin; i < p_end; i++) {
            if (m_actions[i] != p_func[i - p_begin]) {
                return false;
            }
        }

        return true;
    }
};


class robot {
private:
    integer_computer<std::int64_t> m_computer;
    std::string m_calls;
    function m_fa;
    function m_fb;
    function m_fc;

    std::map<int, char> m_ascii = { {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'}, {0, '0'} };

public:
    robot(const std::string & p_path, const function& p_fa, const function& p_fb, const function& p_fc) :
        m_calls(p_path), m_fa(p_fa), m_fb(p_fb), m_fc(p_fc)
    {
        m_computer.get_memory().at(0) = 2;

        declare_functions();

        define_function(m_fa);
        define_function(m_fb);
        define_function(m_fc);

        m_computer.get_input().push_back('n');
        m_computer.get_input().push_back(10);
    }

public:
    std::list<int64_t> run() {
        auto status = m_computer.execute();
        if (status != integer_computer<int64_t>::status::terminated) {
            throw std::exception("Robot hasn't finished its job.");
        }

        return m_computer.get_output();
    }

private:
    void declare_functions() {
        m_computer.get_input().push_back(m_calls[0]);
        for (std::size_t i = 1; i < m_calls.size(); i++) {
            m_computer.get_input().push_back(',');
            m_computer.get_input().push_back(m_calls[i]);
        }
        m_computer.get_input().push_back(10);
    }

    void define_function(const function& p_func) {
        for (std::size_t i = 0; i < p_func.steps.size(); i += 2) {
            m_computer.get_input().push_back(p_func.steps[i][0]);  // action
            m_computer.get_input().push_back(',');
            
            for (const auto digit : p_func.steps[i + 1]) {
                m_computer.get_input().push_back(digit);
            }
            m_computer.get_input().push_back(',');
        }
        m_computer.get_input().pop_back();
        m_computer.get_input().push_back(10);
    }
};


int main() {
    auto ascii_map = get_map();
    std::cout << intersection_finder(ascii_map).get_sum_alignment() << std::endl;

    std::cout << "Map:" << std::endl;
    for (auto& row : ascii_map) {
        std::cout << row << std::endl;
    }

    auto instructions = traverser(ascii_map).traverse();

    std::cout << std::endl << "Instruction: " << instructions[0];
    for (std::size_t i = 1; i < instructions.size(); i++) {
        std::cout << "," << instructions[i];
    }
    std::cout << std::endl;

    auto result = compressor(instructions).compress();

    auto & funcs = std::get<0>(result);
    std::cout << std::endl << "Functions:" << std::endl;
    for (auto& fn : funcs) {
        std::cout << fn << std::endl;
    }

    std::cout << std::endl << "Path: " << std::get<1>(result) << std::endl;

    robot secure(std::get<1>(result), funcs[0], funcs[1], funcs[2]);
    auto report = secure.run();

    std::cout << std::endl << "Amount of collected dust: ";
    for (auto iter = report.begin(); iter != report.end(); iter++) {
        if (*iter > 127) {
            std::cout << *iter;
        }
    }
    std::cout << std::endl;

    return 0;
}