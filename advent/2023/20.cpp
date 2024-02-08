#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>


enum state_t {
    ON = 0,
    OFF
};


enum pulse_t {
    LOW = 0,
    HIGH,
    NO_PULSE
};


struct event_t {
    std::string source;
    pulse_t value = NO_PULSE;
    std::int64_t tick = 0;
};


enum module_t {
    BROADCAST = 0,
    FLIPFLOP,
    CONJUNCTION,
    PROBE
};


class module {
protected:
    module_t m_type;
    std::string m_name;

    std::list<event_t> m_queue;

public:
    module(const module_t p_type, const std::string& p_name) :
        m_type(p_type),
        m_name(p_name)
    { }

public:
    module_t get_type() const {
        return m_type;
    }

    const std::string& get_name() const {
        return m_name;
    }

    bool has_input_signal() const {
        return !m_queue.empty();
    }

    void push_signal(const std::string& p_source, const pulse_t p_pulse, const std::int64_t p_tick) {
        m_queue.push_back({ p_source, p_pulse, p_tick });
    }

    virtual void register_input_source(const std::string& p_source) { }

    virtual pulse_t handle_signal(const std::int64_t p_tick) = 0;
};


class flip_flop_module : public module {
    state_t m_state = state_t::OFF;

public:
    flip_flop_module(const std::string& p_name) :
        module(module_t::FLIPFLOP, p_name),
        m_state(state_t::OFF)
    { }

public:
    pulse_t handle_signal(const std::int64_t p_tick) override {
        if (m_queue.empty()) {
            return NO_PULSE;
        }

        auto pulse_event = m_queue.front();
        if (pulse_event.tick != p_tick) {
            return NO_PULSE;
        }

        m_queue.pop_front();

        if (pulse_event.value == pulse_t::HIGH) {
            return NO_PULSE;
        }

        if (m_state == state_t::ON) {
            m_state = state_t::OFF;
            return pulse_t::LOW;
        }
        else {
            m_state = state_t::ON;
            return pulse_t::HIGH;
        }
    }
};



class conjunction_module : public module {
private:
    std::unordered_map<std::string, pulse_t> m_inputs;

public:
    conjunction_module(const std::string& p_name) :
        module(module_t::CONJUNCTION, p_name)
    { }

public:
    pulse_t handle_signal(const std::int64_t p_tick) override {
        if (m_queue.empty()) {
            return NO_PULSE;
        }

        event_t pulse_event = m_queue.front();
        m_queue.pop_front();

        m_inputs[pulse_event.source] = pulse_event.value;

        for (const auto& pair : m_inputs) {
            if (pair.second == pulse_t::LOW) {
                return pulse_t::HIGH;
            }
        }

        return pulse_t::LOW;
    }

    void register_input_source(const std::string& p_source) override {
        m_inputs[p_source] = pulse_t::LOW;
    }
};



class broadcast_module : public module {
public:
    broadcast_module(const std::string& p_name) :
        module(module_t::BROADCAST, p_name)
    { }

public:
    pulse_t handle_signal(const std::int64_t p_tick) override {
        if (m_queue.empty()) {
            return NO_PULSE;
        }

        pulse_t output = m_queue.front().value;
        m_queue.pop_front();

        return output;
    }
};


struct stats_t {
    std::uint64_t counter_high = 0;
    std::uint64_t counter_low = 0;
};


class network {
    std::unordered_map<std::string, std::shared_ptr<module>> m_modules;
    std::unordered_map<std::string, std::vector<std::string>> m_connections;

    std::uint64_t m_counter_high = 0;
    std::uint64_t m_counter_low = 0;

    bool m_rx_low_pulse = false;
    std::string m_rx_source = "";
    std::uint64_t m_rx_counter = 0;
    std::uint64_t m_sand_machine_activation = 0;
    std::unordered_map<std::string, std::uint64_t> rx_cycle_hi;

public:
    void load() {
        std::fstream stream("input.txt");

        for (std::string line; std::getline(stream, line);) {
            module_t module_type = module_t::BROADCAST;
            std::size_t name_offset = 0;

            switch (line[0]) {
            case '%':
                module_type = module_t::FLIPFLOP;
                name_offset = 1;
                break;

            case '&':
                module_type = module_t::CONJUNCTION;
                name_offset = 1;
                break;
            }

            auto position = line.find("-");
            std::string module_name = line.substr(name_offset, position - name_offset - 1);

            std::shared_ptr<module> module_object = nullptr;
            switch (module_type) {
            case module_t::BROADCAST:
                module_object = std::make_shared<broadcast_module>(module_name);
                break;

            case module_t::CONJUNCTION:
                module_object = std::make_shared<conjunction_module>(module_name);
                break;

            case module_t::FLIPFLOP:
                module_object = std::make_shared<flip_flop_module>(module_name);
                break;

            default:
                const std::string message = "Unknown module type: '" + std::to_string(module_type) + "'";
                std::cout << message << std::endl;
                throw std::exception(message.c_str());
            }

            m_modules[module_name] = module_object;

            std::string connected_modules = line.substr(position + 3);
            std::size_t valid_position = 0;

            for (position = connected_modules.find(','); position != std::string::npos; position = connected_modules.find(',', position)) {
                std::string neighbor_name = connected_modules.substr(valid_position, position - valid_position);

                position += 2;
                valid_position = position; /* ignore ", " */

                m_connections[module_name].push_back(neighbor_name);
            }

            std::string neighbor_name = connected_modules.substr(valid_position);
            m_connections[module_name].push_back(neighbor_name);
        }

        for (const auto& pair : m_connections) {
            std::string source = pair.first;
            for (const std::string& nei : pair.second) {
                auto iter = m_modules.find(nei);
                if (iter == m_modules.end()) {
                    continue;
                }

                m_modules[nei]->register_input_source(source);
                if (m_connections[nei][0] == "rx") {
                    m_rx_source = nei;
                }
            }
        }

        for (const auto& pair : m_connections) {
            std::string source = pair.first;
            for (const std::string& nei : pair.second) {
                if (nei == m_rx_source) {
                    rx_cycle_hi[source] = 0;
                }
            }
        }
    }

    std::int64_t simulate(const std::int64_t runs) {
        for (std::int64_t i = 0; i < runs; i++) {
            stats_t run_stats = press_button(i);

            m_counter_high += run_stats.counter_high;
            m_counter_low += run_stats.counter_low;
        }

        std::uint64_t result = m_counter_high * m_counter_low;
        return result;
    }

    std::int64_t count_runs_to_activate_sand_machine() {
        std::uint64_t runs = 1;
        for (; m_rx_low_pulse != true; runs++) {
            press_button(runs);
        }

        return m_sand_machine_activation;
    }

private:
    stats_t press_button(const std::uint64_t p_run_number) {
        stats_t stats = { 0, 0 };

        std::list<std::string> queue;
        queue.push_back("broadcaster");

        m_modules["broadcaster"]->push_signal("button", pulse_t::LOW, 0);
        stats.counter_low++;

        std::int64_t current_tick = 0;
        while (!queue.empty()) {
            std::list<std::string> next_queue;

            while (!queue.empty()) {
                std::string module_name = queue.front();
                queue.pop_front();

                auto module_ptr = m_modules[module_name];
                pulse_t output_pulse = module_ptr->handle_signal(current_tick);

                for (const std::string nei_name : m_connections[module_name]) {
                    if (output_pulse == pulse_t::LOW) {
                        m_counter_low++;
                    }
                    else if (output_pulse == pulse_t::HIGH) {
                        m_counter_high++;
                    }

                    if (is_valid_pulse(output_pulse)) {
#if 0
                        std::cout << module_name << " -" << (output_pulse == pulse_t::LOW ? "low" : "high") << "-> " << nei_name << std::endl;
#endif

                        if ((nei_name == m_rx_source) && (output_pulse == pulse_t::HIGH)) {
                            if (rx_cycle_hi[module_name] == 0) {
                                rx_cycle_hi[module_name] = p_run_number;
                                m_rx_counter++;

                                if (m_rx_counter == rx_cycle_hi.size()) {
                                    std::uint64_t result = 1;
                                    for (const auto& pair : rx_cycle_hi) {
                                        result *= pair.second;
                                    }

                                    m_sand_machine_activation = result;
                                    m_rx_low_pulse = true;
                                }
                            }
                        }

                        auto iter_nei = m_modules.find(nei_name);
                        if (iter_nei == m_modules.end()) {
                            continue;
                        }

                        m_modules[nei_name]->push_signal(module_name, output_pulse, current_tick + 1);
                        next_queue.push_back(nei_name);
                    }
                }
            }

            current_tick++;
            queue = next_queue;
        }

#if 0
        std::cout << std::endl;
#endif
        return stats;
    }


    bool is_valid_pulse(const pulse_t p_pulse) const {
        return (p_pulse == pulse_t::HIGH) || (p_pulse == pulse_t::LOW);
    }
};


int main() {
    {
        network sand_flow_network;
        sand_flow_network.load();
        std::uint64_t result = sand_flow_network.simulate(1000);

        std::cout << "Multiplication of sent pulses: " << result << std::endl;
    }

    {
        network sand_flow_network;
        sand_flow_network.load();
        std::uint64_t result = sand_flow_network.count_runs_to_activate_sand_machine();

        std::cout << "The number of time when the button is needed to be pressed to run the machine: " << result << std::endl;
    }

    return 0;
}
