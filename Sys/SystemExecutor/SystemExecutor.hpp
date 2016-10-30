#ifndef SYSTEMEXECUTOR_HPP_
#define SYSTEMEXECUTOR_HPP_


#include <string>
#include <vector>

#include "ExecutorResult.hpp"


class SystemExecutor {
private:
    static const int PERFORMER_FAILURE_CODE;
    static const int BUFFER_READER_SIZE;

public:
    static const int SYSTEM_EXECUTER_FAILURE_CODE;
    static const int SYSTEM_EXECUTER_SUCCESS_CODE;

private:
    SystemExecutor(void);

    ~SystemExecutor(void);

public:
    static int execute(const std::string & pCommand);

    static int execute(const std::string & pCommand, ExecutorResult & pOutputResult);

private:
    static void readChannel(const int pChannel, std::string & pTextResult);
};


#endif
