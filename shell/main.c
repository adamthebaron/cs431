#include "shell.h"

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
clearcmds(void) {
	for (int i = 0; i < curtcmd; i++) {
		for (int j = 0; j < tcmds[i]->argc; j++) {
			memcpy(tcmds[i]->args[j], 0, strlen(tcmds[i]->args[j]));
		}
		memcpy(tcmds[i]->base, 0, strlen(tcmds[i]->base));
	}
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

/* parse parses the tokens and executes
 * subprocesses accordingly
 * return 1 == exit shell
 * return 0 == success
 * return -1 == failure
 */
int
parse(Tcmd *tcmd) {
	printf("parsing ");
	printtcmd(tcmd);
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

	initshellvars();
	inittcmds();
	strcpy(shellvars[0]->val, "$");
	curarg = 0;

	forever {
		curtcmd = 0;
		promptline = malloc(PROMPTLINE * sizeof(char));
		tokstr = malloc(ARGSIZE * sizeof(char*));

		for (int i = 0; i < ARGSIZE; ++i)
			tokstr[i] = malloc(ARGSIZE * sizeof(char));

		printf("%s ", shellvars[0]->val);
		if (fgets(promptline, PROMPTLINE, stdin) == NULL)
			sprintf(stderr, "fgets(): %s\n", strerror(errno));

		promptline[strcspn(promptline, "\n")] = 0;
		if (tokline(promptline) == -1)
			exit(0);

		for (int i = 0; i < curtcmd; i++) {
			printf("parsing %d commands\n", curtcmd);
			if(parse(tcmds[i]))
				exit(0);
		}
		clearcmds();
	}
}
