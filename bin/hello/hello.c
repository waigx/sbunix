#include <stdio.h>
#include <libio.h>
#include <stdlib.h>
#include <const.h>
#include <libstr.h>

int main(int argc, char* argv[], char* envp[]) {
	char **envp_ptr = malloc(lenstrarr(envp) * (sizeof(char *) + 1));
	cpystrarr(envp_ptr, envp);
	while (*(++envp_ptr) != NULL)
		writeline(*envp_ptr, 1);
	free(envp_ptr);
	return 0;
}
