#include "shell.h"

Tshellvar *shellvars[3];
Tcmd *tcmds[32];
int curarg;
int curtcmd;

void
printtcmd(Tcmd *cmd) {
	printf("argc = %d\tcmd: ", cmd->argc);
	printf("%s args: ", cmd->base);
	for(int i = 0; i < cmd->argc; i++) {
		printf("%s ", cmd->args[i]);
	}
	printf("\n");
	return;
}

void
inittcmds(void) {
	for (int i = 0; i < 32; i++) {
		tcmds[i] = malloc(sizeof(Tcmd));
		tcmds[i]->base = malloc(ARGSIZE * sizeof(char));
		for(int j = 0; j < 32; j++)
			tcmds[i]->args[j] = malloc(ARGSIZE * sizeof(char));
	}
	return;
}

void
initshellvars(void) {
	for (int i = 0; i < 2; i++) {
		shellvars[i] = malloc(sizeof(Tshellvar));
		shellvars[i]->name = malloc(ARGSIZE * sizeof(char));
		shellvars[i]->val = malloc(ARGSIZE * sizeof(char));
		strcpy(shellvars[i]->name, shellvarname[i]);
	}

	strcpy(shellvars[0]->val, "$");
	strcpy(shellvars[1]->val, getenv("HOME"));
	return;
}

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

void
tokcmd(char *cmd) {
	char *cur;

	printf("tokcmd: %s\n", cmd);
	cur = strtok(cmd, " ");
	tcmds[curtcmd]->base = cur;
	tcmds[curtcmd]->args[curarg++] = cur;
	forever {
		tcmds[curtcmd]->args[curarg] = strtok(NULL, " ");
		printf("tokcmd: %s\n", tcmds[curtcmd]->args[curarg]);
		if (tcmds[curtcmd]->args[curarg] == NULL)
			break;
		curarg++;
	}
	tcmds[curtcmd]->argc = curarg;
	curarg = 0;
	curtcmd++;
	return;
}

/* tokline tokenizes a promptline and sends each cmd to tokcmd
 * return 0 == success
 * return -1 == failure but it prolly wont send this
 */
int
tokline(char *str, int *i) {
	char *curcmd, *cmd;

	printf("tokenizing: %s\n", str);
	cmd = strtok(str, ";");
	curcmd = cmd;
	printf("found command: %s\n", curcmd);
	tokcmd(curcmd);

	while((cmd = strtok(NULL, "; ")) != NULL) {
		printf("tokenizing: %s\n", cmd);
		curcmd = cmd;
		printf("found command: %s\n", curcmd);
		tokcmd(curcmd);
	}

	return 0;
}

/* builtinset stores val v in
 * shell variable k
 */
void
builtinset(char *k, char *v) {
	if (!strcmp("PROMPT", k))
		strcpy(shellvars[0]->val, v);
	else if (!strcmp("HOME", k))
		strcpy(shellvars[1]->val, v);
	return;
}

/* builtinecho prints args to stdout
 */
void
builtinecho(char **args, int count) {
	for (int i = 0; i < count && args[i] != NULL; i++)
		printf("%s ", args[i]);
	printf("\n");
	return;
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
				  made by adam\n", shellvars[0]->val,
								   shellvars[1]->val);

	return;
}

/* parse parses the tokens and executes
 * subprocesses accordingly
 * return 1 == exit shell
 * return 0 == success
 * return -1 == failure
 */
int
parse(Tcmd *tcmd) {
	if (!strcmp(tcmd->base, "cd"))
		tcmd->args[1] == NULL ? 
			builtincd(shellvars[1]->val) : 
		builtincd(tcmd->args[1]);

	else if (!strcmp(tcmd->base, "echo"))
		builtinecho(&tcmd->args[1], tcmd->argc);
	
	else if (!strcmp(tcmd->base, "exit"))
		return 1;
	
	else if (!strcmp(tcmd->base, "help"))
		builtinhelp();
	
	else if (!strcmp(tcmd->base, "set"))
		if (tcmd->args[1] == NULL || tcmd->args[2] == NULL)
			return -1;
		else
			builtinset(tcmd->args[1], tcmd->args[2]);
	
	else {
		switch(fork()) {
			/* child */
			case 0:
				if (tcmd->argc > 1)
					if (execvp(tcmd->base, tcmd->args) == -1)
						sprintf(stderr, "execvp: %s\n", strerror(errno));
				else
					if (execlp(tcmd->base, tcmd->base, (char *) NULL) == -1)
						sprintf(stderr, "execlp: %s\n", strerror(errno));
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
	char **tokstr;
	int *count;

	initshellvars();
	inittcmds();
	strcpy(shellvars[0]->val, "$");
	curarg = 0;

	forever {
		curtcmd = 0;
		inittcmds();
		count = malloc(sizeof(int));
		promptline = malloc(PROMPTLINE * sizeof(char));
		tokstr = malloc(ARGSIZE * sizeof(char*));

		for (int i = 0; i < ARGSIZE; ++i)
			tokstr[i] = malloc(ARGSIZE * sizeof(char));

		*count = 0;
		printf("%s ", shellvars[0]->val);
		if (fgets(promptline, PROMPTLINE, stdin) == NULL)
			sprintf(stderr, "fgets(): %s\n", strerror(errno));

		promptline[strcspn(promptline, "\n")] = 0;
		if (tokline(promptline, count) == -1)
			exit(0);

		printf("cmds: %d\n", curtcmd);
		for (int i = 0; i < curtcmd; i++) {
			printf("k parsing ");
			printtcmd(tcmds[i]);
			if(parse(tcmds[i]))
				exit(0);
		}
	}
}
