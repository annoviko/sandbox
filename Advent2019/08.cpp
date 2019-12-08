#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <unordered_map>


std::string read_transmission() {
    std::string result;

    std::ifstream infile("input.txt");
    std::getline(infile, result);
    infile.close();

    return result;
}


using image_layer = std::vector<std::string>;


class image_decoder {
public:
    static const image_layer no_layer;

private:
    std::size_t m_height;
    std::size_t m_width;
    std::string m_image;

    std::size_t m_cursor;

public:
    image_decoder(std::size_t p_height, std::size_t p_width, const std::string & p_image) :
        m_height(p_height),
        m_width(p_width),
        m_image(p_image),
        m_cursor(0)
    { }

public:
    image_layer get_layer() {
        const std::size_t image_size = m_height * m_width;
        const std::size_t layer_end = image_size + m_cursor;

        if (layer_end > m_image.size()) {
            return image_decoder::no_layer;
        }

        image_layer layer;
        for (; m_cursor < layer_end; m_cursor += m_width) {
            layer.push_back(m_image.substr(m_cursor, m_width));
        }

        return layer;
    }

    std::vector<image_layer> get_layers() {
        std::vector<image_layer> layers;

        auto layer = get_layer();
        while (layer != no_layer) {
            layers.push_back(std::move(layer));
            layer = get_layer();
        }

        return layers;
    }
};


const image_layer image_decoder::no_layer = image_layer();


class image {
private:
    enum colors {
        black = '0',
        white = '1',
        transparent = '2'
    };

private:
    std::vector<image_layer> m_layers;
    std::size_t m_height;
    std::size_t m_width;

public:
    image(const std::size_t p_height, const std::size_t p_width, std::string& p_image) :
        m_layers(image_decoder(p_height, p_width, p_image).get_layers()),
        m_height(p_height),
        m_width(p_width)
    { }

public:
    void show() {
        auto image_description = merge_layers();
        for (const auto& row : image_description) {
            std::string image_line;
            for (const auto symbol : row) {
                image_line += (symbol == colors::black) ? "   " : "###";
            }
            std::cout << image_line << std::endl;
        }
    }

private:
     std::vector<std::string> merge_layers() {
         std::vector<std::string> result(m_height, std::string(m_width, '2'));
         for (std::size_t row = 0; row < result.size(); row++) {
             for (std::size_t col = 0; col < result[row].size(); col++) {
                 result[row][col] = merge_pixel(row, col);
             }
         }

         return result;
    }

    char merge_pixel(const std::size_t p_row, const std::size_t p_col) {
        for (const auto& layer : m_layers) {
            const char layer_pixel_value = layer[p_row][p_col];
            if (layer_pixel_value != colors::transparent) {
                return layer_pixel_value;
            }
        }
        return colors::transparent;
    }
};


std::size_t count_digits(const image_layer& layer, const char digit) {
    std::size_t amount = 0;

    for (const auto& row : layer) {
        amount += std::count(row.begin(), row.end(), digit);
    }

    return amount;
}


std::size_t check_image_integrity(const std::size_t p_height, const std::size_t p_width, const std::string & p_message) {
    image_decoder decoder(p_height, p_width, p_message);

    image_layer optimal;
    std::size_t minimal_zeros = std::numeric_limits<std::size_t>::max();

    image_layer layer = decoder.get_layer();
    while (layer != image_decoder::no_layer) {
        std::size_t zeros = count_digits(layer, '0');
        if (zeros < minimal_zeros) {
            optimal = layer;
            minimal_zeros = zeros;
        }

        layer = decoder.get_layer();
    }

    std::size_t value1 = count_digits(optimal, '1');
    std::size_t value2 = count_digits(optimal, '2');

    return value1 * value2;
}


int main() {
    auto image_encoded = read_transmission();
    std::size_t height = 6, width = 25;

    std::cout << "The number of 1 digits multiplied by the number of 2 digits: " << check_image_integrity(height, width, image_encoded) << std::endl;
    std::cout << std::endl << "Received image:" << std::endl << std::endl;

    image img(height, width, image_encoded);
    img.show();

    return 0;
}