#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <set>


using recipe_entry = std::set<std::string>;


struct recipe {
    recipe_entry m_ingredients;
    recipe_entry m_allergens;
};


struct product {
    std::string m_product;
    std::string m_allergen;
};


using recipe_container = std::list<recipe>;


recipe_container read(const std::string & p_file) {
    const static std::regex pattern_basic("(.*) \\(contains (.*)\\)");

    auto getter = [](const std::string & p_content, const std::string p_token, std::set<std::string> & p_result) {
        std::size_t lborder = 0;
        std::size_t rborder = p_content.find(p_token, lborder);

        while (rborder != std::string::npos) {
            const std::string value = p_content.substr(lborder, rborder - lborder);
            p_result.insert(value);

            lborder = rborder + p_token.size();
            rborder = p_content.find(p_token, lborder);
        }

        const std::string value = p_content.substr(lborder);
        p_result.insert(value);
    };

    std::ifstream stream(p_file);
    recipe_container result;

    for (std::string line; std::getline(stream, line); ) {
        recipe entry;

        std::smatch groups;
        if (!std::regex_match(line, groups, pattern_basic)) {
            throw std::exception("Unexpected recipe input string.");
        }

        getter(groups[1], " ", entry.m_ingredients);
        getter(groups[2], ", ", entry.m_allergens);

        result.push_back(std::move(entry));
    }

    return result;
}


class allocator {
private:
    recipe_container m_container;

    std::list<product> m_nonfree;

public:
    allocator(const recipe_container & p_container) :
        m_container(p_container)
    { }

public:
    std::size_t get_allergen_free() {
        process();
        return count_free_ingerients();
    }

    std::string get_dangerous_ingredients() {
        m_nonfree.sort([](const product & p_a, const product & p_b) {
            return p_a.m_allergen < p_b.m_allergen;
        });

        std::string result = m_nonfree.front().m_product;
        for (auto iter = std::next(m_nonfree.begin()); iter != m_nonfree.end(); iter++) {
            result += "," + iter->m_product;
        }

        return result;
    }

private:
    std::size_t count_free_ingerients() {
        std::size_t result = 0;
        for (auto & entry : m_container) {
            result += entry.m_ingredients.size();
        }

        return result;
    }

    void reduce(const std::string & p_ingredient, const std::string & p_allergen) {
        m_nonfree.push_back({ p_ingredient, p_allergen });

        std::set<std::pair<std::string, std::string>> to_erase;

        for (auto & entry : m_container) {
            entry.m_ingredients.erase(p_ingredient);
            entry.m_allergens.erase(p_allergen);

            if (entry.m_ingredients.size() == 1 && entry.m_allergens.size() == 1) {
                const std::string intredient_to_remove = *entry.m_ingredients.begin();
                const std::string allergen_to_remove = *entry.m_allergens.begin();

                to_erase.insert({ intredient_to_remove, allergen_to_remove });
            }
        }

        /* there are new ingredients to remove */
        for (auto & pair_to_remove : to_erase) {
            reduce(pair_to_remove.first, pair_to_remove.second);
        }
    }

    void process() {
        /* Complexity: O(n^3) */
        for (auto cur_iter = m_container.begin(); cur_iter != m_container.end(); cur_iter++) {
            if (cur_iter->m_allergens.empty()) {
                continue;
            }

            for (auto next_iter = std::next(m_container.begin()); next_iter != m_container.end(); next_iter++) {
                recipe_entry cur_ingredients = cur_iter->m_ingredients;
                recipe_entry cur_allergens = cur_iter->m_allergens;

                if (next_iter->m_allergens.empty()) {
                    continue;
                }

                for (auto iter = next_iter; iter != m_container.end(); iter++) {
                    recipe_entry intersection_ingredients;
                    recipe_entry intersection_allergens;

                    std::set_intersection(cur_ingredients.begin(), cur_ingredients.end(), iter->m_ingredients.begin(), iter->m_ingredients.end(), std::inserter(intersection_ingredients, intersection_ingredients.begin()));
                    std::set_intersection(cur_allergens.begin(), cur_allergens.end(), iter->m_allergens.begin(), iter->m_allergens.end(), std::inserter(intersection_allergens, intersection_allergens.begin()));

                    if (intersection_ingredients.size() == 1) {
                        if (intersection_allergens.size() == 1) {
                            /* there is an ingredient that contains allergen */
                            reduce(*intersection_ingredients.begin(), *intersection_allergens.begin());

                            /* start from the begining */
                            cur_iter = m_container.begin();
                            next_iter = cur_iter;

                            break;
                        }
                    }

                    if ((intersection_ingredients.size() > 1) && (intersection_allergens.size() > 0)) {
                        cur_ingredients = intersection_ingredients;
                        cur_allergens = intersection_allergens;
                    }
                }
            }
        }

        /* no matches - then it is over */
    }
};


int main() {
    auto cookbook = read("input.txt");

    allocator filter(cookbook);

    std::cout << "The amount of possibly allergen-free ingredients: " << filter.get_allergen_free() << std::endl;
    std::cout << "The canonical dangerous ingredient list: " << filter.get_dangerous_ingredients() << std::endl;

    return 0;
}
