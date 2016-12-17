#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pushfilter.h"

int
main(int argc, char **argv)
{
	char buf[1024];
	size_t n;
	int status;

	/* we don't need the pid here */
	pushfilter(0, "sed", "s/foo/bar/g", NULL);
	/*
	I can modify what happens on stdin, without library support!
	Beware, the stuff that's already in the buffer won't be touched.
	*/
	fgets(buf, 1024, stdin);
	printf("%s\n", buf);

	/* and wait for the subcommand to exit */
	wait(&status);
}
