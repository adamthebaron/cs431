#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "main.h"

/* changedir moves cwd from cwd to dir,
 * or $HOME if NULL.
 * return 0 == success
 * return -1 == failure
 */
int
changedir(char *dir) {
	if (chdir(dir) == -1) {
		sprintf(stderr, "changedir(): %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int
main(int argc, char **argv) {
	char* promptline, curtok, str;
	char *args[];
	int count;

	while (1) {
		promptline = malloc(PROMPTLINE * sizeof(char));
		printf("% ");
		fgets(&promptline, PROMPTLINE, stdin);

	}

	return 0;
}
