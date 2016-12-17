#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "pushfilter.h"

enum {
	Maxarg = 1024,
};

pid_t
pushfilter(int fd, char *cmd, ...)
{
	va_list ap;
	pid_t ret;

	va_start(ap, cmd);
	ret = pushfilterv(fd, cmd, ap);
	va_end(ap);
	return ret;
}

pid_t
pushfilterv(int fd, char *cmd, va_list ap)
{
	char *args[Maxarg];
	size_t i;

	args[0] = cmd;
	for (i = 1; i < Maxarg; i++) {
		args[i] = va_arg(ap, char*);
		if (!args[i])
			break;
	}
	/* overflowed list */
	if (i == Maxarg) {
		errno = E2BIG;
		return -1;
	}
	return pushfilterl(fd, args);
}

pid_t
pushfilterl(int fd, char **cmd)
{
	int outfd[2];
	pid_t pid;

	if (pipe(outfd) != 0)
		return -1;
	pid = fork();
	/* error */
	if (pid == -1){
		return -1;
	/* child */
	} else if (pid == 0) {
		/* input becomes child stdin */
		if (dup2(fd, 0) == -1)
			exit(1);

		/* output becomes child stdout */
		if (dup2(outfd[1], 1) == -1)
			exit(1);

		/* close unused fds */
		close(outfd[0]);
		close(outfd[1]);
		/* don't double close */
		if (fd != 0)
			close(fd);

		execvp(cmd[0], cmd);
		/* if exec returns, we have an error */
		exit(1);
	/* parent */
	} else {
		if (dup2(outfd[0], fd) == -1)
			return -1;
		close(outfd[0]);
		close(outfd[1]);
	}
	return pid;
}
