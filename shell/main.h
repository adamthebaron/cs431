#define PROMPTLINE 65536
#define ARGSIZE 1024

const char *builtin[] = {
	"cd",
	"echo",
	"exit",
	"help",
	"set"
};

const char *shellvar[] = {
	"PROMPT",
	"HOME",
	"COLOR"
};

int builtincd(char *dir);
int builtinecho(char **args);
int builtinhelp(void);
int builtinset(char *p, char *s);
int tokenize(char *str, int count, char **tokstr);
void about(void);