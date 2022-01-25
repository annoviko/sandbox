#include <iostream>
#include <fstream>
#include <unordered_map>


using fish_age_map = std::unordered_map<std::size_t, std::size_t>;


class population_analyser {
private:
    constexpr static std::size_t DAYS_TO_PRODUCE = 0;
    constexpr static std::size_t DAYS_NEW_BIRTH = 8;
    constexpr static std::size_t DAYS_RESET = 6;

private:
    fish_age_map m_population;

public:
    population_analyser(const fish_age_map& p_population) : m_population(p_population) { }

public:
    std::size_t run(const std::size_t p_days) {
        for (std::size_t i = 0; i < p_days; i++) {
            simulate();
        }

        std::size_t population = 0;
        for (const auto & pair : m_population) {
            population += pair.second;
        }

        return population;
    }

private:
    void simulate() {
        fish_age_map next_population;

        for (const auto& pair : m_population) {
            if (pair.first == 0) {
                next_population[DAYS_RESET] += pair.second;
                next_population[DAYS_NEW_BIRTH] += pair.second;
            }
            else {
                next_population[pair.first - 1] += pair.second;
            }
        }

        m_population = std::move(next_population);
    }
};


fish_age_map read_input() {
    fish_age_map result;

    std::ifstream stream("input.txt");
    for(std::size_t p_value = 0; !stream.eof();) {
        stream >> p_value;
        stream.ignore(1);
        result[p_value]++;
    }

    return result;
}


int main() {
    fish_age_map population = read_input();

    std::cout << "Population after 80 days: " << population_analyser(population).run(80) << std::endl;
    std::cout << "Population after 256 days: " << population_analyser(population).run(256) << std::endl;

    return 0;
}
