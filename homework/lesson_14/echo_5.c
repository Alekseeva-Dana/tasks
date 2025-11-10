#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		char newline = '\n';
		write(1, &newline, 1);
		return 0;
	}

	char *arg = argv[1];
	while (*arg) {
		write(1, arg, 1);
		arg++;
	}

	if (argc > 2) {
		char space = ' ';
		write(1, &space, 1);
	}

	return main(argc - 1, &argv[1]);
}
