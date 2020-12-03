#ifndef EXECUTORRESULT_HPP_
#define EXECUTORRESULT_HPP_


#include <ostream>
#include <string>


class ExecutorResult {
private:
    std::string textResult;
    std::string textError;
    int         resultValue;

public:
    ExecutorResult(void);

    ExecutorResult(const int pResultValue, const std::string & pTextResult, const std::string & pTextError);

    ExecutorResult(const ExecutorResult & pOther);

    ExecutorResult(ExecutorResult && pOther);

    virtual ~ExecutorResult(void);

public:
    void clear(void);

public:
    inline std::string & getTextResult(void) {
        return textResult;
    }

    inline void setTextResult(const std::string & pTextResult) {
        textResult = pTextResult;
    }

    inline std::string & getTextError(void) {
        return textError;
    }

    inline void setTextError(const std::string & pTextError) {
        textError = pTextError;
    }

    inline int getExitCode(void) const {
        return resultValue;
    }

    inline void setExitCode(const int pResultValue) {
        resultValue = pResultValue;
    }

public:
    friend std::ostream & operator<< (std::ostream & pOutputStream, const ExecutorResult & pResult);
};


#endif
