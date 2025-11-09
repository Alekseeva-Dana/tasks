#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Должно быть больше аргументов\n");
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		pid_t pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(1);
		}

		if (pid == 0) {
			/*дочерний процесс*/
			execlp(argv[i], argv[i], NULL);
			perror("execlp");
			exit(1);
		} else {
			/*родительский процесс ждет завершения текущей команды*/
			wait(NULL);
		}
	}

	return 0;
}
