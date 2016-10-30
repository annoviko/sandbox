#include "ExecutorResult.hpp"

#include <limits>
#include <utility>


ExecutorResult::ExecutorResult(void) :
    textResult(""),
    textError(""),
    resultValue(std::numeric_limits<int>::max())
{ }


ExecutorResult::ExecutorResult(const int pResultValue, const std::string & pTextResult, const std::string & pTextError) :
    textResult(pTextResult),
    textError(pTextError),
    resultValue(pResultValue)
{ }


ExecutorResult::ExecutorResult(const ExecutorResult & pOther) :
    textResult(pOther.textResult),
    textError(pOther.textError),
    resultValue(pOther.resultValue)
{ }


ExecutorResult::ExecutorResult(ExecutorResult && pOther) :
    textResult(std::move(pOther.textResult)),
    textError(std::move(pOther.textError)),
    resultValue(std::move(pOther.resultValue))
{ }


ExecutorResult::~ExecutorResult(void)
{ }


void ExecutorResult::clear(void) {
    textResult.clear();
    textError.clear();
    resultValue = std::numeric_limits<int>::max();
}


std::ostream & operator<<(std::ostream & pOutputStream, const ExecutorResult & pResult) {
    pOutputStream << "'" << std::to_string(pResult.resultValue) << "', '" << pResult.textResult << "', '" << pResult.textError << "'.";
    return pOutputStream;
}
