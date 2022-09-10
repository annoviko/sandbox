#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class image_algorithm {
private:
    std::vector<std::string> m_image;
    std::string m_algorithm;


public:
    image_algorithm(const std::vector<std::string>& p_image, const std::string& p_algorithm) :
        m_image(p_image),
        m_algorithm(p_algorithm)
    { }


public:
    void apply(const int p_times) {
        expand_borders(300);

        for (int i = 0; i < p_times; i++) {
            m_image = std::move(decode_image());
            fill_borders();
        }
    }

    int count_lit_pixels() const {
        int result = 0;

        for (const auto& row : m_image) {
            for (const char value : row) {
                if (value == '#') {
                    result++;
                }
            }
        }

        return result;
    }

private:
    void show() {
        for (const auto& row : m_image) {
            std::cout << row << std::endl;
        }

        std::cout << std::endl;
    }


    void expand_borders(const std::size_t p_size) {
        for (std::size_t t = 0; t < p_size; t++) {
            m_image.insert(m_image.begin(), std::string(m_image[0].size(), '.'));
            m_image.push_back(std::string(m_image[0].size(), '.'));
        }

        for (std::size_t t = 0; t < p_size; t++) {
            for (std::size_t i = 0; i < m_image.size(); i++) {
                m_image[i].insert(m_image[i].begin(), '.');
                m_image[i].push_back('.');
            }
        }
    }


    void fill_borders() {
        const char filler = m_image[1][1];

        for (std::size_t i = 0; i < m_image.size(); i++) {
            m_image.front()[i] = filler;
            m_image.back()[i] = filler;
        }

        for (std::size_t j = 0; j < m_image[0].size(); j++) {
            m_image[j].front() = filler;
            m_image[j].back() = filler;
        }
    }


    std::vector<std::string> decode_image() const {
        std::vector<std::string> image;
        for (int i = 1; i < (int) m_image.size() - 1; i++) {
            image.push_back(std::string());

            for (int j = 1; j < (int) m_image[0].size() - 1; j++) {
                const char pixel = decode_pixel(i, j);
                image.back().push_back(pixel);
            }
        }

        return image;
    }


    bool is_out_of_image(const int p_i, const int p_j) const {
        return (p_i < 0) || (p_i >= (int) m_image.size()) || (p_j < 0) || (p_j >= (int) m_image[0].size());
    }


    char decode_pixel(const int p_i, const int p_j) const {
        int value = 0;
        int count_out_pixel = 0;
        for (int i = p_i - 1; i <= p_i + 1; i++) {
            for (int j = p_j - 1; j <= p_j + 1; j++) {
                value <<= 1;
                value += (m_image[i][j] == '#') ? 1 : 0;
            }
        }

        return m_algorithm[value];
    }
};


void read_image(std::string& p_algorithm, std::vector<std::string>& p_image) {
    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            break;
        }

        p_algorithm += line;
    }

    for (std::string line; std::getline(stream, line);) {
        p_image.push_back(line);
    }
}


int main() {
    std::string algorithm;
    std::vector<std::string> image;
    read_image(algorithm, image);

    auto algorithm_object = image_algorithm(image, algorithm);
    algorithm_object.apply(2);

    std::cout << "The amount of lit pixels after 2 iterations: " << algorithm_object.count_lit_pixels() << std::endl;

    algorithm_object = image_algorithm(image, algorithm);
    algorithm_object.apply(50);

    std::cout << "The amount of lit pixels after 50 iterations: " << algorithm_object.count_lit_pixels() << std::endl;

    return 0;
}
