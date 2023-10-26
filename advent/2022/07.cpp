#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


enum class file_type {
    file,
    directory
};


struct file {
public:
    using ptr = std::shared_ptr<file>;

public:
    std::string name;
    std::uint64_t size = 0;
};


struct folder {
public:
    using ptr = std::shared_ptr<folder>;

public:
    std::string name;
    std::weak_ptr<folder> parent;
    std::map<std::string, file::ptr> files = { };
    std::map<std::string, folder::ptr> folders;
    std::uint64_t size = 0;
};


std::vector<std::string> read_shell_content() {
    std::ifstream stream("input.txt");

    std::vector<std::string> result;
    for (std::string line; std::getline(stream, line); ) {
        result.push_back(line);
    }

    return result;
}


class filesystem_scanner {
private:
    folder::ptr m_root = nullptr;
    std::vector<std::string> m_shell;

    std::vector<std::weak_ptr<folder>> m_folders;

    folder::ptr m_cur = nullptr;

public:
    filesystem_scanner(const std::vector<std::string>& p_shell) :
        m_shell(p_shell)
    { }

public:
    filesystem_scanner& execute() {
        for (int i = 0; i < m_shell.size();) {
            i = handle_command(i);
        }

        analyse_folder_size(m_root);
        m_cur = nullptr;

        return *this;
    }

    std::uint64_t count_folder_sizes(const std::uint64_t p_limit) {
        return count_folder_sizes(m_root, p_limit);
    }

    folder::ptr find_folder_to_delete(const std::uint64_t p_disk_space, const std::uint64_t p_required_free_space) {
        const std::uint64_t remaining_space = p_disk_space - m_root->size;
        const std::uint64_t difference_to_cover = p_required_free_space - remaining_space;

        std::sort(m_folders.begin(), m_folders.end(), [](const std::weak_ptr<folder>& ptr1, const std::weak_ptr<folder>& ptr2) {
            return ptr1.lock()->size < ptr2.lock()->size; 
        });

        folder::ptr optimal = nullptr;
        for (const auto& ptr : m_folders) {
            auto candidate = ptr.lock();

            if (candidate->size > difference_to_cover) {
                if (optimal == nullptr) {
                    optimal = candidate;
                }
                else if (candidate->size < optimal->size) {
                    optimal = candidate;
                }
            }
        }

        return optimal;
    }

private:
    std::uint64_t count_folder_sizes(const folder::ptr& p_folder, const std::uint64_t p_limit) {
        std::uint64_t total_size = 0;

        for (const auto& pair : p_folder->folders) {
            total_size += count_folder_sizes(pair.second, p_limit);
        }

        if (p_folder->size <= p_limit) {
            total_size += p_folder->size;
        }

        return total_size;
    }


    void analyse_folder_size(const folder::ptr& p_folder) {
        std::uint64_t folder_size = 0;
        for (const auto & pair : p_folder->folders) {
            analyse_folder_size(pair.second);
            folder_size += pair.second->size;
        }

        for (const auto& pair : p_folder->files) {
            folder_size += pair.second->size;
        }

        p_folder->size = folder_size;
        m_folders.push_back(p_folder);
    }

    std::uint64_t handle_command(const std::uint64_t p_cur) {
        if (m_shell[p_cur][0] != '$') {
            throw std::invalid_argument("the line is not command");
        }

        std::stringstream stream(m_shell[p_cur]);
        stream.ignore(2);

        std::string instruction;
        stream >> instruction;

        if (instruction == "cd") {
            std::string argument;
            stream >> argument;

            handle_instruction_cd(argument);
            return p_cur + 1;
        }
        else if (instruction == "ls") {
            return handle_instruction_ls(p_cur);
        }
    }


    std::uint64_t handle_instruction_ls(const std::uint64_t p_cur) {
        for (std::uint64_t i = p_cur + 1; i < m_shell.size(); i++) {
            const std::string& output = m_shell[i];

            if (output[0] == '$') {
                return i;
            }

            std::stringstream stream(output);
            std::string arg1, name;
            stream >> arg1 >> name;

            if (arg1 == "dir") {
                auto iter = m_cur->folders.find(name);
                if (iter == m_cur->folders.cend()) {
                    folder::ptr child_folder = std::make_shared<folder>();
                    child_folder->name = name;
                    child_folder->parent = m_cur;

                    m_cur->folders[name] = child_folder;
                }
            }
            else {
                auto iter = m_cur->files.find(name);
                if (iter == m_cur->files.cend()) {
                    file::ptr child_file = std::make_shared<file>();
                    child_file->name = name;
                    child_file->size = std::stoull(arg1);

                    m_cur->files[name] = child_file;
                }
            }
        }
    }


    void handle_instruction_cd(const std::string& p_argument) {
        if (p_argument == "/") {
            if (m_root != nullptr) {
                m_cur = m_root;
            }
            else {
                m_root = std::make_shared<folder>();
                m_root->name = "/";
                m_cur = m_root;
            }
        }
        else if (p_argument == "..") {
            if (m_cur->parent.lock() == nullptr) {
                throw std::invalid_argument("parent folder does not exist");
            }

            m_cur = m_cur->parent.lock();
        }
        else {
            auto iter = m_cur->folders.find(p_argument);
            if (iter == m_cur->folders.cend()) {
                folder::ptr child_folder = std::make_shared<folder>();
                child_folder->name = p_argument;
                child_folder->parent = m_cur;

                m_cur->folders[p_argument] = child_folder;
            }
            else {
                m_cur = iter->second;
            }
        }
    }
};


int main() {
    const auto shell_content = read_shell_content();

    auto scanner = filesystem_scanner(shell_content).execute();

    std::cout << "The total size (at most 100000): " << scanner.count_folder_sizes(100000) << std::endl;
    std::cout << "The size of the optimal directory to delete: " << scanner.find_folder_to_delete(70000000, 30000000)->size << std::endl;

    return 0;
}
