#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PROMPTLINE 65536
#define ARGSIZE 1024
#define HISTSIZE 4096

const char *builtin[] = {
	"cd",
	"echo",
	"exit",
	"help",
	"set"
};

const char *shellvarname[] = {
	"PROMPT",
	"HOME",
	"COLOR"
};

typedef struct tshellvar {
	char *name;
	char *val;
} Tshellvar;

Tshellvar *defshellvar[3] = {
	{
		"PROMPT",
		"$"
	},
	{
		"HOME",
		""
	},
	{
		"COLOR",
		""
	}
};

int builtincd(char *dir);
int builtinecho(char **args);
void builtinhelp(void);
void builtinset(char *p, char *s);
int tokenize(char *str, int count, char **tokstr);
void initshellvar(Tshellvar *vars[3]);
void about(void);
int main(int argc, char **argv);