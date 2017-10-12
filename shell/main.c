#include "shell.h"

Tshellvar *shellvars[3];

void
printtokstr(char **tokstr, int *count) {
	printf("count = %d\ntokstr: ", *count);
	for(int i = 0; i < *count; i++) {
		printf("%s", tokstr[i]);
	}

	printf("\n");
	return;
}

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
tokenize(char *str, int *i, char **tokstr) {
	char *curtok;

	curtok = strtok(str, " ");
	tokstr[*i] = curtok;
	(*i)++;

	for(;;(*i)++) {
		tokstr[*i] = strtok(NULL, " ");
		if (tokstr[*i] == NULL)
			break;
	}

	return 0;
}

/* builtinset stores val v in
 * shell variable k
 */
void
builtinset(char *k, char *v) {
	switch(*k) {
		case 'P':
			strcpy(shellvars[0]->val, v);
			break;
		case 'H':
			strcpy(shellvars[1]->val, v);
			break;
		case 'C':
			strcpy(shellvars[2]->val, v);
			break;
	}
	return;
}

/* builtinecho prints args to stdout
 */
int
builtinecho(char **args) {

}

/* builtinhelp prints help to stdout
 */
void
builtinhelp(void) {
	printf("built in programs:\n \
	help: prints this help output\n \
	cd: change dir to arg, or HOME if no arg is passed\n \
	echo: echoes any args to stdout\n \
	exit: quit shell\n \
	set: set shell vars (PROMPT HOME COLOR)\n \
	env:\n \
		PROMPT: %s\n \
		HOME:   %s\n \
		COLOR:  %s\n \
				  made by adam\n", shellvars[0]->val,
								   shellvars[1]->val,
								   shellvars[2]->val);

	return;
}

/* parse parses the tokens and executes
 * subprocesses accordingly
 * return 1 == exit shell
 * return 0 == success
 * return -1 == failure
 */
int
parse(char **tokstr, int *count) {
	//printtokstr(tokstr, count);
	
	if (!strcmp(tokstr[0], "cd"))
		tokstr[1] == NULL ? 
			builtincd(shellvars[1]->val) : 
		builtincd(tokstr[1]);

	else if (!strcmp(tokstr[0], "echo"))
		builtinecho(&tokstr[1]);
	
	else if (!strcmp(tokstr[0], "exit"))
		return 1;
	
	else if (!strcmp(tokstr[0], "help"))
		builtinhelp();
	
	else if (!strcmp(tokstr[0], "set"))
		if (tokstr[1] == NULL || tokstr[2] == NULL)
			return -1;
		else
			builtinset(tokstr[1], tokstr[2]);
	
	else {
		switch(fork()) {
			/* child */
			case 0:
				if (*count > 1)
					execvp(tokstr[0], &tokstr[1]);
				else
					execlp(tokstr[0], tokstr[0], NULL);
				break;
			case -1:
				return -1;
			/* parent */
			default:
				wait(NULL);
				break;
		}
	}
	return 0;
}

int
main(int argc, char **argv) {
	char *promptline;
	char *args[ARGSIZE];
	char **tokstr;
	int *count;

	initshellvar(shellvars);
	while (1) {
		count = malloc(sizeof(int));
		promptline = malloc(PROMPTLINE * sizeof(char));
		tokstr = malloc(ARGSIZE * sizeof(char*));

		for (int i = 0; i < ARGSIZE; ++i) {
			tokstr[i] = malloc(ARGSIZE * sizeof(char));
		}

		*count = 0;
		strcpy(shellvars[0]->val, "$");
		printf("%s ", shellvars[0]->val);
		if (fgets(promptline, PROMPTLINE, stdin) == NULL)
			sprintf(stderr, "fgets(): %s\n", strerror(errno));

		promptline[strcspn(promptline, "\n")] = 0;
		if (tokenize(promptline, count, tokstr) == -1)
			goto Free;

		if(parse(tokstr, count))
			goto Free;
	}

	//im gonna bomb like vietnam under the same name tame one
	Free:
		for (int i = 0; i < 3; i++)
			free(shellvars[i]);
		free(shellvars);
		free(promptline);	
		for (int i = 0; i < ARGSIZE; i++)
			free(tokstr[i]);
		free(tokstr);
	return 0;
}
