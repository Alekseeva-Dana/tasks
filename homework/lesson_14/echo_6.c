#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		char newline[] = "\n";
		write(1, newline, 1);
		return 0;
	}

	char **echo_argv = malloc((argc + 1) * sizeof(char*));

	echo_argv[0] = "echo";

	for (int i = 1; i < argc; i++) {
		echo_argv[i] = argv[i];
	}
	echo_argv[argc] = NULL;

	execvp("echo", echo_argv);
	free(echo_argv);
	return 1;
}
