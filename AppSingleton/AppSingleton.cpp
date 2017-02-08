#include <string>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int assertInstance(const char * const pAssertAppFile) {
	int fileDescr = open(pAssertAppFile, O_CREAT | O_RDWR);
	int result = flock(fileDescr, LOCK_EX | LOCK_NB);

	
	if (result != 0) {
		int pidOther = 0;
		read(fileDescr, &pidOther, sizeof(pidOther));
		return pidOther;
	}
	
	int pid = getpid();
	write(fileDescr, &pid, sizeof(pid));

	return 0;
}



int main(int argc, char *argv[]) {
	const char * assertInstanceFile = "/var/run/rcm.pid";
	int pid = assertInstance(assertInstanceFile);
	if (pid == 0) {
		pid = getpid();
		for (int i = 0; i < 10; i++) {
			sleep(1);
		}
	}
	else {
		printf("Application is already started (pid = %d).\n", pid);
	}

	return 0;
}
