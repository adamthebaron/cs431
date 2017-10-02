#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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
