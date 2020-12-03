#include "UniqueApplication.hpp"

#include <cerrno>
#include <cstdlib>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Logging.hpp"
#include "Utils.hpp"


void UniqueApplication::lock(void) {
	ProcessInfo currentProcess(getpid());
	std::string fileLock = "/tmp/" + currentProcess.getName() + ".pid";

	umask(S_IXGRP | S_IXOTH);
	int fileDescr = open(fileLock.c_str(), O_CREAT | O_RDWR, 0666);
	int result = flock(fileDescr, LOCK_EX | LOCK_NB);


	if (result != 0) {
		int pidOther = -1;
		if (read(fileDescr, &pidOther, sizeof(pidOther)) < 0) {
			LOG(ERROR) << "Impossible to read PID of the process that lock instance (cause: '" << errno << "').";
		}

		if (pidOther > 0) {
			ProcessInfo process(pidOther);
			LOG(ERROR) << "Application '" << process.getName() << "' is already started PID '" << pidOther << "' by user '" << process.getUser() << "'.";
		}

		LOG(VIP) << "Termination due to unique requirement.";
		std::exit(-1);
	}

	int pid = getpid();
	if (write(fileDescr, &pid, sizeof(pid)) < 0) {
		LOG(ERROR) << "Impossible to write PID of the current process (cause: '" << errno << "').";
	}
}
