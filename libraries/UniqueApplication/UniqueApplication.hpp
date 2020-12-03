#pragma once


#include "ProcessInfo.hpp"


class UniqueApplication {
private:
	UniqueApplication(void) = delete;

	UniqueApplication(const UniqueApplication & pOther) = delete;

	UniqueApplication(UniqueApplication && pOther) = delete;

	~UniqueApplication(void) = delete;

public:
	static void lock(void);
};
