#include <iostream>
#include <vector>


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> existed_gnomes;
    std::vector<bool> missing_gnomes;

    for (int i = 0; i < n; i++) {
        missing_gnomes.push_back(true);
    }

    for (int i = 0; i < m; i++) {
        int gnome_index = 0;
        std::cin >> gnome_index;

        existed_gnomes.push_back(gnome_index);
        missing_gnomes[gnome_index - 1] = false;
    }

    int j = 0;
    for (int i = 0; i < existed_gnomes.size(); i++) {
        while ((j < missing_gnomes.size()) && (existed_gnomes[i] > (j + 1))) {
            if (missing_gnomes[j] == true) {
                std::cout << j + 1 << std::endl;
            }

            j++;
        }

        std::cout << existed_gnomes[i] << std::endl;
    }

    while (j < missing_gnomes.size()) {
        if (missing_gnomes[j] == true) {
            std::cout << j + 1 << std::endl;
        }

        j++;
    }

    return 0;
}