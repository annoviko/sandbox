#include <iostream>
#include <set>
#include <string>


int main() {
    std::string line;

    std::set<std::string> files_wo_indexes;
    while (std::getline(std::cin, line) && !line.empty()) {
        files_wo_indexes.insert(line);
    }

    std::set<std::string> files_with_index;
    std::set<std::string> indexes_wo_file;

    while (std::getline(std::cin, line) && !line.empty()) {
        std::size_t position = line.size() - 1;
        for (int i = 0; i < 2; i++) {
            position = line.rfind('_', position) - 1;
        }

        std::string filename = line.substr(0, position + 1);

        auto iter = files_wo_indexes.find(filename);
        if (iter == files_wo_indexes.cend()) {
            indexes_wo_file.insert(line);
        }
        else {
            files_with_index.insert(filename);
        }
    }

    for (const std::string& filename : files_with_index) {
        files_wo_indexes.erase(filename);
    }

    if (files_wo_indexes.empty() && indexes_wo_file.empty()) {
        std::cout << "No mismatches." << std::endl;
    }
    else {
        for (const std::string& index : indexes_wo_file) {
            std::cout << "F " << index << std::endl;
        }
        for (const std::string& filename : files_wo_indexes) {
            std::cout << "I " << filename << std::endl;
        }
    }

    return 0;
}