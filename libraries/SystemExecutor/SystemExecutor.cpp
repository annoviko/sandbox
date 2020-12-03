#include "SystemExecutor.hpp"

#include <cstring>
#include <limits>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>


const int SystemExecutor::PERFORMER_FAILURE_CODE = std::numeric_limits<int>::max();

const int SystemExecutor::BUFFER_READER_SIZE = 2048;

const int SystemExecutor::SYSTEM_EXECUTER_FAILURE_CODE = -1;
const int SystemExecutor::SYSTEM_EXECUTER_SUCCESS_CODE = 0;


SystemExecutor::SystemExecutor(void)
{ }


SystemExecutor::~SystemExecutor(void)
{ }


int SystemExecutor::execute(const std::string & pCommand) {
    ExecutorResult result;
    execute(pCommand, result);

    return result.getExitCode();
}


int SystemExecutor::execute(const std::string & pCommand, ExecutorResult & pOutputResult) {
    pOutputResult.clear();

    int outputChannel[2];
    int errorChannel[2];

    int result = pipe(outputChannel);
    if (result < 0) {
        std::cout << "Impossible to create channel to obtain output stream." << std::endl;
        return SYSTEM_EXECUTER_FAILURE_CODE;
    }

    result = pipe(errorChannel);
    if (result < 0) {
        std::cout << "Impossible to create channel to obtain error stream." << std::endl;
        return SYSTEM_EXECUTER_FAILURE_CODE;
    }

    pid_t childProcessId = fork();
    if (childProcessId < 0) {
        std::cout << "Impossible to create process for command execution." << std::endl;
        return SYSTEM_EXECUTER_FAILURE_CODE;
    }
    else if (!childProcessId) {
        /* child process */
        close(outputChannel[0]);
        close(errorChannel[0]);

        dup2(outputChannel[1], STDOUT_FILENO);  /* redirect output stream to output channel for parent */
        dup2(errorChannel[1], STDERR_FILENO);   /* redirect error stream to error channel for parent */

        close(outputChannel[1]);
        close(errorChannel[1]);

        execl("/bin/sh", "sh", "-c", pCommand.c_str(), (char *) 0);

        /* shouldn't be reached in case of execl success */
        std::cerr << "Impossible to execute command using 'execl' system call.";
        _exit(PERFORMER_FAILURE_CODE);
    }

    close(outputChannel[1]);    /* close write end of channel */
    close(errorChannel[1]);     /* close write end of channel */

    readChannel(outputChannel[0], pOutputResult.getTextResult());
    readChannel(errorChannel[0], pOutputResult.getTextError());

    close(outputChannel[0]);    /* close read end of channel */
    close(errorChannel[0]);     /* close read end of channel */

    int returnValue = 0;
    waitpid(childProcessId, &returnValue, 0);

    pOutputResult.setExitCode(WEXITSTATUS(returnValue));

    return SYSTEM_EXECUTER_SUCCESS_CODE;
}


void SystemExecutor::readChannel(const int pChannel, std::string & pTextResult) {
    char buffer[BUFFER_READER_SIZE];
    std::memset(&buffer, 0, BUFFER_READER_SIZE);

    read(pChannel, &buffer, BUFFER_READER_SIZE);
    pTextResult.assign(buffer);

    if (pTextResult.back() == '\n') {
        pTextResult.erase(pTextResult.end() - 1);
    }
}
