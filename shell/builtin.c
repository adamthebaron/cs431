#include "shell.h"

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
