#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int ownout = STDOUT_FILENO;

void die() {
	exit(1);
}

void die_usage() {
	fprintf(fdopen(ownout, "a"), "Usage: fdtool [--pipe fdread fdwrite]* -- program [option]*\n");
	exit(1);
}

int is_opened(int fd) {
	if (fcntl(fd, F_GETFD) < 0) {
		if (errno == EBADF)
			return 0;
		return -errno;
	}
	return 1;
}

void create_pipe(int fdi, int fdo) {
	int p[2];
	if (is_opened(fdi) || is_opened(fdo))
		die();
	if (dup2(2, fdi) < 0 || dup2(2, fdo) < 0)
		die();
	if (pipe(p) < 0)
		die();
	if (dup2(p[0], fdi) < 0 || dup2(p[1], fdo) < 0)
		die();
	if (close(p[0]) < 0 || close(p[1]) < 0)
		die();
}

int main (int argc, char** argv) {
	int i = 1;
	for (; i < argc; ) {
		if (!strcmp("--", argv[i])) {
			if (i + 1 > argc)
				die_usage();
			if (execvp(argv[i + 1], argv + i + 1) < 0)
				return 1;
		} else if (!strcmp("--pipe", argv[i])) {
			if (i + 2 > argc)
				die_usage();
			create_pipe(atoi(argv[i + 1]), atoi(argv[i + 2]));
			i += 3;
		} else
			die_usage();
	}
	die_usage();
	return 0;
}
