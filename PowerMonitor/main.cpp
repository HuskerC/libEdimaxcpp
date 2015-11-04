#include <iostream>
#include "../libEdimax.h"
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#endif

int main(int argc, char* argv[])
{
#ifdef __linux__
	syslog(LOG_DAEMON,"Starting PowerMonitor");
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	 we can exit the parent process. */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Open any logs here */

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log any failures here */
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* Log any failures here */
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon-specific initialization goes here */
#endif
	EdimaxDevice device("127.0.0.1");
	/* The Big Loop */
	while (1) {
		EdimaxDevice::PowerInformation info = device.getCurrentPowerUsage();
		syslog(LOG_DAEMON,"PowerInfo:%f",info.amps);
		sleep(30);
	}
}

