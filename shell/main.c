#include "main.h"

int
main(int argc, char **argv) {
	if(!strcmp(argv[1], "cd"))
		changedir(argv[2]);

	return 0;
}
