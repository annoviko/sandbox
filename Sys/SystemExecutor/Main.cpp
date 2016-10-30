#include <iostream>

#include "SystemExecutor.hpp"


int main(int argc, char * argv[]) {
    ExecutorResult result;
    SystemExecutor::execute("pwd -a", result);

    std::cout << result << std::endl;
}
