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


/* tokenize tokenizes a string and places it in tokstr
 * return 0 == success
 * return -1 == failure but it prolly wont send this
 */
int
tokenize(char *str, int *count, char **tokstr) {
	char *curtok;

	curtok = strtok(str, " ");
	printf("curtok: %s\n", curtok);
	**tokstr = curtok;
	printf("tokstr[%d]: %s\n", *count, **tokstr);
	*tokstr++;
	*count++;
	while(*curtok != NULL) {
		**tokstr = strtok(NULL, " ");
		printf("tokstr: %s\n", **tokstr);
		*tokstr++;
		*count++;
	}
	return 0;
}

int
main(int argc, char **argv) {
	char *promptline;
	char *args[ARGSIZE];
	char **tokstr;
	int count;

	while (1) {
		count = 0;
		promptline = malloc(PROMPTLINE * sizeof(char));
		printf("allocated memory for prompt\n");
		tokstr = malloc(ARGSIZE * sizeof(char*));
		printf("allocated memory for tokstr\n");
		for (int i = 0; i < ARGSIZE; i++) {
			*tokstr = malloc(ARGSIZE * sizeof(char));
			*tokstr++;
		}
		printf("allocated memory for tokstr[]\n");
		printf("$ ");
		fgets(promptline, PROMPTLINE, stdin);
		printf("user gave: %s\n", promptline);
		if (tokenize(promptline, &count, tokstr) == -1)
			return -1;
		
	}

	return 0;
}
