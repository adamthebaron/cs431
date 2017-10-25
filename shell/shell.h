#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#define PROMPTLINE 65536
#define ARGSIZE 1024
#define HISTSIZE 4096

#define type(x) _Generic((x), \
	int*:	"int*", \
	char:	"char", \
	char*:	"char*", \
	char**:	"char**")

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
void builtinecho(char **args, int count);
void builtinhelp(void);
void builtinset(char *p, char *s);
int tokenize(char *str, int *count, char **tokstr);
void initshellvar(Tshellvar *vars[3]);
void about(void);
void printtokstr(char **tokstr, int *count);
void freeall(int count, ...);
int main(int argc, char **argv);