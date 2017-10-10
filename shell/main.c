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
tokenize(char *str, int i, char **tokstr) {
	char *curtok;

	curtok = strtok(str, " ");
	printf("curtok: %s\n", curtok);
	tokstr[i] = curtok;
	printf("tokstr[]: %s\n", tokstr[i]);
	*tokstr++;
	i++;
	printf("before loop\n");

	for(;;i++) {
		printf("in loop\n");
		tokstr[i] = strtok(NULL, " ");
		if (tokstr[i] == NULL)
			break;
		printf("tokstr: %s\n", tokstr[i]);
	}
	printf("num of args: %d\n", i);
	return 0;
}

/* chprompt stores s in prompt p
*/
int
chprompt(char *p, char *s) {
	strcpy(p, s);
}

int
main(int argc, char **argv) {
	char* prompt, promptline;
	char *args[ARGSIZE];
	char **tokstr;
	int count;

	while (1) {
		count = 0;
		promptline = malloc(PROMPTLINE * sizeof(char));
		prompt = malloc(ARGSIZE * sizeof(char));
		tokstr = malloc(ARGSIZE * sizeof(char*));

		for (int i = 0; i < ARGSIZE; ++i) {
			tokstr[i] = malloc(ARGSIZE * sizeof(char));
		}

		strcpy(prompt, "$");
		printf("%s ", prompt);
		if (fgets(promptline, PROMPTLINE, stdin) == NULL)
			sprintf(stderr, "fgets(): %s\n", strerror(errno));
		printf("got fgets\n");
		printf("user gave: %s\n", promptline);

		if (tokenize(promptline, count, tokstr) == -1)
			return -1;
	}

	//
	free(prompt);
	free(promptline);
	for (int i = 0; i < ARGSIZE; i++)
		free(tokstr[i]);
	free(tokstr);
	return 0;
}
