#define PROMPTLINE 65536
#define ARGSIZE 1024

int changedir(char *dir);
int tokenize(char *str, int count, char **tokstr);
int chprompt(char *p, char *s);
void about(void);