#include <iostream>

#include "SystemExecutor.hpp"


int main(int argc, char * argv[]) {
    ExecutorResult result;

    SystemExecutor::execute("pwd -a", result);
    std::cout << result << std::endl;

    SystemExecutor::execute("pwd", result);
    std::cout << result << std::endl;

    SystemExecutor::execute("ifconfig", result);
    std::cout << result << std::endl;
}
