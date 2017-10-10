#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "main.h"

/* builtincd moves cwd from cwd to dir,
 * or $HOME if NULL.
 * return 0 == success
 * return -1 == failure
 */
int
builtincd (char *dir) {
	if (chdir(dir) == -1) {
		sprintf(stderr, "builtincd(): %s\n", strerror(errno));
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

/* builtin stores val v in
 * shell variable k
 */
int
builtinset(char *v, char *k) {
	
}


/* parse parses the tokens and executes
 * subprocesses accordingly
 * return 1 == exit shell
 * return 0 == success
 * return -1 == failure
 * TODO: does it really need to be this big
 */
int
parse(char **tokstr) {
	if (!strcmp(tokstr[0], "cd")) {
		switch(fork()) {
			/* child */
			case 0:
				tokstr[1] == NULL ? 
					builtincd(getenv("HOME")) : 
				builtincd(tokstr[1]);
				break;
			case -1:
				return -1;
			/* parent */
			default:
				wait(NULL);
				break;
		}
	} else if (!strcmp(tokstr[0], "echo")) {
		switch(fork()) {
			/* child */
			case 0:
				builtinecho(&tokstr[1]);
				break;
			case -1:
				return -1;
			/* parent */
			default:
				wait(NULL);
				break;
		}
	} else if (!strcmp(tokstr[0], "exit")) {
		return 1;
	} else if (!strcmp(tokstr[0], "help")) {
		switch(fork()) {
			/* child */
			case 0:
				builtinhelp();
				break;
			case -1:
				return -1;
			/* parent */
			default:
				wait(NULL);
				break;
		}
	} else if (!strcmp(tokstr[0], "set")) {
		switch(fork()) {
			/* child */
			case 0:
				builtinset(tokstr[1]);
				break;
			case -1:
				return -1;
			/* parent */
			default:
				wait(NULL);
				break;
		}
	}
}

int
main(int argc, char **argv) {
	char *prompt, *promptline;
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

		if (tokenize(promptline, count, tokstr) == -1)
			return -1;
		
		if(parse(tokstr) == -1)
			return -1;
	}

	//im gonna bomb like vietnam under the same name tame one
	free(prompt);
	free(promptline);	
	for (int i = 0; i < ARGSIZE; i++)
		free(tokstr[i]);
	free(tokstr);
	return 0;
}
