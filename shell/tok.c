#include "shell.h"

void
tokcmd(char *cmd) {
	char *cur;

	printf("tokcmd received %s\nparsing command number %d\n", cmd, curtcmd);
	cur = strtok(cmd, " ");
	strcpy(tcmds[curtcmd]->base, cur);
	printf("base: %s\n", tcmds[curtcmd]->base);
	strcpy(tcmds[curtcmd]->args[curarg++], cur);
	forever {
		//tcmds[curtcmd]->args[curarg] = strtok(NULL, " ");
		if ((cur = strtok(NULL, " ")) == NULL)
			break;
		strcpy(tcmds[curtcmd]->args[curarg], cur);
		printf("arg: %s\n", tcmds[curtcmd]->args[curarg]);
		curarg++;
	}
	tcmds[curtcmd]->argc = curarg;
	printf("number of args: %d\n", tcmds[curtcmd]->argc);
	curarg = 0;
	curtcmd++;
	printf("curtcmd is now %d\n", curtcmd);
	return;
}

/* tokline tokenizes a promptline and sends each cmd to tokcmd
 * return 0 == success
 * return -1 == failure but it prolly wont send this
 */
int
tokline(char *str) {
	char *curcmd, *cmd;

	curcmd = malloc(ARGSIZE * sizeof(char));
	printf("tokenizing: %s\n", str);
	cmd = strtok(str, ";");
	strcpy(curcmd, cmd);
	printf("found command: %s\n", curcmd);
	tokcmd(curcmd);

	while((cmd = strtok(NULL, ";")) != NULL) {
		printf("tokenizing: %s\n", cmd);
		strcpy(curcmd, cmd);
		printf("found command: %s\n", curcmd);
		tokcmd(curcmd);
	}

	free(curcmd);
	return 0;
}
