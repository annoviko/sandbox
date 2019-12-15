#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <unordered_map>
#include <numeric>
#include <sstream>
#include <string>


struct recipe {
public:
    std::string m_product;
    std::size_t m_amount;

    std::map<std::string, std::size_t> m_ingredients;
};


using recipe_book = std::map<std::string, recipe>;


class recipe_reader {
public:
    recipe_book read_recipes() {
        recipe_book result;

        std::ifstream stream("input.txt");

        std::string recipe_line;
        while (std::getline(stream, recipe_line)) {
            recipe rcp = parse(recipe_line);
            result[rcp.m_product] = rcp;
        }

        stream.close();
        return result;
    }

private:
    recipe parse(const std::string& p_line) {
        recipe rcp;

        std::stringstream stream(p_line);
        state_extract_ingredient(stream, rcp);

        std::string continue_sign;
        stream >> continue_sign;
        if (continue_sign != "=>") {
            throw std::runtime_error("Error: Incorrect format - no split is found between ingredients and product.");
        }

        state_extract_product(stream, rcp);

        return rcp;
    }

    void state_extract_ingredient(std::stringstream& p_stream, recipe& p_rcp) {
        std::size_t amount = 0;
        std::string ingredient;

        bool proceed = true;

        do {
            p_stream >> amount;
            p_stream >> ingredient;

            if (ingredient.back() == ',') {
                ingredient.erase(ingredient.size() - 1, 1);
            }
            else {
                proceed = false;
            }

            p_rcp.m_ingredients[ingredient] = amount;
        } while (proceed);
    }

    void state_extract_product(std::stringstream& p_stream, recipe& p_rcp) {
        p_stream >> p_rcp.m_amount;
        p_stream >> p_rcp.m_product;
    }
};


class fuel_calculator {
private:
    std::map<std::string, std::size_t> m_warehouse;

public:
    std::size_t calculate(const recipe_book& p_book, const std::size_t p_amount) {
        return produce(p_book, "FUEL", p_amount);
    }

private:
    std::size_t produce(const recipe_book& p_book, const std::string & p_product, std::size_t p_amount) {
        std::map<std::string, std::size_t> to_produce = { { p_product, p_amount } };

        std::size_t ore = 0;

        while (!to_produce.empty()) {
            auto iter_product = to_produce.begin();
            auto product = *iter_product;

            to_produce.erase(iter_product);

            if (product.first == "ORE") {
                ore += product.second;
            }
            else {
                std::size_t product_amount = product.second;

                std::size_t batch_amount = p_book.at(product.first).m_amount;
                std::size_t required_iteration_to_produce = std::ceil((double) product_amount / (double) batch_amount);

                m_warehouse[product.first] = batch_amount * required_iteration_to_produce - product_amount;

                for (const auto& ingredients : p_book.at(product.first).m_ingredients) {
                    std::size_t total_amount_ingredients = ingredients.second * required_iteration_to_produce;

                    std::size_t available_product = std::min(m_warehouse[ingredients.first], total_amount_ingredients);
                    m_warehouse[ingredients.first] -= available_product;
                    total_amount_ingredients -= available_product;

                    if (total_amount_ingredients > 0) {
                        to_produce[ingredients.first] += total_amount_ingredients;
                    }
                }
            }
        }

        return ore;
    }
};


std::tuple<std::size_t, std::size_t> get_borders(const recipe_book & p_book, const std::size_t p_ore_for_one) {
    const std::size_t target = 1000000000000;

    std::size_t right_border = 0;
    std::size_t left_border = (target / p_ore_for_one) * 2;

    return { left_border, right_border };
}


std::size_t binary_search(const recipe_book& p_book, std::size_t p_left, std::size_t p_right) {
    const std::size_t target = 1000000000000;

    std::size_t ore = 0;
    std::size_t middle = 0;
    std::size_t result = p_right;

    while (p_left > p_right) {
        middle = (p_left + p_right + 1) / 2;
        ore = fuel_calculator().calculate(p_book, middle);

        if (ore > target) {
            p_left = middle - 1;
        }
        else if (ore < target) {
            p_right = middle;
        }
        else {
            return middle;
        }
    }

    return p_right;
}


int main() {
    auto book = recipe_reader().read_recipes();

    auto calculator = fuel_calculator();

    std::int64_t ore_amount = calculator.calculate(book, 1);

    std::cout << "Minimum amount of ORE required to produce exactly 1 FUEL: " << ore_amount << std::endl;

    auto borders = get_borders(book, ore_amount);
    auto result = binary_search(book, std::get<0>(borders), std::get<1>(borders));

    std::cout << "Maximum amount of FUEL: " << result << std::endl;

    return 0;
}