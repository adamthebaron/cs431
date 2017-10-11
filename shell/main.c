#include "main.h"

void
initshellvar(Tshellvar *vars[3]) {
	for (int i = 0; i < 3; i++) {
		vars[i] = malloc(sizeof(Tshellvar));
		vars[i]->name = malloc(ARGSIZE * sizeof(char));
		vars[i]->val = malloc(ARGSIZE * sizeof(char));
		strcpy(vars[i]->name, shellvarname[i]);
	}

	strcpy(vars[0]->val, "$");
	strcpy(vars[1]->val, getenv("HOME"));
	strcpy(vars[2]->val, "\x031");
	return;
}

/* builtincd moves cwd from cwd to dir,
 * or $HOME if NULL.
 * return 0 == success
 * return -1 == failure
 */
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
	tokstr[i] = curtok;
	*tokstr++;
	i++;

	for(;;i++) {
		tokstr[i] = strtok(NULL, " ");
		if (tokstr[i] == NULL)
			break;
	}
	return 0;
}

/* builtinset stores val v in
 * shell variable k
 */
int
builtinset(char *v, char *k) {

}

/* builtinecho prints args to stdout
 */
int
builtinecho(char **args) {

}

/* builtinhelp prints help to stdout
 */
int
builtinhelp(void) {

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
	printf("parsing out %s newline\n", tokstr[0]);
	if (strcmp(tokstr[0], "cd") == 0) {
		printf("calling builtincd\n");
		switch(fork()) {
			/* child */
			case 0:
				printf("builtincd is passed %s\n", tokstr[1]);
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
	} else if (strcmp(tokstr[0], "echo") == 0) {
		printf("calling builtinecho\n");
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
	} else if (strcmp(tokstr[0], "exit") == 0) {
		printf("calling builtinexit\n");
		return 1;
	} else if (strcmp(tokstr[0], "help") == 0) {
		printf("calling builtinhelp\n");
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
	} else if (strcmp(tokstr[0], "set") == 0) {
		printf("calling builtinset\n");
		switch(fork()) {
			/* child */
			case 0:
				//builtinset(tokstr[1]);
				break;
			case -1:
				return -1;
			/* parent */
			default:
				wait(NULL);
				break;
		}
	} else {
		printf("calling %s\n", tokstr[0]);
		switch(fork()) {
			/* child */
			case 0:
				execvp(tokstr[0], &tokstr[1]);
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
	Tshellvar *shellvars[3];

	initshellvar(shellvars);
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

		promptline[strcspn(promptline, "\n")] = 0;
		if (tokenize(promptline, count, tokstr) == -1)
			return -1;
		
		if(parse(tokstr) == -1)
			return -1;
	}

	//im gonna bomb like vietnam under the same name tame one
	for (int i = 0; i < 3; i++)
		free(shellvars[i]);
	free(shellvars);
	free(prompt);
	free(promptline);	
	for (int i = 0; i < ARGSIZE; i++)
		free(tokstr[i]);
	free(tokstr);
	return 0;
}
